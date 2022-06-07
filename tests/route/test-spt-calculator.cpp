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

#include <iostream>

#include "tests/boost-test.hpp"

#include "route/spt-calculator.hpp"

namespace nlsr {
  namespace test {
    BOOST_AUTO_TEST_SUITE(TestMulticastTreeCalculator)

    BOOST_AUTO_TEST_CASE(ShortestPathTreeCalculation)
    {
      const size_t nRouters = 6;

      double adjMatrixMA[nRouters][nRouters] = {
          {0, 1, 2, -1, -1, -1},
          {1, 0, -1, 4, -1, 6},
          {2, -1, 0, 1, 6, -1},
          {-1, 4, 1, 0, 1, 5},
          {-1, -1, 6, 1, 0, -1},
          {-1, 6, -1, 5, -1, 0}
      };

      double* adjMatrix[nRouters] = {
          adjMatrixMA[0],
          adjMatrixMA[1],
          adjMatrixMA[2],
          adjMatrixMA[3],
          adjMatrixMA[4],
          adjMatrixMA[5]
      };

      ShortestPathTreeCalculator calculator(nRouters, adjMatrix);
      std::set<int32_t> include = {1, 4, 5};
      calculator.calculateTree(0, include);
    }

    BOOST_AUTO_TEST_SUITE_END()
  }
}