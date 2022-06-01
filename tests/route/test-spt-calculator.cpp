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
      std::cerr << *(calculator.getTree().getRoot());
    }

    BOOST_AUTO_TEST_SUITE_END()
  }
}