/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_TEX_COORD_H
#define VCL_MESH_COMPONENTS_OPTIONAL_TEX_COORD_H

#include <vclib/space/tex_coord.h>

#include "optional_info.h"
#include "../components/detection/tex_coord_detection.h"

namespace vcl::comp {

template<typename Scalar, typename T>
class OptionalTexCoord : public OptionalTexCoordTriggerer, public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

public:
	using TexCoordType = vcl::TexCoord<Scalar>;

	const TexCoordType& texCoord() const { return B::optCont().texCoord(thisId()); }
	TexCoordType&       texCoord() { return B::optCont().texCoord(thisId()); }

private:
	uint thisId() const { return B::index((T*)this); }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_OPTIONAL_TEX_COORD_H
