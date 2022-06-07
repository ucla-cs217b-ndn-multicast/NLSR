/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2022,  The University of Memphis,
 *                           Regents of the University of California
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

#include "tests/boost-test.hpp"

#include "route/tree-node.hpp"

namespace nlsr {
  namespace test {
    BOOST_AUTO_TEST_SUITE(TestTreeNode)
      BOOST_AUTO_TEST_CASE(AddTreeNodeChildren)
      {
        auto node1 = std::make_shared<TreeNode<int>>(5);
        auto node2 = std::make_shared<TreeNode<int>>(6);
        auto node3 = std::make_shared<TreeNode<int>>(7);

        node1->addChild(node2);
        node1->addChild(node3);

        auto it = node1->beginChildren();

        BOOST_CHECK_EQUAL(node2->getValue(), (*it)->getValue());
        it++;

        BOOST_CHECK_EQUAL(node3->getValue(), (*it)->getValue());
        it++;

        BOOST_CHECK(node1->endChildren() == it);
      }

      BOOST_AUTO_TEST_CASE(RemoveTreeNodeChildren)
      {
        auto node1 = std::make_shared<TreeNode<int>>(5);
        auto node2 = std::make_shared<TreeNode<int>>(6);
        auto node3 = std::make_shared<TreeNode<int>>(7);

        node1->addChild(node2);
        node1->addChild(node3);

        node1->removeChild(node2);

        auto it = node1->beginChildren();
        BOOST_CHECK_EQUAL(node3->getValue(), (*it)->getValue());
        it++;

        BOOST_CHECK(node1->endChildren() == it);

        node3->remove();

        it = node1->beginChildren();
        BOOST_CHECK(node1->endChildren() == it);
      }

    BOOST_AUTO_TEST_SUITE_END()
  }
}