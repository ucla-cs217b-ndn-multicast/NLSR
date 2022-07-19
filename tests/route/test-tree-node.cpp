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
      BOOST_AUTO_TEST_CASE(GetTreeNodeValue)
      {
        const int testValue = 5;
        auto node1 = std::make_shared<TreeNode<int>>(testValue);

        BOOST_CHECK_EQUAL(testValue, node1->getValue());
      }

      BOOST_AUTO_TEST_CASE(GetTreeNodeParent)
      {
        auto node1 = std::make_shared<TreeNode<int>>(5);
        auto node2 = std::make_shared<TreeNode<int>>(6);

        node1->addChild(node2);

        BOOST_CHECK_EQUAL(node1, node2->getParent());
      }

      BOOST_AUTO_TEST_CASE(GetTreeNodeNullParent)
      {
        auto node = std::make_shared<TreeNode<int>>(5);

        BOOST_CHECK(nullptr == node->getParent());
      }

      BOOST_AUTO_TEST_CASE(TreeNodeIsLeaf)
      {
        auto node1 = std::make_shared<TreeNode<int>>(5);
        auto node2 = std::make_shared<TreeNode<int>>(6);

        node1->addChild(node2);

        BOOST_CHECK_EQUAL(false, node1->isLeaf());
        BOOST_CHECK_EQUAL(true, node2->isLeaf());
      }

      BOOST_AUTO_TEST_CASE(AddTreeNodeChildrenByValue)
      {
        auto node1 = std::make_shared<TreeNode<int>>(5);
        auto node2 = node1->addChild(6);
        auto node3 = node1->addChild(7);

        BOOST_CHECK_EQUAL(6, node2->getValue());
        BOOST_CHECK_EQUAL(7, node3->getValue());

        auto it = node1->beginChildren();

        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK_EQUAL(node3, *it);
        it++;
        BOOST_CHECK(node1->endChildren() == it);
      }

      BOOST_AUTO_TEST_CASE(AddTreeNodeChildren)
      {
        auto node1 = std::make_shared<TreeNode<int>>(5);
        auto node2 = std::make_shared<TreeNode<int>>(6);
        auto node3 = std::make_shared<TreeNode<int>>(7);

        bool result = node1->addChild(node2);
        result &= node1->addChild(node3);

        BOOST_CHECK(result);

        auto it = node1->beginChildren();
        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK_EQUAL(node3, *it);
        it++;
        BOOST_CHECK(node1->endChildren() == it);
      }

      BOOST_AUTO_TEST_CASE(AddAssociatedTreeNodeChildren)
      {
        auto node1 = std::make_shared<TreeNode<int>>(5);
        auto node2 = std::make_shared<TreeNode<int>>(6);
        auto node3 = std::make_shared<TreeNode<int>>(7);

        node1->addChild(node2);
        bool result = node3->addChild(node2);

        BOOST_CHECK(!result);

        auto it = node1->beginChildren();
        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK(node1->endChildren() == it);
      }

      BOOST_AUTO_TEST_CASE(RemoveTreeNodeChildren)
      {
        auto node1 = std::make_shared<TreeNode<int>>(5);
        auto node2 = std::make_shared<TreeNode<int>>(6);

        node1->addChild(node2);
        bool result = node1->removeChild(node2);

        BOOST_CHECK(result);
        BOOST_CHECK(node1->isLeaf());
      }

      BOOST_AUTO_TEST_CASE(RemoveUnassociatedTreeNodeChildren)
      {
        auto node1 = std::make_shared<TreeNode<int>>(5);
        auto node2 = std::make_shared<TreeNode<int>>(6);

        bool result = node1->removeChild(node2);

        BOOST_CHECK(!result);
      }

      BOOST_AUTO_TEST_CASE(TreeNodeRemoveSelf)
      {
        auto node1 = std::make_shared<TreeNode<int>>(5);
        auto node2 = std::make_shared<TreeNode<int>>(6);

        node1->addChild(node2);
        bool result = node2->remove();

        BOOST_CHECK(result);
        BOOST_CHECK(node1->isLeaf());
      }

      BOOST_AUTO_TEST_CASE(TreeNodeRemoveUnassociatedSelf)
      {
        auto node = std::make_shared<TreeNode<int>>(5);
        bool result = node->remove();

        BOOST_CHECK(!result);
      }

    BOOST_AUTO_TEST_SUITE_END()
  }
}