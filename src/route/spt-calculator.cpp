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

#include "spt-calculator.hpp"

namespace nlsr {
  ShortestPathTreeCalculator::ShortestPathTreeCalculator(size_t nNodes, double **adjMatrix)
    : m_nNodes(nNodes)
    , m_adjMatrix(adjMatrix)
  {
    queue = new int32_t[m_nNodes];
    parents = new int32_t[m_nNodes];
    distances = new double[m_nNodes];
    m_tree = Tree<int32_t>();
  }

  ShortestPathTreeCalculator::~ShortestPathTreeCalculator()
  {
    delete [] queue;
    delete [] parents;
    delete [] distances;
  }

  void
  ShortestPathTreeCalculator::enqueue(const int32_t node, const int32_t parent, const double distance)
  {
    for (size_t i = 0; i < queueHead; i++) {
      if (queue[i] == node) {
        return;
      }
    }

    for (size_t i = queueHead; i < queueTail; i++) {
      if (queue[i] == node) {
        if (distance < distances[node]) {
          parents[node] = parent;
          distances[node] = distance;
        }

        return;
      }
    }

    queue[queueTail++] = node;
    parents[node] = parent;
    distances[node] = distance;
  }

  void
  ShortestPathTreeCalculator::sortQueueByDistance()
  {
    for (size_t i = queueHead ; i < queueTail ; i++) {
      for (size_t j = i + 1; j < queueTail; j++) {
        if (distances[queue[j]] < distances[queue[i]]) {
          int tempU = queue[j];
          queue[j] = queue[i];
          queue[i] = tempU;
        }
      }
    }
  }

  void
  ShortestPathTreeCalculator::calculateTree(int32_t rootNodeId, const std::set<int32_t>& inclNodes)
  {
    // Limit resulting tree to only contain included nodes, if provided
    bool limit = !inclNodes.empty();
    std::set<int32_t> remInclNodes(inclNodes);

    // Add root node to tree and initialize queue
    enqueue(rootNodeId, rootNodeId, 0);
    m_tree.setRoot(rootNodeId);

    // Return if included nodes contains only root node
    if (limit && remInclNodes.erase(rootNodeId) && remInclNodes.empty()) {
      return;
    }

    int32_t head = queue[queueHead];

    while (true) {
      // Enqueue adjacent nodes of current queue head
      for (size_t i = 0; i < m_nNodes; i++) {
        int32_t distance = m_adjMatrix[head][i];

        if (distance < 0) {
          continue;
        }

        enqueue(i, head, distances[head] + distance);
      }

      if (++queueHead < queueTail) {
        // Select next node in queue and add it to the tree
        sortQueueByDistance();
        head = queue[queueHead];
        auto parent = m_tree[parents[head]];
        m_tree.addChild(parent, head);

        // Stop algorithm once tree contains all included nodes
        if (limit && remInclNodes.erase(head) && remInclNodes.empty()) {
          break;
        }
      } else {
        break;
      }
    }

    pruneTree(inclNodes);
  }

  void
  ShortestPathTreeCalculator::pruneTree(const std::set<int32_t>& inclNodes)
  {
    m_tree.pruneIf([inclNodes] (const Tree<int32_t>::TreeNodePtr& nodePtr) {
      return inclNodes.find(nodePtr->getValue()) == inclNodes.end();
    });
  }
}
