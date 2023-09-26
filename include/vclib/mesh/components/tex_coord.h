/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_MESH_COMPONENTS_TEX_COORD_H
#define VCL_MESH_COMPONENTS_TEX_COORD_H

#include <vclib/concepts/mesh/components/tex_coord.h>
#include <vclib/space/tex_coord.h>

#include "bases/component.h"

namespace vcl::comp {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

/**
 * @brief The TexCoord class represents a component that stores a texture
 * coordinate.
 *
 * It exposes a vcl::TexCoord object, that stores a texture coordinate and an
 * id of the texture to use.
 *
 * For example, if you have a Vertex Element `v` with the TexCoord component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * auto texcoord = v.texCoord();
 * @endcode
 *
 * @tparam Scalar: The scalar type of the texture coordinate values.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename Scalar, typename ElementType = void, bool OPT = false>
class TexCoord :
		public Component<
			TexCoord<Scalar, ElementType, OPT>,
			TEX_COORD,
			vcl::TexCoord<Scalar>,
			ElementType,
			OPT>
{
	using Base = Component<
		TexCoord<Scalar, ElementType, OPT>,
		TEX_COORD,
		vcl::TexCoord<Scalar>,
		ElementType,
		OPT>;

public:
	/**
	 * @brief Expose the type of the TexCoord.
	 */
	using TexCoordType = vcl::TexCoord<Scalar>;

	const TexCoordType& texCoord() const;
	TexCoordType&       texCoord();

protected:
	// Component interface functions
	template<typename Element>
	void importFrom(const Element& e);
};

/* Detector function to check if a class has TexCoord available */

bool isTexCoordAvailableOn(const ElementConcept auto& element);

/**
 * The TexCoordf class is an alias of the TexCoord component that uses float as
 * scalar type.
 *
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
using TexCoordf = TexCoord<float, ElementType, OPT>;

/**
 * The TexCoordd class is an alias of the TexCoord component that uses double as
 * scalar type.
 *
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
using TexCoordd = TexCoord<double, ElementType, OPT>;

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

/**
 * @brief Returns a const reference of the tex coord of the element.
 * @return a const reference of the tex coord of the element.
 */
template<typename Scalar, typename El, bool O>
auto TexCoord<Scalar, El, O>::texCoord() const -> const TexCoordType&
{
	return Base::data();
}

/**
 * @brief Returns a reference of the tex coord of the element.
 * @return a reference of the tex coord of the element.
 */
template<typename Scalar, typename El, bool O>
auto TexCoord<Scalar, El, O>::texCoord() -> TexCoordType&
{
	return Base::data();
}

template<typename Scalar, typename El, bool O>
template<typename Element>
void TexCoord<Scalar, El, O>::importFrom(const Element& e)
{
	if constexpr(HasTexCoord<Element>) {
		if (isTexCoordAvailableOn(e)){
			texCoord() = e.texCoord().template cast<Scalar>();
		}
	}
}

/**
 * @brief Checks if the given Element has TexCoord component
 * available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that
 * satisfies the ElementConcept.
 * @return `true` if the element has TexCoord component available,
 * `false` otherwise.
 */
bool isTexCoordAvailableOn(const ElementConcept auto& element)
{
	return isComponentAvailableOn<TEX_COORD>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TEXCOORD_H
