/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_INFO_H
#define MGP_MESH_COMMON_INFO_H

#include <mgp/misc/type_traits.h>

namespace mgp::common {

class Info
{
public:
	Info() : _id(0) {}

	unsigned int id() const { return _id; }

protected:
	unsigned int _id;
};

} // namespace mgp::common

#endif // MGP_MESH_COMMON_INFO_H
