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

#ifndef NLSR_SPT_CALCULATOR_HPP
#define NLSR_SPT_CALCULATOR_HPP

#include <set>

#include "map.hpp"
#include "routing-table-calculator.hpp"
#include "tree.hpp"

namespace nlsr {
  class ShortestPathTreeCalculator {
  public:
    /*! \brief Constructs a new shortest-path tree calculator.
     *
     * @param nNodes The amount of network nodes.
     * @param adjMatrix The adjacency matrix representing the network topology.
     */
    explicit ShortestPathTreeCalculator(size_t nNodes, double** adjMatrix);

    ~ShortestPathTreeCalculator();

    /*! \brief Returns the calculated shortest path tree.
     *  \return The shortest path tree.
     */
    Tree<int32_t>
    getTree()
    {
      return m_tree;
    }

    /*! \brief Calculates a shortest path tree from the given graph adjacency matrix from a specified root node with the
     *  specified included nodes. The resulting tree does not contain leaf nodes that are not tagged as included.
     *
     *  \param rootNodeId The identifier of the root node.
     *  \param inclNodes The list of included nodes.
     */
    void
    calculateTree(int32_t rootNodeId, const std::set<int32_t>& inclNodes);

  private:
    size_t m_nNodes;
    double** m_adjMatrix;
    int32_t* queue;
    size_t queueHead = 0;
    size_t queueTail = 0;
    int32_t* parents;
    double* distances;
    Tree<int32_t> m_tree;

    /*! \brief Enqueues a node in the processing queue and keeps track of its distance to the root node of the tree as
     *  well as its parent node.
     *
     * @param node The node to enqueue.
     * @param parent The parent node.
     * @param distance The node's distance to the root node of the tree.
     */
    void
    enqueue(int32_t node, int32_t parent, double distance);

    /*! \brief Sorts the entries of the processing queue by ascending order of their distances to the root node of the
     *  tree.
     */
    void
    sortQueueByDistance();

    /*! \brief Prunes of all branches that do not contain any of the included nodes.
     *  \param inclNodes The nodes to be included in the tree.
     */
    void
    pruneTree(const std::set<int32_t>& inclNodes);
  };
}

#endif //NLSR_SPT_CALCULATOR_HPP