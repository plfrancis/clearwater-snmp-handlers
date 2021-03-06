/**
* Project Clearwater - IMS in the Cloud
* Copyright (C) 2013 Metaswitch Networks Ltd
*
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or (at your
* option) any later version, along with the "Special Exception" for use of
* the program along with SSL, set forth below. This program is distributed
* in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR
* A PARTICULAR PURPOSE. See the GNU General Public License for more
* details. You should have received a copy of the GNU General Public
* License along with this program. If not, see
* <http://www.gnu.org/licenses/>.
*
* The author can be reached by email at clearwater@metaswitch.com or by
* post at Metaswitch Networks Ltd, 100 Church St, Enfield EN2 6BQ, UK
*
* Special Exception
* Metaswitch Networks Ltd grants you permission to copy, modify,
* propagate, and distribute a work formed by combining OpenSSL with The
* Software, or a work derivative of such a combination, even if such
* copying, modification, propagation, or distribution would otherwise
* violate the terms of the GPL. You must comply with the GPL in all
* respects for all of the code used other than OpenSSL.
* "OpenSSL" means OpenSSL toolkit software distributed by the OpenSSL
* Project and licensed under the OpenSSL Licenses, or a work based on such
* software and licensed under the OpenSSL Licenses.
* "OpenSSL Licenses" means the OpenSSL License and Original SSLeay License
* under which the OpenSSL Project distributes the OpenSSL toolkit software,
* as those licenses appear in the file LICENSE-OPENSSL.
*/

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <cstdlib>
#include "oid.hpp"

// Constructors - can either create from an oid[] or a string

OID::OID(oid* oids_ptr, int len)
{
  int i;
  for (i = 0; i < len; i++)
  {
    _oids.push_back(*(oids_ptr+i));
  }
}

OID::OID(std::string oidstr)
{
  std::vector<std::string> result;
  boost::split(result, oidstr, boost::is_any_of("."));
  for (std::vector<std::string>::iterator it = result.begin() ; it != result.end(); ++it)
  {
    if (!it->empty())   // Ignore an initial dot
    {
      _oids.push_back(atoi(it->c_str()));
    }
  }
}

// Functions to expose the underlying oid* for compatability with the
// C API

const oid* OID::get_ptr() const
{
  return &_oids[0];
}

int OID::get_len() const
{
  return _oids.size();
}

bool OID::equals(OID other_oid)
{
  return (netsnmp_oid_equals(get_ptr(), get_len(),
                             other_oid.get_ptr(), other_oid.get_len()) == 0);
}

bool OID::subtree_contains(OID other_oid)
{
  return (snmp_oidtree_compare(get_ptr(), get_len(),
                               other_oid.get_ptr(), other_oid.get_len()) == 0);
}

// Appends the given OID string to this OID
// e.g. OID("1.2.3.4").append("5.6") is OID("1.2.3.4.5.6")
void OID::append(std::string more)
{
  std::vector<std::string> result;
  boost::split(result, more, boost::is_any_of("."));
  for (std::vector<std::string>::iterator it = result.begin() ; it != result.end(); ++it)
  {
    _oids.push_back(atoi(it->c_str()));
  }
}

// Debugging tool - prints this OID to stderr
void OID::dump() const
{
  for (std::vector<oid>::const_iterator it = _oids.begin() ; it != _oids.end(); ++it)
  {
    std::cerr << "." << *it;
  }
}

