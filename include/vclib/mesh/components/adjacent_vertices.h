/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include "element_references.h"

namespace vcl::comp {

class AdjacentVerticesTriggerer
{
};

template<typename Vertex, int N>
class AdjacentVertices : protected ElementReferences<Vertex, N>, public AdjacentVerticesTriggerer
{
	using Base = ElementReferences<Vertex, N>;

public:
	static const int ADJ_VERTEX_NUMBER = Base::CONTAINER_SIZE;

	/** Iterator Types declaration **/

	using AdjacentVertexIterator           = typename Base::GCIterator;
	using ConstAdjacentVertexIterator      = typename Base::ConstGCIterator;
	using AdjacentVertexRangeIterator      = typename Base::GCRangeIterator;
	using ConstAdjacentVertexRangeIterator = typename Base::ConstGCRangeIterator;

	/** Constructor **/

	AdjacentVertices() : Base() {}

	/** Member functions **/

	unsigned int adjVerticesNumber() const { return Base::size(); }

	Vertex*&      adjVertex(unsigned int i) { return Base::at(i); }
	const Vertex* adjVertex(unsigned int i) const { return Base::at(i); }
	Vertex*&      adjVertexMod(int i) { return Base::atMod(i); }
	const Vertex* adjVertexMod(int i) const { return Base::atMod(i); }

	void setAdjVertex(Vertex* v, unsigned int i) { Base::set(v, i); }
	void setAdjVertices(const std::vector<Vertex*>& list) { Base::set(list); }

	bool containsAdjVertex(const Vertex* v) const { return Base::contains(v); }

	AdjacentVertexIterator findAdjVertex(const Vertex* v) { return Base::find(v); }
	ConstAdjacentVertexIterator findAdjVertex(const Vertex* v) const { return Base::find(v); }

	int indexOfAdjVertex(const Vertex* v) const { return Base::indexOf(v); }

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeAdjVertices(unsigned int n)
	{
		Base::resize(n);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushAdjVertex(Vertex* v)
	{
		Base::pushBack(v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertAdjVertex(unsigned int i, Vertex* v)
	{
		Base::insert(i, v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseAdjVertex(unsigned int i)
	{
		Base::erase(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearAdjVertices()
	{
		Base::clear();
	}

	/** Iterator Member functions **/

	AdjacentVertexIterator           adjVertexBegin() { return Base::begin(); }
	AdjacentVertexIterator           adjVertexEnd() { return Base::end(); }
	ConstAdjacentVertexIterator      adjVertexBegin() const { return Base::begin(); }
	ConstAdjacentVertexIterator      adjVertexEnd() const { return Base::end(); }
	AdjacentVertexRangeIterator      adjVertices() { return Base::rangeIterator(); }
	ConstAdjacentVertexRangeIterator adjVertices() const { return Base::rangeIterator(); }

protected:
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
	{
		Base::updateElementReferences(oldBase, newBase);
	}

	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices)
	{
		Base::updateElementReferencesAfterCompact(base, newIndices);
	}
};

template<typename T>
using hasAdjacentVerticesT = std::is_base_of<AdjacentVerticesTriggerer, T>;

template<typename T>
bool constexpr hasAdjacentVertices()
{
	return hasAdjacentVerticesT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H