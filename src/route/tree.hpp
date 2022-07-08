/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
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
 *
 **/

#ifndef NLSR_TREE_HPP
#define NLSR_TREE_HPP

#include <memory>
#include <unordered_map>
#include <ndn-cxx/name.hpp>
#include <utility>

#include "tree-node.hpp"

namespace nlsr {
  template <typename TreeNodeValueType>
  class Tree {
  public:
    using TreeNodeT = TreeNode<TreeNodeValueType>;
    using TreeNodePtr = std::shared_ptr<TreeNodeT>;
    using TreeNodePool = std::unordered_map<TreeNodeValueType, TreeNodePtr>;

    /*! \brief Constructs a new empty tree.
     */
    Tree()
    {
      m_root = std::shared_ptr<TreeNodeT>(nullptr);
    }

    /*! \brief Returns the root node of the multicast tree.
     *  \return The root node.
     */
    TreeNodePtr
    getRoot() {
      return m_root;
    }

    /*! \brief Creates a tree node with the provided value and sets it as the root node of the multicast tree.
     *  \param rootValue The value of the tree node.
     *  \return The created root node.
     */
    Tree::TreeNodePtr
    setRoot(const TreeNodeValueType& rootValue);

    /*! \brief Adds a child node with a given value to a node of the multicast tree.
     *  \param parent The parent node to which a child node is added.
     *  \param childValue The value of the child node.
     *  \return true if the child was added, false if not and the child is already assigned to another parent node
     */
    Tree::TreeNodePtr
    addChild(const TreeNodePtr &parent, const TreeNodeValueType& childValue);

    /*! \brief Adds a child node to a node of the multicast tree.
     *  \param parent The parent node to which a child node is added.
     *  \param child The child node.
     *  \return true if the child was added, false if not and the child is already assigned to another parent node
     */
    bool
    addChild(const TreeNodePtr &parent, const TreeNodePtr &child);

    /*! \brief Removes a child node from a node of the multicast tree.
     *  \param parent The parent node of which a child node is removed.
     *  \param child The child node.
     *  \return true if the child was removed, false if not and the child is not assigned to the specified parent node
     */
    bool
    removeChild(const TreeNodePtr &parent, const TreeNodePtr &child);

    /*! \brief Prunes of branches of that tree where the leaf nodes satisfy the given predicate.
     *  \param predicate The predicate for leaf nodes.
     */
    void
    pruneIf(std::function<bool(TreeNodePtr)> predicate)
    {
      pruneIf(m_root, predicate);
    }

    /*! \brief Returns whether or not the tree contains a node with the given value.
     *  \param value The value to check for.
     *  \return true if the tree contains a node with that value, false otherwise.
     */
    bool
    contains(const TreeNodeValueType& value);

    /*! \brief Returns the node at a given index.
     *  \param The index.
     *  \return The node of that tree that corresponds to the specified index.
     */
    TreeNodePtr
    operator[](const TreeNodeValueType &index);

  private:
    TreeNodePool m_mctnPool;
    TreeNodePtr m_root;

    void
    pruneIf(const TreeNodePtr& node, std::function<bool(TreeNodePtr)> predicate);
  };

  template <typename TreeNodeValueType>
  typename Tree<TreeNodeValueType>::TreeNodePtr
  Tree<TreeNodeValueType>::setRoot(const TreeNodeValueType &rootValue) {
    auto root = std::make_shared<TreeNodeT>(rootValue);
    m_root = root;
    m_mctnPool[rootValue] = root;
    return root;
  }

  template <typename TreeNodeValueType>
  typename Tree<TreeNodeValueType>::TreeNodePtr
  Tree<TreeNodeValueType>::addChild(const Tree::TreeNodePtr &parent,
                                    const TreeNodeValueType &childValue) {
    auto child = parent->addChild(childValue);
    m_mctnPool[childValue] = child;
    return child;
  }

  template <typename TreeNodeValueType>
  bool
  Tree<TreeNodeValueType>::addChild(const Tree::TreeNodePtr &parent,
                                    const Tree::TreeNodePtr &child) {
    if (!parent->addChild(child)) {
      return false;
    }

    m_mctnPool[child->getValue()] = child;
    return true;
  }

  template <typename TreeNodeValueType>
  bool
  Tree<TreeNodeValueType>::removeChild(const Tree::TreeNodePtr &parent,
                                       const Tree::TreeNodePtr &child) {
    if (!parent->removeChild(child)) {
      return false;
    }

    m_mctnPool.erase(child->getValue());
    return true;
  }

  template<typename TreeNodeValueType>
  void
  Tree<TreeNodeValueType>::pruneIf(const Tree::TreeNodePtr &node, std::function<bool(TreeNodePtr)> predicate)
  {
    for (auto itr = node->beginChildren(); itr != node->endChildren(); itr++) {
      pruneIf(*itr, predicate);
    }

    for (auto itr = node->beginChildren(); itr != node->endChildren();) {
      auto child = *itr;
      itr++;

      if (!child->isLeaf()) {
        continue;
      }

      if (predicate(child)) {
        removeChild(node, child);
      }
    }
  }

  template <typename TreeNodeValueType>
  bool
  Tree<TreeNodeValueType>::contains(const TreeNodeValueType &value) {
    return m_mctnPool.find(value) != m_mctnPool.end();
  }

  template <typename TreeNodeValueType>
  typename Tree<TreeNodeValueType>::TreeNodePtr
  Tree<TreeNodeValueType>::operator[](const TreeNodeValueType &index) {
    return m_mctnPool[index];
  }
} // namespace nlsr

#endif //NLSR_TREE_HPP
