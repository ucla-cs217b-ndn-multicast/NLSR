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

#include "route/tree.hpp"

namespace nlsr {
  namespace test {
    BOOST_AUTO_TEST_SUITE(TestTree)
      BOOST_AUTO_TEST_CASE(GetTreeRoot)
      {
        Tree<int> tree;

        BOOST_CHECK(nullptr == tree.getRoot());

        Tree<int>::TreeNodePtr root = std::make_shared<TreeNode<int>>(5);
        tree.m_root = root;

        BOOST_CHECK_EQUAL(root, tree.getRoot());
      }

      BOOST_AUTO_TEST_CASE(SetTreeRoot)
      {
        Tree<int> tree;
        const int value = 5;
        auto root = tree.setRoot(value);

        BOOST_CHECK_EQUAL(value, root->getValue());
        BOOST_CHECK_EQUAL(root, tree.getRoot());

        auto newRoot = tree.setRoot(value + 1);
        BOOST_CHECK(nullptr == newRoot);
        BOOST_CHECK_EQUAL(root, tree.getRoot());
      }

      BOOST_AUTO_TEST_CASE(AddTreeNodeByValue)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = tree.addChild(root, 6);
        auto node2 = tree.addChild(root, 7);
        auto node3 = tree.addChild(node1, 8);

        auto it = root->beginChildren();
        BOOST_CHECK_EQUAL(node1, *it);
        it++;
        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK(root->endChildren() == it);

        it = node1->beginChildren();
        BOOST_CHECK_EQUAL(node3, *it);
        it++;
        BOOST_CHECK(node1->endChildren() == it);
      }

      BOOST_AUTO_TEST_CASE(AddTreeNodeByValueDuplicate)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = tree.addChild(root, 6);
        auto node2 = tree.addChild(root, 7);
        auto node3 = tree.addChild(node1, 5);

        BOOST_CHECK(nullptr == node3);

        auto it = root->beginChildren();
        BOOST_CHECK_EQUAL(node1, *it);
        it++;
        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK(root->endChildren() == it);
        BOOST_CHECK(node1->beginChildren() == node1->endChildren());
      }

      BOOST_AUTO_TEST_CASE(AddTreeNode)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = std::make_shared<Tree<int>::TreeNodeT>(6);
        auto node2 = std::make_shared<Tree<int>::TreeNodeT>(7);
        auto node3 = std::make_shared<Tree<int>::TreeNodeT>(8);

        bool result = tree.addChild(root, node1);
        result &= tree.addChild(root, node2);
        result &= tree.addChild(node1, node3);

        BOOST_CHECK(result);

        auto it = root->beginChildren();
        BOOST_CHECK_EQUAL(node1, *it);
        it++;
        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK(root->endChildren() == it);

        it = node1->beginChildren();
        BOOST_CHECK_EQUAL(node3, *it);
        it++;
        BOOST_CHECK(node1->endChildren() == it);
      }

      BOOST_AUTO_TEST_CASE(AddTreeNodeWithChildren)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = std::make_shared<Tree<int>::TreeNodeT>(6);
        auto node2 = std::make_shared<Tree<int>::TreeNodeT>(7);
        auto node3 = std::make_shared<Tree<int>::TreeNodeT>(8);

        node1->addChild(node3);
        bool result = tree.addChild(root, node1);

        BOOST_CHECK(!result);

        BOOST_CHECK(root->beginChildren() == root->endChildren());
      }

      BOOST_AUTO_TEST_CASE(AddTreeNodeDuplicate)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = std::make_shared<Tree<int>::TreeNodeT>(6);
        auto node2 = std::make_shared<Tree<int>::TreeNodeT>(7);
        auto node3 = std::make_shared<Tree<int>::TreeNodeT>(5);

        bool result = tree.addChild(root, node1);
        result &= tree.addChild(root, node2);

        BOOST_CHECK(result);

        result &= tree.addChild(node1, node3);

        BOOST_CHECK(!result);

        auto it = root->beginChildren();
        BOOST_CHECK_EQUAL(node1, *it);
        it++;
        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK(root->endChildren() == it);

        BOOST_CHECK(node1->beginChildren() == node1->endChildren());
      }

      BOOST_AUTO_TEST_CASE(AddAssociatedTreeNode)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = std::make_shared<Tree<int>::TreeNodeT>(6);
        auto node2 = std::make_shared<Tree<int>::TreeNodeT>(7);
        auto node3 = std::make_shared<Tree<int>::TreeNodeT>(8);

        bool result = tree.addChild(root, node1);
        result &= tree.addChild(root, node2);
        result &= tree.addChild(root, node3);

        BOOST_CHECK(result);

        result = tree.addChild(node1, node3);

        BOOST_CHECK(!result);

        auto it = root->beginChildren();

        BOOST_CHECK_EQUAL(node1, *it);
        it++;
        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK_EQUAL(node3, *it);
        it++;
        BOOST_CHECK(root->endChildren() == it);

        BOOST_CHECK(node1->beginChildren() == node1->endChildren());
      }

      BOOST_AUTO_TEST_CASE(RemoveTreeNode)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = tree.addChild(root, 6);
        auto node2 = tree.addChild(root, 7);

        bool result = tree.removeChild(root, node1);

        BOOST_CHECK(result);

        auto it = root->beginChildren();

        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK(root->endChildren() == it);
      }

      BOOST_AUTO_TEST_CASE(RemoveTreeNodeWithChildren)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = tree.addChild(root, 6);
        auto node2 = tree.addChild(root, 7);
        auto node3 = tree.addChild(node1, 8);

        bool result = tree.removeChild(root, node1);

        BOOST_CHECK(!result);

        auto it = root->beginChildren();
        BOOST_CHECK_EQUAL(node1, *it);
        it++;
        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK(root->endChildren() == it);

        it = node1->beginChildren();
        BOOST_CHECK_EQUAL(node3, *it);
        it++;
        BOOST_CHECK(node1->endChildren() == it);
      }

      BOOST_AUTO_TEST_CASE(RemoveUnassociatedTreeNode)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = tree.addChild(root, 6);
        auto node2 = tree.addChild(root, 7);

        bool result = tree.removeChild(node1, node2);

        BOOST_CHECK(!result);

        auto it = root->beginChildren();
        BOOST_CHECK_EQUAL(node1, *it);
        it++;
        BOOST_CHECK_EQUAL(node2, *it);
        it++;
        BOOST_CHECK(root->endChildren() == it);
      }

      BOOST_AUTO_TEST_CASE(TreeContainsNodeWithValue)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = tree.addChild(root, 6);
        auto node2 = tree.addChild(root, 7);

        BOOST_CHECK(tree.contains(5));
        BOOST_CHECK(tree.contains(6));
        BOOST_CHECK(tree.contains(7));
        BOOST_CHECK(!tree.contains(8));

        tree.removeChild(root, node1);

        BOOST_CHECK(!tree.contains(6));
      }

      BOOST_AUTO_TEST_CASE(GetTreeNodeByValue)
      {
        Tree<int> tree;
        auto root = tree.setRoot(5);
        auto node1 = tree.addChild(root, 6);
        auto node2 = tree.addChild(root, 7);

        BOOST_CHECK_EQUAL(root, tree[5]);
        BOOST_CHECK_EQUAL(node1, tree[6]);
        BOOST_CHECK_EQUAL(node2, tree[7]);
      }
    BOOST_AUTO_TEST_SUITE_END()
  }
}