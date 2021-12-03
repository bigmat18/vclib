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

#ifndef VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
#define VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H

#include "optional_info.h"

#include <any>
#include <string>
#include <vector>

#include "../components/detection/custom_components_detection.h"

namespace vcl::comp {

/**
 * @brief The CustomComponents class is a container of custom and additional components associated
 * to an Element (e.g. Vertex, Face).
 *
 */
template<typename T>
class CustomComponents : public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;
	uint thisId() const { return B::index((T*)this); }

public:
	bool hasCustomComponent(const std::string& attrName) const;

	template<typename AttrType>
	const AttrType& customComponent(const std::string& attrName) const;

	template<typename AttrType>
	AttrType& customComponent(const std::string& attrName);

	template <typename Element>
	void importFrom(const Element& e);
};

} // namespace vcl::comp

#include "custom_components.cpp"

#endif // VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
