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

#ifndef VCL_CONCEPTS_MESH_CONTAINERS_ELEMENT_CONTAINER_H
#define VCL_CONCEPTS_MESH_CONTAINERS_ELEMENT_CONTAINER_H

#include <vclib/concepts/mesh/elements/element.h>

namespace vcl {
namespace mesh {

class ElementContainerTriggerer
{
};

/**
 * @brief ElementContainerConcept is a concept satisfied when the type T is an Element Container.
 */
template<typename T>
concept ElementContainerConcept =
	std::is_base_of<ElementContainerTriggerer, T>::value;

/**
 * @brief The predicate IsElementContainerPred sets its bool `value` to `true` when the type T
 * satisfies the ElementContainerConcept concept
 */
template<typename T>
struct IsElementContainerPred
{
	static const bool value = ElementContainerConcept<T>;
};

/**
 * @brief Given the ElementEnumType of an Element and a list of ElementContainers, this predicate
 * sets its bool `value` to `true` if there exists an ElementContainer in the list that contains
 * Elements having the ID El, and sets `type` to the TypeWrapper of the found container.
 *
 * If no Container was found, value will be set to `false` and type will contain an empty
 * TypeWrapper.
 */
template<uint El, typename ... Containers>
struct ContainerOfElementPred
{
private:
	template <typename Cont>
	struct SameElPred
	{
		static constexpr bool value = Cont::ELEMENT_TYPE == El;
	};

public:
	// TypeWrapper of the found container, if any
	using type = typename vcl::FilterTypesByCondition<SameElPred, Containers...>::type;
	static constexpr bool value = NumberOfTypes<type>::value == 1;
};

template<uint El, typename ... Containers>
struct ContainerOfElementPred<El, TypeWrapper<Containers...>> :
		public ContainerOfElementPred<El, Containers...>
{
};

} // namespace vcl::mesh
} // namespace vcl

#endif // VCL_CONCEPTS_MESH_CONTAINERS_ELEMENT_CONTAINER_H
