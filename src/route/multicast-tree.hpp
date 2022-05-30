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

#ifndef NLSR_MULTICAST_TREE_HPP
#define NLSR_MULTICAST_TREE_HPP

#include <memory>
#include <unordered_map>
#include <ndn-cxx/name.hpp>

#include "tree-node.hpp"

namespace nlsr {
  class MulticastTree {
  public:
    using MulticastTreeNodeValue = ndn::Name;
    using MulticastTreeNode = TreeNode<MulticastTreeNodeValue>;
    using MulticastTreeNodePtr = std::shared_ptr<MulticastTreeNode>;
    using MulticastTreeIndex = ndn::Name;
    using NodePool = std::unordered_map<MulticastTreeIndex, MulticastTreeNodePtr>;

    explicit MulticastTree(std::shared_ptr<MulticastTreeNodeValue> rootValue);

    /*! \brief Returns the root node of the multicast tree.
     *  \return The root node.
     */
    MulticastTreeNodePtr
    getRoot() {
      return m_root;
    }

    /*! \brief Adds a child node with a given value to a node of the multicast tree.
     *  \param parent The parent node to which a child node is added.
     *  \param childValue The value of the child node.
     *  \return true if the child was added, false if not and the child is already assigned to another parent node
     */
    MulticastTree::MulticastTreeNodePtr
    addChild(const MulticastTreeNodePtr &parent, const std::shared_ptr<MulticastTreeNodeValue>& childValue);

    /*! \brief Adds a child node to a node of the multicast tree.
     *  \param parent The parent node to which a child node is added.
     *  \param child The child node.
     *  \return true if the child was added, false if not and the child is already assigned to another parent node
     */
    bool
    addChild(const MulticastTreeNodePtr &parent, const MulticastTreeNodePtr &child);

    /*! \brief Removes a child node from a node of the multicast tree.
     *  \param parent The parent node of which a child node is removed.
     *  \param child The child node.
     *  \return true if the child was removed, false if not and the child is not assigned to the specified parent node
     */
    bool
    removeChild(const MulticastTreeNodePtr &parent, const MulticastTreeNodePtr &child);

    /*! \brief Returns the node at a given index.
     *  \param The index.
     *  \return The node of that tree that corresponds to the specified index.
     */
    MulticastTreeNodePtr
    operator[](const MulticastTreeIndex &index);

  private:
    NodePool m_mctnPool;
    MulticastTreeNodePtr m_root;
  };
} // namespace nlsr

#endif //NLSR_MULTICAST_TREE_HPP
