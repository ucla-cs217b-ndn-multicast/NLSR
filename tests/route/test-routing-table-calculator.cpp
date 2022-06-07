/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2020,  The University of Memphis,
 *                           Regents of the University of California,
 *                           Arizona Board of Regents.
 *
 * This file is part of NLSR (Named-data Link State Routing).
 * See AUTHORS.md for complete list of NLSR authors and contributors.
 *
 * NLSR is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NLSR is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NLSR, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "route/routing-table-calculator.hpp"

#include "adjacency-list.hpp"
#include "lsdb.hpp"
#include "nlsr.hpp"
#include "../test-common.hpp"
#include "route/map.hpp"
#include "route/routing-table.hpp"
#include "adjacent.hpp"

#include <ndn-cxx/util/dummy-client-face.hpp>

namespace nlsr {
  namespace test {

    static const ndn::time::system_clock::TimePoint MAX_TIME =
        ndn::time::system_clock::TimePoint::max();

    class MulticastCalculatorFixture : public BaseFixture
    {
    public:
      MulticastCalculatorFixture()
          : face(m_ioService, m_keyChain)
          , conf(face, m_keyChain)
          , confProcessor(conf)
          , nlsr(face, m_keyChain, conf)
          , routingTable(nlsr.m_routingTable)
          , lsdb(nlsr.m_lsdb)
      {
        setUpTopology();
      }

      // Triangle topology with routers A, B, C connected
      void setUpTopology()
      {
        Adjacent a(ROUTER_A_NAME, ndn::FaceUri(ROUTER_A_FACE), 0, Adjacent::STATUS_ACTIVE, 0, 0);
        Adjacent b(ROUTER_B_NAME, ndn::FaceUri(ROUTER_B_FACE), 0, Adjacent::STATUS_ACTIVE, 0, 0);
        Adjacent c(ROUTER_C_NAME, ndn::FaceUri(ROUTER_C_FACE), 0, Adjacent::STATUS_ACTIVE, 0, 0);

        // Router A
        b.setLinkCost(LINK_AB_COST);
        c.setLinkCost(LINK_AC_COST);

        AdjacencyList& adjacencyListA = conf.getAdjacencyList();
        adjacencyListA.insert(b);
        adjacencyListA.insert(c);

        AdjLsa adjA(a.getName(), 1, MAX_TIME, 2, adjacencyListA);
        lsdb.installLsa(std::make_shared<AdjLsa>(adjA));

        // Router B
        a.setLinkCost(LINK_AB_COST);
        c.setLinkCost(LINK_BC_COST);

        AdjacencyList adjacencyListB;
        adjacencyListB.insert(a);
        adjacencyListB.insert(c);

        AdjLsa adjB(b.getName(), 1, MAX_TIME, 2, adjacencyListB);
        lsdb.installLsa(std::make_shared<AdjLsa>(adjB));

        // Router C
        a.setLinkCost(LINK_AC_COST);
        b.setLinkCost(LINK_BC_COST);

        AdjacencyList adjacencyListC;
        adjacencyListC.insert(a);
        adjacencyListC.insert(b);

        AdjLsa adjC(c.getName(), 1, MAX_TIME, 2, adjacencyListC);
        lsdb.installLsa(std::make_shared<AdjLsa>(adjC));

        auto lsaRange = lsdb.getLsdbIterator<AdjLsa>();
        map.createFromAdjLsdb(lsaRange.first, lsaRange.second);
      }

    public:
      ndn::util::DummyClientFace face;
      ConfParameter conf;
      DummyConfFileProcessor confProcessor;
      Nlsr nlsr;
      Map map;

      RoutingTable& routingTable;
      Lsdb& lsdb;

      static const ndn::Name ROUTER_A_NAME;
      static const ndn::Name ROUTER_B_NAME;
      static const ndn::Name ROUTER_C_NAME;

      static const std::string ROUTER_A_FACE;
      static const std::string ROUTER_B_FACE;
      static const std::string ROUTER_C_FACE;

      static const double LINK_AB_COST;
      static const double LINK_AC_COST;
      static const double LINK_BC_COST;
    };

    const ndn::Name MulticastCalculatorFixture::ROUTER_A_NAME = "/ndn/site/%C1.Router/this-router";
    const ndn::Name MulticastCalculatorFixture::ROUTER_B_NAME = "/ndn/site/%C1.Router/b";
    const ndn::Name MulticastCalculatorFixture::ROUTER_C_NAME = "/ndn/site/%C1.Router/c";

    const std::string MulticastCalculatorFixture::ROUTER_A_FACE = "udp4://10.0.0.1";
    const std::string MulticastCalculatorFixture::ROUTER_B_FACE = "udp4://10.0.0.2";
    const std::string MulticastCalculatorFixture::ROUTER_C_FACE = "udp4://10.0.0.3";

    const double MulticastCalculatorFixture::LINK_AB_COST = 5;
    const double MulticastCalculatorFixture::LINK_AC_COST = 10;
    const double MulticastCalculatorFixture::LINK_BC_COST = 17;

    BOOST_FIXTURE_TEST_SUITE(TestMulticastRoutingTableCalculator, MulticastCalculatorFixture)

      BOOST_AUTO_TEST_CASE(Basic)
      {
        std::set<ndn::Name> destinations {
            MulticastCalculatorFixture::ROUTER_A_NAME,
            MulticastCalculatorFixture::ROUTER_B_NAME,
            MulticastCalculatorFixture::ROUTER_C_NAME
        };

        MulticastRoutingTableCalculator calculator(map.getMapSize(), map, lsdb, conf);
        NexthopList nexthopList = calculator.calculateNextHopList(destinations);

        BOOST_REQUIRE_EQUAL(nexthopList.getNextHops().size(), 2);

        for (const NextHop& hop : nexthopList) {
          std::string faceUri = hop.getConnectingFaceUri();

          BOOST_CHECK(faceUri == ROUTER_B_FACE || faceUri == ROUTER_C_FACE);
        }
      }

    BOOST_AUTO_TEST_SUITE_END()

  } // namespace test
} // namespace nlsr
