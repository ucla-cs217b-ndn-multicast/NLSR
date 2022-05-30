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

#include "multicast-tree.hpp"

namespace nlsr {
  MulticastTree::MulticastTree(std::shared_ptr<MulticastTreeNodeValue> rootValue) {
    m_root = std::make_shared<MulticastTreeNode>(rootValue);
  }

  MulticastTree::MulticastTreeNodePtr
  MulticastTree::addChild(const MulticastTree::MulticastTreeNodePtr &parent,
                          const std::shared_ptr<MulticastTree::MulticastTreeNodeValue>& childValue) {
    auto child = parent->addChild(childValue);
    m_mctnPool[*childValue] = child;
    return child;
  }

  bool
  MulticastTree::addChild(const MulticastTree::MulticastTreeNodePtr &parent,
                          const MulticastTree::MulticastTreeNodePtr &child) {
    if (!parent->addChild(child)) {
      return false;
    }

    m_mctnPool[*(child->getValue())] = child;
    return true;
  }

  bool
  MulticastTree::removeChild(const MulticastTree::MulticastTreeNodePtr &parent,
                             const MulticastTree::MulticastTreeNodePtr &child) {
    if (!parent->removeChild(child)) {
      return false;
    }

    m_mctnPool.erase(*(child->getValue()));
    return true;
  }

  MulticastTree::MulticastTreeNodePtr
  MulticastTree::operator[](const MulticastTree::MulticastTreeIndex &index) {
    return m_mctnPool[index];
  }
}