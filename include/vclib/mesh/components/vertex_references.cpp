/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "vertex_references.h"

namespace vcl::comp {

template<typename Vertex, int N>
VertexReferences<Vertex, N>::VertexReferences() : Base()
{
}

template<typename Vertex, int N>
uint VertexReferences<Vertex, N>::vertexNumber() const
{
	return Base::container.size();
}

template<typename Vertex, int N>
Vertex*& VertexReferences<Vertex, N>::vertex(uint i)
{
	return Base::container.at(i);
}

template<typename Vertex, int N>
const Vertex* VertexReferences<Vertex, N>::vertex(uint i) const
{
	return Base::container.at(i);
}

template<typename Vertex, int N>
Vertex*& VertexReferences<Vertex, N>::vertexMod(int i)
{
	return Base::container.atMod(i);
}

template<typename Vertex, int N>
const Vertex* VertexReferences<Vertex, N>::vertexMod(int i) const
{
	return Base::container.atMod(i);
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::setVertex(Vertex* v, uint i)
{
	Base::container.set(v, i);
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::setVertices(const std::vector<Vertex*>& list)
{
	Base::container.set(list);
}

template<typename Vertex, int N>
bool VertexReferences<Vertex, N>::containsVertex(const Vertex* v) const
{
	return Base::container.contains(v);
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::VertexIterator
VertexReferences<Vertex, N>::findVertex(const Vertex* v)
{
	return Base::container.find(v);
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexIterator
VertexReferences<Vertex, N>::findVertex(const Vertex* v) const
{
	return Base::container.find(v);
}

template<typename Vertex, int N>
int VertexReferences<Vertex, N>::indexOfVertex(const Vertex* v) const
{
	return Base::container.indexOf(v);
}

template<typename Vertex, int N>
int VertexReferences<Vertex, N>::indexOfEdge(const Vertex* v1, const Vertex* v2) const
{
	int vid = indexOfVertex(v1);
	if (vid < 0) {
		return -1;
	}
	else if (vertexMod(vid+1) == v2) {
		return vid;
	}
	else if (vertexMod(vid-1) == v2) {
		uint n = vertexNumber();
		return ((vid-1) % n + n) % n;
	}
	else {
		return -1;
	}
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::resizeVertices(uint n) requires (N < 0)
{
	Base::container.resize(n);
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::pushVertex(Vertex* v) requires (N < 0)
{
	Base::container.pushBack(v);
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::insertVertex(uint i, Vertex* v) requires (N < 0)
{
	Base::container.insert(i, v);
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::eraseVertex(uint i) requires (N < 0)
{
	Base::container.erase(i);
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::clearVertices() requires (N < 0)
{
	Base::container.clear();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::VertexIterator VertexReferences<Vertex, N>::vertexBegin()
{
	return Base::container.begin();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::VertexIterator VertexReferences<Vertex, N>::vertexEnd()
{
	return Base::container.end();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexIterator
VertexReferences<Vertex, N>::vertexBegin() const
{
	return Base::container.begin();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexIterator
VertexReferences<Vertex, N>::vertexEnd() const
{
	return Base::container.end();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::VertexRangeIterator VertexReferences<Vertex, N>::vertices()
{
	return Base::container.rangeIterator();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexRangeIterator
VertexReferences<Vertex, N>::vertices() const
{
	return Base::container.rangeIterator();
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	Base::updateElementReferences(oldBase, newBase);
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices);
}

template<typename Vertex, int N>
template<typename Element>
void VertexReferences<Vertex, N>::importFrom(const Element&)
{
}

template<typename Vertex, int N>
template<typename Element, typename ElVType>
void VertexReferences<Vertex, N>::importVertexReferencesFrom(
	const Element& e,
	Vertex* base,
	const ElVType* ebase)
{
	if constexpr (HasVertexReferences<Element>) {
		if constexpr(N > 0) {
			// same size non-polygonal faces
			if constexpr (N == Element::VERTEX_NUMBER) {
				importReferencesFrom(e, base, ebase);
			}
			// from polygonal to fixed size, but the polygon size == the fixed face size
			else if constexpr (Element::VERTEX_NUMBER < 0){
				if (e.vertexNumber() == N) {
					importReferencesFrom(e, base, ebase);
				}
			}
			else {
				// do not import in this case: cannot import from a face of different size
			}
		}
		else {
			// from fixed to polygonal size: need to resize first, then import
			resizeVertices(e.vertexNumber());
			importReferencesFrom(e, base, ebase);
		}
	}
}

template<typename Vertex, int N>
template<typename Element, typename ElVType>
void VertexReferences<Vertex, N>::importReferencesFrom(
	const Element& e,
	Vertex* base,
	const ElVType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.vertexNumber(); ++i){
			if (e.vertex(i) != nullptr){
				vertex(i) = base + (e.vertex(i) - ebase);
			}
		}
	}
}

} // namespace vcl::comp
