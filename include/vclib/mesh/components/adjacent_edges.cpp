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

#include "adjacent_edges.h"

namespace vcl::comp {

/**
 * @brief Returns the number of adjacent edges of this element.
 * @return The number of adjacent edges of this element.
 */
template<typename Edge, int N, typename El, bool h, bool o>
uint AdjacentEdges<Edge, N, El, h, o>::adjEdgesNumber() const
{
	return Base::container(this).size();
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent edge of this element.
 *
 * You can use this function to set the i-th adjacent edge:
 *
 * @code{.cpp}
 * e.adjEdge(2) = &m.edge(k); // the second adj edge of e will point to the k-th edge of the mesh.
 * @endcode
 *
 * @param[in] i: the position of the required adjacent edge in the container; the value must be
 * between 0 and the number of adj edges.
 * @return The pointer to the i-th adjacent edge of this element.
 */
template<typename Edge, int N, typename El, bool h, bool o>
Edge*& AdjacentEdges<Edge, N, El, h, o>::adjEdge(uint i)
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a const pointer to the i-th adjacent edge of this element.
 * @param[in] i: the position of the required adjacent edge in the container; the value must be
 * between 0 and the number of adj edges.
 * @return The pointer to the i-th adjacent edge of this element.
 */
template<typename Edge, int N, typename El, bool h, bool o>
const Edge* AdjacentEdges<Edge, N, El, h, o>::adjEdge(uint i) const
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent edge of this element but using as
 * index the module between i and the number of adjacent edges. You can use this function if you
 * need to get the "next adjacent edge after position k", without check if it is less than the
 * number of adj edges. Works also for negative numbers:
 *
 * @code{.cpp}
 * k = pos; // some position of an adjacent edge
 * auto* next = e.adjEdgeMod(k+1); // the adj edge next to k, that may also be at pos 0
 * auto* last = e.adjEdgeMod(-1); // the adj edge in position adjEdgeNumber()-1
 * @endcode
 *
 * @param[in] i: the position of the required adjacent edge in the container, w.r.t. the position 0;
 * value is modularized on adjEdgeNumber().
 * @return The pointer to the required adjacent edge of this element.
 */
template<typename Edge, int N, typename El, bool h, bool o>
Edge*& AdjacentEdges<Edge, N, El, h, o>::adjEdgeMod(int i)
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Same of adjEdgeMod, but returns a const Pointer to the adjacent edge.
 * @param[in] i: the position of the required adjacent edge in the container, w.r.t. the position 0;
 * value is modularized on adjEdgeNumber().
 * @return The pointer to the required adjacent edge of this element.
 */
template<typename Edge, int N, typename El, bool h, bool o>
const Edge* AdjacentEdges<Edge, N, El, h, o>::adjEdgeMod(int i) const
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Sets the i-th adjacent edge of this element.
 * @param[in] e: The pointer to the adjacent edge to set to this element.
 * @param[in] i: the position in the container on which set the adj edge; the value must be
 * between 0 and the number of adj edges.
 */
template<typename Edge, int N, typename El, bool h, bool o>
void AdjacentEdges<Edge, N, El, h, o>::setAdjEdge(Edge* e, uint i)
{
	Base::container(this).set(e, i);
}

/**
 * @brief Sets all the adjacent edges of this element.
 *
 * If the size of the container is static, the size of the input vector must be the same one of the
 * container.
 *
 * @param[in] list: vector of adjacent edges to set.
 */
template<typename Edge, int N, typename El, bool h, bool o>
void AdjacentEdges<Edge, N, El, h, o>::setAdjEdges(const std::vector<Edge*>& list)
{
	Base::container(this).set(list);
}

template<typename Edge, int N, typename El, bool h, bool o>
bool AdjacentEdges<Edge, N, El, h, o>::containsAdjEdge(const Edge* e) const
{
	return Base::container(this).contains(e);
}

template<typename Edge, int N, typename El, bool h, bool o>
typename AdjacentEdges<Edge, N, El, h, o>::AdjacentEdgeIterator
AdjacentEdges<Edge, N, El, h, o>::findAdjEdge(const Edge* e)
{
	return Base::container(this).find(e);
}

template<typename Edge, int N, typename El, bool h, bool o>
typename AdjacentEdges<Edge, N, El, h, o>::ConstAdjacentEdgeIterator
AdjacentEdges<Edge, N, El, h, o>::findAdjEdge(const Edge* e) const
{
	return Base::container(this).find(e);
}

template<typename Edge, int N, typename El, bool h, bool o>
int AdjacentEdges<Edge, N, El, h, o>::indexOfAdjEdge(const Edge* e) const
{
	return Base::container(this).indexOf(e);
}

/**
 * @brief Resize the container of the adjacent edges to the given size.
 * @note This function is available only if the container of the Adjacent Edges is has dynamic
 * size.
 * @param n
 */
template<typename Edge, int N, typename El, bool h, bool o>
void AdjacentEdges<Edge, N, El, h, o>::resizeAdjEdges(uint n)
	requires(N < 0)
{
	Base::container(this).resize(n);
}

template<typename Edge, int N, typename El, bool h, bool o>
void AdjacentEdges<Edge, N, El, h, o>::pushAdjEdge(Edge* e)
	requires(N < 0)
{
	Base::container(this).pushBack(e);
}

template<typename Edge, int N, typename El, bool h, bool o>
void AdjacentEdges<Edge, N, El, h, o>::insertAdjEdge(uint i, Edge* e)
	requires(N < 0)
{
	Base::container(this).insert(i, e);
}

template<typename Edge, int N, typename El, bool h, bool o>
void AdjacentEdges<Edge, N, El, h, o>::eraseAdjEdge(uint i)
	requires(N < 0)
{
	Base::container(this).erase(i);
}

template<typename Edge, int N, typename El, bool h, bool o>
void AdjacentEdges<Edge, N, El, h, o>::clearAdjEdges()
	requires(N < 0)
{
	Base::container(this).clear();
}

template<typename Edge, int N, typename El, bool h, bool o>
typename AdjacentEdges<Edge, N, El, h, o>::AdjacentEdgeIterator
AdjacentEdges<Edge, N, El, h, o>::adjEdgeBegin()
{
	return Base::container(this).begin();
}

template<typename Edge, int N, typename El, bool h, bool o>
typename AdjacentEdges<Edge, N, El, h, o>::AdjacentEdgeIterator
AdjacentEdges<Edge, N, El, h, o>::adjEdgeEnd()
{
	return Base::container(this).end();
}

template<typename Edge, int N, typename El, bool h, bool o>
typename AdjacentEdges<Edge, N, El, h, o>::ConstAdjacentEdgeIterator
AdjacentEdges<Edge, N, El, h, o>::adjEdgeBegin() const
{
	return Base::container(this).begin();
}

template<typename Edge, int N, typename El, bool h, bool o>
typename AdjacentEdges<Edge, N, El, h, o>::ConstAdjacentEdgeIterator
AdjacentEdges<Edge, N, El, h, o>::adjEdgeEnd() const
{
	return Base::container(this).end();
}

template<typename Edge, int N, typename El, bool h, bool o>
typename AdjacentEdges<Edge, N, El, h, o>::AdjacentEdgeRangeIterator
AdjacentEdges<Edge, N, El, h, o>::adjEdges()
{
	return Base::container(this).rangeIterator();
}

template<typename Edge, int N, typename El, bool h, bool o>
typename AdjacentEdges<Edge, N, El, h, o>::ConstAdjacentEdgeRangeIterator
AdjacentEdges<Edge, N, El, h, o>::adjEdges() const
{
	return Base::container(this).rangeIterator();
}

template<typename Edge, int N, typename El, bool h, bool o>
void AdjacentEdges<Edge, N, El, h, o>::updateEdgeReferences(const Edge* oldBase, const Edge* newBase)
{
	Base::updateElementReferences(oldBase, newBase, this);
}

template<typename Edge, int N, typename El, bool h, bool o>
void AdjacentEdges<Edge, N, El, h, o>::updateEdgeReferencesAfterCompact(
	const Edge*             base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices, this);
}

template<typename Edge, int N, typename El, bool h, bool o>
template<typename Element>
void AdjacentEdges<Edge, N, El, h, o>::importFrom(const Element&)
{
}

template<typename Edge, int N, typename El, bool h, bool o>
template<typename Element, typename ElEType>
void AdjacentEdges<Edge, N, El, h, o>::importEdgeReferencesFrom(
	const Element& e,
	Edge*          base,
	const ElEType* ebase)
{
	if constexpr (HasAdjacentEdges<Element>) {
		if (isAdjacentEdgesEnabledOn(e)) {
			if constexpr (N > 0) {
				// same static size
				if constexpr (N == Element::ADJ_EDGE_NUMBER) {
					importReferencesFrom(e, base, ebase);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::ADJ_EDGE_NUMBER < 0) {
					if (e.adjEdgesNumber() == N) {
						importReferencesFrom(e, base, ebase);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first, then import
				resizeAdjEdges(e.adjEdgesNumber());
				importReferencesFrom(e, base, ebase);
			}
		}
	}
}

template<typename Edge, int N, typename El, bool h, bool o>
template<typename Element, typename ElEType>
void AdjacentEdges<Edge, N, El, h, o>::importReferencesFrom(
	const Element& e,
	Edge*          base,
	const ElEType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.adjEdgesNumber(); ++i) {
			if (e.adjEdge(i) != nullptr) {
				adjEdge(i) = base + (e.adjEdge(i) - ebase);
			}
		}
	}
}

} // namespace vcl::comp
