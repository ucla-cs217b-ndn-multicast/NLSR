//
// Created by Lukas Trommer on 5/25/22.
//

#include "tests/boost-test.hpp"

#include "route/tree-node.hpp"

namespace nlsr {
  namespace test {
    BOOST_AUTO_TEST_SUITE(TestTreeNode)

    BOOST_AUTO_TEST_CASE(AddTreeNodeChildren)
    {
      auto node1 = std::make_shared<TreeNode<int>>(std::make_shared<int>(5));
      auto node2 = std::make_shared<TreeNode<int>>(std::make_shared<int>(6));
      auto node3 = std::make_shared<TreeNode<int>>(std::make_shared<int>(7));

      node1->addChild(node2);
      node1->addChild(node3);

      auto it = node1->beginChildren();
      BOOST_CHECK_EQUAL(*(node2->getValue().get()), *(it->lock()->getValue().get()));
      it++;

      BOOST_CHECK_EQUAL(*(node3->getValue().get()), *(it->lock()->getValue().get()));
      it++;

      BOOST_CHECK(node1->endChildren() == it);
    }

      BOOST_AUTO_TEST_CASE(RemoveTreeNodeChildren)
      {
        auto node1 = std::make_shared<TreeNode<int>>(std::make_shared<int>(5));
        auto node2 = std::make_shared<TreeNode<int>>(std::make_shared<int>(6));
        auto node3 = std::make_shared<TreeNode<int>>(std::make_shared<int>(7));

        node1->addChild(node2);
        node1->addChild(node3);

        node1->removeChild(node2);

        auto it = node1->beginChildren();
        BOOST_CHECK_EQUAL(*(node3->getValue().get()), *(it->lock()->getValue().get()));
        it++;

        BOOST_CHECK(node1->endChildren() == it);

        node3->remove();

        it = node1->beginChildren();
        BOOST_CHECK(node1->endChildren() == it);
      }

    BOOST_AUTO_TEST_SUITE_END()
  }
}