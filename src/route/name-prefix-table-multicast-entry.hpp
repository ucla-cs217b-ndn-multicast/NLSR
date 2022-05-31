/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2020,  The University of Memphis,
 *                           Regents of the University of California,
 *                           Arizona Board of Regents.
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

#ifndef NLSR_NAME_PREFIX_TABLE_MULTICAST_ENTRY_HPP
#define NLSR_NAME_PREFIX_TABLE_MULTICAST_ENTRY_HPP

#include "test-access-control.hpp"
#include "nexthop-list.hpp"

#include <set>

namespace nlsr {

class NamePrefixTableMulticastEntry { 
public: 
  NamePrefixTableMulticastEntry(const ndn::Name& name)
    : m_namePrefix(name) 
  { 
  }

  const ndn::Name&
  getNamePrefix() const
  {
    return m_namePrefix;
  }

  // TODO: Maybe I should learn how C++ iterators work someday...
  const std::set<ndn::Name>&
  getMemberRouters() const
  {
    return m_memberRouters; 
  }

  size_t
  size() const
  {
    return m_memberRouters.size(); 
  }

  void
  addMemberRouter(const ndn::Name& name)
  {
    m_memberRouters.insert(name); 
  }

  void
  removeMemberRouter(const ndn::Name& name)
  {
    m_memberRouters.insert(name); 
  }

  bool
  containsMemberRouter(const ndn::Name& name) const
  {
    return m_memberRouters.find(name) != m_memberRouters.end(); 
  }

private: 
  ndn::Name m_namePrefix; 

PUBLIC_WITH_TESTS_ELSE_PRIVATE:
  std::set<ndn::Name> m_memberRouters; 
  NexthopList m_nexthopList;
}; 

bool
operator==(const NamePrefixTableMulticastEntry& lhs, const NamePrefixTableMulticastEntry& rhs);

bool
operator==(const NamePrefixTableMulticastEntry& lhs, const ndn::Name& rhs);

std::ostream&
operator<<(std::ostream& os, const NamePrefixTableMulticastEntry& entry);

} // namespace nlsr

#endif // NLSR_NAME_PREFIX_TABLE_MULTICAST_ENTRY_HPP
