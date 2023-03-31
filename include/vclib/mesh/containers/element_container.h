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

#ifndef VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
#define VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H

#include <vector>

#include <vclib/concept/mesh/component/custom_components.h>
#include <vclib/concept/mesh/container.h>
#include <vclib/concept/mesh/element/element.h>
#include <vclib/iterators/view.h>
#include <vclib/mesh/iterators/element_container_iterator.h>

#include "custom_components_vector_map.h"
#include "vertical_components_vector_tuple.h"

namespace vcl::mesh {

template <ElementConcept T>
class ElementContainer : public ElementContainerTriggerer
{
	template <ElementConcept U>
	friend class ElementContainer;

	using ElementContainerType = ElementContainer<T>;

public:
	using ElementType          = T;

	static const uint ELEMENT_TYPE = T::ELEMENT_TYPE;

	ElementContainer();

protected:
	using ElementIterator      = ElementContainerIterator<std::vector, T>;
	using ConstElementIterator = ConstElementContainerIterator<std::vector, T>;
	using ElementView          = vcl::View<ElementIterator>;
	using ConstElementView     = vcl::View<ConstElementIterator>;

	const T& element(uint i) const;
	T& element(uint i);

	uint elementNumber() const;
	uint elementContainerSize() const;
	uint deletedElementNumber() const;

	void deleteElement(uint i);
	void deleteElement(const T* e);

	uint elementIndexIfCompact(uint i) const;
	std::vector<int> elementCompactIndices() const;

	template<typename MeshType>
	void setParentMeshPointers(MeshType* parentMesh);

	template<typename C>
	bool isOptionalComponentEnabled() const;

	template<typename C>
	void enableOptionalComponent();

	template<typename C>
	void disableOptionalComponent();

	ElementIterator      elementBegin(bool jumpDeleted = true);
	ElementIterator      elementEnd();
	ConstElementIterator elementBegin(bool jumpDeleted = true) const;
	ConstElementIterator elementEnd() const;
	ElementView          elements(bool jumpDeleted = true);
	ConstElementView     elements(bool jumpDeleted = true) const;

	uint index(const T *e) const;
	void clearElements();

	template<typename MeshType>
	uint addElement(MeshType* parentMesh);

	template<typename MeshType>
	uint addElements(uint size, MeshType* parentMesh);

	template<typename MeshType>
	void reserveElements(uint size, MeshType* parentMesh);

	template<typename MeshType>
	void resizeElements(uint size, MeshType* parentMesh);

	std::vector<int> compactElements();

	template<typename Element>
	void updateReferences(const Element* oldBase, const Element* newBase);

	template<typename Element>
	void updateReferencesAfterCompact(const Element* base, const std::vector<int>& newIndices);

	template<typename OtherMesh>
	void enableOptionalComponentsOf(const OtherMesh& m);

	template<typename OtherMesh, typename ParentMeshType>
	void importFrom(const OtherMesh& m, ParentMeshType* parent);

	template<typename OtherMesh, typename ElRefBase>
	void importReferencesFrom(const OtherMesh& othMesh, ElRefBase* base);
	
	// filter components of elements, taking only vertical ones
	using vComps = typename vcl::FilterTypesByCondition<comp::IsVerticalComponentPred, typename T::Components>::type;
	
	/**
	 * @brief en: the number of elements in the container. Could be different from elements.size()
	 * due to elements marked as deleted into the container.
	 */
	uint en = 0;

	/**
	 * @brief vec: the vector of elements: will contain the set of elements, each one of these will
	 * contain the data of the horizontal components and a pointer to the parent mesh
	 */
	std::vector<T> vec;

	/**
	 * @brief vcVecTuple the tuple of vectors of all the vertical components of the element. Contains
	 * both the optional and the persistent vertical components
	 */
	VerticalComponentsVectorTuple<vComps> vcVecTuple;

	/**
	 * @brief ccVecMap the map that associates a string to a vector of custom components
	 */
	CustomComponentsVectorMap<T, comp::HasCustomComponents<T>> ccVecMap;

private:
	template<typename ElRef, typename... Comps>
	void updateReferencesOnComponents(
		const ElRef* oldBase,
		const ElRef* newBase,
		TypeWrapper<Comps...>);

	template<typename ElRef, typename... Comps>
	void updateReferencesAfterCompactOnComponents(
		const ElRef*            base,
		const std::vector<int>& newIndices,
		TypeWrapper<Comps...>);

	template<typename Container, typename MyBase, typename CBase>
	void importReferencesFromContainer(const Container& c, MyBase* base, const CBase* cbase);

	template<typename Container, typename ElRef, typename CBase, typename... Comps>
	void importReferencesOnComponentsFrom(
		const Container& c,
		ElRef*           base,
		const CBase*     cbase,
		TypeWrapper<Comps...>);

	template<typename Comp, typename ElRef>
	void updateReferencesOnComponent(const ElRef* oldBase, const ElRef* newBase);

	template<typename Comp, typename ElRef>
	void updateReferencesAfterCompactOnComponent(
		const ElRef* base,
		const std::vector<int>& newIndices);

	template<typename Comp, typename Container, typename ElRef, typename CBase>
	void importReferencesOnComponentFrom(
		const Container& c,
		ElRef*           base,
		const CBase*     cbase);
};

} // namespace vcl::mesh

#include "element_container.cpp"

#endif // VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
