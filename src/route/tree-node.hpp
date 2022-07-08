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

#ifndef NLSR_TREE_NODE_HPP
#define NLSR_TREE_NODE_HPP

#include <iostream>
#include <memory>
#include <list>

namespace nlsr {
  template<typename ValueType>
  class TreeNode : public std::enable_shared_from_this<TreeNode<ValueType>> {
    using TreeNodeList = std::list<std::shared_ptr<TreeNode<ValueType>>>;
    using const_iterator = typename TreeNodeList::const_iterator;

  public:
    /*! \brief Instantiates a new tree node holding the provided value.
     *  \param value The value of the tree node.
     */
    explicit TreeNode(ValueType value)
      : m_value(value)
    {
      m_children = TreeNodeList();
    }

    /*! \brief Returns the value encapsulated by this tree node.
     *  \return The encapsulated value.
     */
    ValueType
    getValue()
    {
      return m_value;
    }

    /*!
     *  \brief Returns the parent tree node or nullptr if there is no parent node.
     *  \return The parent tree node.
     */
    std::shared_ptr<TreeNode<ValueType>>
    getParent()
    {
      return m_parent.lock();
    }

    /*! \brief Returns whether the tree node is a leaf node in the tree or not. A node is considered a leaf if it does
     *  not have any child nodes associated.
     *  \return true if the node is a leaf node, false otherwise.
     */
    bool
    isLeaf()
    {
      return m_children.size() == 0;
    }

    /*! \brief Creates and adds a new tree node with the provided value as child to this tree node.
     *  \param node The value of the tree node that should be created and added as child node.
     *  \return The added tree node.
     */
    std::shared_ptr<TreeNode<ValueType>>
    addChild(ValueType nodeValue);

    /*! \brief Adds a tree node as child to this tree node.
     *  \param node The tree node that should be added as child node.
     *  \return true if the node was added, false if not and the child is already assigned to another parent.
     */
    bool
    addChild(std::shared_ptr<TreeNode<ValueType>> node);

    /*! \brief Removes a tree node as child from this tree node.
     *  \param node The tree node that should be removed as child node.
     *  \return true if the node was removed, false if not and the node is not assigned to this node as child.
     */
    bool
    removeChild(std::shared_ptr<TreeNode<ValueType>> node);

    /*! \brief Removes this tree node from the tree.
     *  \return true if the node was removed, false if not and the node is not assigned to any parent node.
     * @return
     */
    bool
    remove();

    const_iterator
    beginChildren();

    const_iterator
    endChildren();

    friend std::ostream&
    operator<<(std::ostream& os, TreeNode<ValueType>& node)
    {
      os << node.m_value << " {";
      bool sep = false;

      for (auto itr = node.beginChildren(); itr != node.endChildren(); itr++) {
        if (sep) {
          os << ", ";
        } else {
          sep = true;
        }

        os << (*itr)->getValue();
      }

      os << "}\n";

      for (auto itr = node.beginChildren(); itr != node.endChildren(); itr++) {
        os << **itr;
      }

      return os;
    }

  private:
    ValueType m_value;
    std::weak_ptr<TreeNode<ValueType>> m_parent;
    TreeNodeList m_children;
  };

  template<typename ValueType>
  std::shared_ptr<TreeNode<ValueType>>
  TreeNode<ValueType>::addChild(ValueType nodeValue) {
    auto node = std::make_shared<TreeNode<ValueType>>(nodeValue);
    m_children.push_back(node);
    node->m_parent = TreeNode<ValueType>::shared_from_this();
    return node;
  }

  template<typename T>
  bool
  TreeNode<T>::addChild(std::shared_ptr<TreeNode<T>> node) {
    TreeNode<T> *n = node.get();

    if (n->m_parent.lock().get() != nullptr) {
      return false;
    }

    m_children.push_back(node);
    n->m_parent = TreeNode<T>::shared_from_this();
    return true;
  }

  template<typename T>
  bool
  TreeNode<T>::removeChild(std::shared_ptr<TreeNode<T>> node) {
    TreeNode<T> *n = node.get();

    if (n->m_parent.lock().get() != this) {
      return false;
    }

    m_children.remove_if([node](std::shared_ptr<TreeNode<T>> ptr) {
      return ptr == node;
    });

    n->m_parent = std::shared_ptr<TreeNode<T>>(nullptr);
    return true;
  }

  template<typename T>
  bool TreeNode<T>::remove() {
    auto parentPtr = m_parent.lock();

    if (parentPtr.get() == nullptr) {
      return false;
    }

    return parentPtr->removeChild(this->shared_from_this());
  }

  template<typename T>
  inline typename TreeNode<T>::const_iterator TreeNode<T>::beginChildren() {
    return m_children.cbegin();
  }

  template<typename T>
  inline typename TreeNode<T>::const_iterator TreeNode<T>::endChildren() {
    return m_children.cend();
  }
} // namespace nlsr

#endif //NLSR_TREE_NODE_HPP
