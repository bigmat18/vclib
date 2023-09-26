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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_EDGES_H
#define VCL_MESH_COMPONENTS_ADJACENT_EDGES_H

#include <vclib/concepts/mesh/components/adjacent_edges.h>
#include <vclib/views/view.h>

#include "bases/pointers_container_component.h"

namespace vcl::comp {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

/**
 * @brief The AdjacentEdges class is a container of Edge pointers. It could be
 * used by any Element to save adjacencies information (also the Edge element
 * itself).
 *
 * It is a random access container having static or dynamic size, depending on
 * the value of the template argument N (a negative number means dynamic).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Vertex Element `v` that has the AdjacentEdges
 * component, you'll be able to access to this component member functions from
 * `v`:
 *
 * @code{.cpp}
 * v.adjEdgesNumber();
 * @endcode
 *
 * @note This component could be *Tied To Vertex Number*: it means that the size
 * of the container, if dynamic, will change automatically along the Vertex
 * Number of the Component. Check the `TTVN` template value on the
 * specialization of your component to check if it is tied to the Vertex Number.
 * For further details check the documentation of the @ref ContainerComponent
 * class.
 *
 * @tparam Edge: The type of the adjacent Edge element.
 * @tparam N: The size of the container, that will represent the number of
 * storable adjacent edges. If negative, the container is dynamic.
 * @tparam TTVN: If true, the size of the container will be tied to the Vertex
 * Number of the component (this is used mostly on Face elements).
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<
	typename Edge,
	int  N,
	bool TTVN,
	typename ElementType = void,
	bool OPT             = false>
class AdjacentEdges :
		public PointersContainerComponent<
			AdjacentEdges<Edge, N, TTVN, ElementType, OPT>,
			ADJACENT_EDGES,
			Edge,
			N,
			ElementType,
			OPT,
			TTVN>
{
	using Base = PointersContainerComponent<
		AdjacentEdges<Edge, N, TTVN, ElementType, OPT>,
		ADJACENT_EDGES,
		Edge,
		N,
		ElementType,
		OPT,
		TTVN>;

public:
	/**
	 * @brief Expose the type of the Adjacent Edge.
	 */
	using AdjacentEdgeType = Edge;

	/* Iterator Types declaration */

	using AdjacentEdgeIterator      = Base::Iterator;
	using ConstAdjacentEdgeIterator = Base::ConstIterator;

	/**
	 * @brief Static size of the container. If the container is dynamic, this
	 * value will be negative and you should use the adjEdgesNumber() member
	 * function.
	 */
	static const int ADJ_EDGE_NUMBER = Base::SIZE;

	/* Member functions */

	uint adjEdgesNumber() const;

	Edge*&      adjEdge(uint i);
	const Edge* adjEdge(uint i) const;
	Edge*&      adjEdgeMod(int i);
	const Edge* adjEdgeMod(int i) const;

	void setAdjEdge(Edge* f, uint i);

	template<Range Rng>
	void setAdjEdges(Rng&& r)
		requires RangeOfConvertibleTo<Rng, Edge*>;

	bool containsAdjEdge(const Edge* e) const;

	AdjacentEdgeIterator      findAdjEdge(const Edge* e);
	ConstAdjacentEdgeIterator findAdjEdge(const Edge* e) const;

	uint indexOfAdjEdge(const Edge* e) const;

	/* Member functions specific for vector of adjacent edges */

	void resizeAdjEdges(uint n) requires (N < 0 && !TTVN);
	void pushAdjEdge(Edge* e) requires (N < 0 && !TTVN);
	void insertAdjEdge(uint i, Edge* e) requires (N < 0 && !TTVN);
	void eraseAdjEdge(uint i) requires (N < 0 && !TTVN);
	void clearAdjEdges() requires (N < 0 && !TTVN);

	/* Iterator Member functions */

	AdjacentEdgeIterator            adjEdgeBegin();
	AdjacentEdgeIterator            adjEdgeEnd();
	ConstAdjacentEdgeIterator       adjEdgeBegin() const;
	ConstAdjacentEdgeIterator       adjEdgeEnd() const;
	View<AdjacentEdgeIterator>      adjEdges();
	View<ConstAdjacentEdgeIterator> adjEdges() const;

protected:
	// Component interface function
	template <typename Element>
	void importFrom(const Element& e);

	// PointersComponent interface functions
	template<typename Element, typename ElEType>
	void importPointersFrom(const Element& e, Edge* base, const ElEType* ebase);

	// ContainerComponent interface functions
	void resize(uint n) requires (N < 0);
	void pushBack(Edge* e = nullptr) requires (N < 0);
	void insert(uint i, Edge* e = nullptr) requires (N < 0);
	void erase(uint i) requires (N < 0);
	void clear() requires (N < 0);

private:
	template<typename Element, typename ElEType>
	void importPtrsFrom(const Element& e, Edge* base, const ElEType* ebase);
};

/* Detector function to check if a class has AdjacentEdges available */

bool isAdjacentEdgesAvailableOn(const ElementConcept auto& element);

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

/**
 * @brief Returns the number of adjacent edges of this element.
 * @return The number of adjacent edges of this element.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
uint AdjacentEdges<Edge, N, TT, El, O>::adjEdgesNumber() const
{
	return Base::container().size();
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent edge of the
 * element.
 *
 * You can use this function to set the i-th adjacent edge:
 *
 * @code{.cpp}
 * e.adjEdge(2) = &m.edge(k); // the second adj edge of e will point to the k-th
 *                            // edge of the mesh.
 * @endcode
 *
 * @param[in] i: the position of the required adjacent edge in the container;
 * the value must be between 0 and the number of adj edges.
 * @return The pointer to the i-th adjacent edge of the element.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
Edge*& AdjacentEdges<Edge, N, TT, El, O>::adjEdge(uint i)
{
	return Base::container().at(i);
}

/**
 * @brief Returns a const pointer to the i-th adjacent edge of the element.
 * @param[in] i: the position of the required adjacent edge in the container;
 * the value must be between 0 and the number of adj edges.
 * @return The pointer to the i-th adjacent edge of the element.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
const Edge* AdjacentEdges<Edge, N, TT, El, O>::adjEdge(uint i) const
{
	return Base::container().at(i);
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent edge of the
 * element but using as index the module between i and the number of adjacent
 * edges. You can use this function if you need to get the "next adjacent edge
 * after position k", without check if it is less than the number of adj edges.
 * Works also for negative numbers:
 *
 * @code{.cpp}
 * k = pos; // some position of an adjacent edge
 * auto* next = e.adjEdgeMod(k+1); // the adj edge next to k, that may also be
 *                                 // at pos 0
 * auto* last = e.adjEdgeMod(-1); // the adj edge in position
 *                                // adjEdgeNumber()-1
 * @endcode
 *
 * @param[in] i: the position of the required adjacent edge in the container,
 * w.r.t. the position 0; value is modularized on adjEdgeNumber().
 * @return The pointer to the required adjacent edge of the element.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
Edge*& AdjacentEdges<Edge, N, TT, El, O>::adjEdgeMod(int i)
{
	return Base::container().atMod(i);
}

/**
 * @brief Same of adjEdgeMod, but returns a const Pointer to the adjacent edge.
 * @param[in] i: the position of the required adjacent edge in the container,
 * w.r.t. the position 0; value is modularized on adjEdgeNumber().
 * @return The pointer to the required adjacent edge of this element.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
const Edge* AdjacentEdges<Edge, N, TT, El, O>::adjEdgeMod(int i) const
{
	return Base::container().atMod(i);
}

/**
 * @brief Sets the i-th adjacent edge of the element.
 * @param[in] e: The pointer to the adjacent edge to set to the element.
 * @param[in] i: the position in the container on which set the adj edge; the
 * value must be between 0 and the number of adj edges.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::setAdjEdge(Edge* e, uint i)
{
	Base::container().set(e, i);
}

/**
 * @brief Sets all the adjacent edges of the element.
 *
 * If the size of the container is static, the size of the input range must be
 * the same one of the container.
 *
 * @tparam Rng: The type of the range of adjacent edges to set. The value type
 * of the range must be convertible to a pointer to an AdjacentEdge.
 *
 * @param[in] r: range of adjacent edges to set.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
template<Range Rng>
void AdjacentEdges<Edge, N, TT, El, O>::setAdjEdges(Rng&& r)
	requires RangeOfConvertibleTo<Rng, Edge*>
{
	Base::container().set(r);
}

/**
 * @brief Returns `true` if the container of adjacent edges contains the given
 * edge, `false` otherwise.
 *
 * @param[in] e: the pointer to the edge to search.
 * @return `true` if the container of adjacent edges contains the given edge,
 * `false` otherwise.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
bool AdjacentEdges<Edge, N, TT, El, O>::containsAdjEdge(const Edge* e) const
{
	return Base::container().contains(e);
}

/**
 * @brief Returns an iterator to the first adjacent edge in the container of
 * this component that is equal to the given edge. If no such adjacent edge is
 * found, past-the-end iterator is returned.
 *
 * @param[in] e: the pointer to the edge to search.
 * @return an iterator pointing to the first adjacent edge equal to the given
 * edge, or end if no such adjacent edge is found.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
auto AdjacentEdges<Edge, N, TT, El, O>::findAdjEdge(const Edge* e)
	-> AdjacentEdgeIterator
{
	return Base::container().find(e);
}

/**
 * @brief Returns a const iterator to the first adjacent edge in the container
 * of this component that is equal to the given edge. If no such adjacent edge
 * is found, past-the-end iterator is returned.
 *
 * @param[in] e: the pointer to the edge to search.
 * @return a const iterator pointing to the first adjacent edge equal to the
 * given edge, or end if no such adjacent edge is found.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
auto AdjacentEdges<Edge, N, TT, El, O>::findAdjEdge(const Edge* e) const
	-> ConstAdjacentEdgeIterator
{
	return Base::container().find(e);
}

/**
 * @brief Returns the index of the given adjacent edge in the container of the
 * element. If the given adjacent edge is not in the container, returns
 * UINT_NULL.
 *
 * @param[in] e: the pointer to the adjacent edge to search.
 * @return the index of the given adjacent edge, or UINT_NULL if it is not
 * found.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
uint AdjacentEdges<Edge, N, TT, El, O>::indexOfAdjEdge(const Edge* e) const
{
	return Base::container().indexOf(e);
}

/**
 * @brief Resize the container of the adjacent edges to the given size.
 * @note This function is available only if the container of the Adjacent Edges
 * component has dynamic size.
 * @param[in] n: The new size of the adjacent edges container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::resizeAdjEdges(uint n)
	requires(N < 0 && !TT)
{
	Base::container().resize(n);
}

/**
 * @brief Pushes in the back of the container the given adjacent edge.
 * @note This function is available only if the container of the Adjacent Edges
 * component has dynamic size.
 * @param[in] e: The pointer to the adjacent edge to push in the back of the
 * container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::pushAdjEdge(Edge* e)
	requires(N < 0 && !TT)
{
	Base::container().pushBack(e);
}

/**
 * @brief Inserts the given adjacent edge in the container at the given
 * position.
 * @note This function is available only if the container of the Adjacent Edges
 * component has dynamic size.
 * @param[in] i: The position in the container where to insert the adjacent
 * edge.
 * @param[in] e: The pointer to the adjacent edge to insert in the container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::insertAdjEdge(uint i, Edge* e)
	requires(N < 0 && !TT)
{
	Base::container().insert(i, e);
}

/**
 * @brief Removes the adjacent edge at the given position from the container.
 * @note This function is available only if the container of the Adjacent Edges
 * component has dynamic size.
 * @param[in] i: The position of the adjacent edge to remove from the
 * container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::eraseAdjEdge(uint i)
	requires(N < 0 && !TT)
{
	Base::container().erase(i);
}

/**
 * @brief Clears the container of adjacent edges, making it empty.
 * @note This function is available only if the container of the Adjacent Edges
 * component has dynamic size.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::clearAdjEdges()
	requires(N < 0 && !TT)
{
	Base::container().clear();
}

/**
 * @brief Returns an iterator to the first adjacent edge in the container of
 * this component.
 *
 * @return an iterator pointing to the begin of this container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
auto AdjacentEdges<Edge, N, TT, El, O>::adjEdgeBegin() -> AdjacentEdgeIterator
{
	return Base::container().begin();
}

/**
 * @brief Returns an iterator to the end of the container of this component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
auto AdjacentEdges<Edge, N, TT, El, O>::adjEdgeEnd() -> AdjacentEdgeIterator
{
	return Base::container().end();
}

/**
 * @brief Returns a const iterator to the first adjacent edge in the container
 * of this component.
 *
 * @return an iterator pointing to the begin of this container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
auto AdjacentEdges<Edge, N, TT, El, O>::adjEdgeBegin() const
	-> ConstAdjacentEdgeIterator
{
	return Base::container().begin();
}

/**
 * @brief Returns a const iterator to the end of the container of this
 * component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
auto AdjacentEdges<Edge, N, TT, El, O>::adjEdgeEnd() const
	-> ConstAdjacentEdgeIterator
{
	return Base::container().end();
}

/**
 * @brief Returns a lightweight view object that stores the begin and end
 * iterators of the container of adjacent edges of the element. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (auto* adjEdge : el.adjEdges()) {
 *     // Do something with adjEdge
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over adjacent edges.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
auto AdjacentEdges<Edge, N, TT, El, O>::adjEdges() -> View<AdjacentEdgeIterator>
{
	return View(adjEdgeBegin(), adjEdgeEnd());
}

/**
 * @brief Returns a lightweight const view object that stores the begin and end
 * iterators of the container of adjacent edges of the element. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (const auto* adjEdge : el.adjEdges()) {
 *     // Do something read-only with adjEdge
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over adjacent edges.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
auto AdjacentEdges<Edge, N, TT, El, O>::adjEdges() const
	-> View<ConstAdjacentEdgeIterator>
{
	return View(adjEdgeBegin(), adjEdgeEnd());
}

template<typename Edge, int N, bool TT, typename El, bool O>
template<typename Element>
void AdjacentEdges<Edge, N, TT, El, O>::importFrom(const Element&)
{
}

template<typename Edge, int N, bool TT, typename El, bool O>
template<typename Element, typename ElEType>
void AdjacentEdges<Edge, N, TT, El, O>::importPointersFrom(
	const Element& e,
	Edge*          base,
	const ElEType* ebase)
{
	if constexpr (HasAdjacentEdges<Element>) {
		if (isAdjacentEdgesAvailableOn(e)) {
			if constexpr (N > 0) {
				// same static size
				if constexpr (N == Element::ADJ_EDGE_NUMBER) {
					importPtrsFrom(e, base, ebase);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::ADJ_EDGE_NUMBER < 0) {
					if (e.adjEdgesNumber() == N) {
						importPtrsFrom(e, base, ebase);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic
					// size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first,
				// then import
				resize(e.adjEdgesNumber());
				importPtrsFrom(e, base, ebase);
			}
		}
	}
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::resize(uint n) requires (N < 0)
{
	Base::container().resize(n);
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::pushBack(Edge* e) requires (N < 0)
{
	Base::container().pushBack(e);
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::insert(uint i, Edge* e) requires (N < 0)
{
	Base::container().insert(i, e);
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::erase(uint i) requires (N < 0)
{
	Base::container().erase(i);
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::clear() requires (N < 0)
{
	Base::container().clear();
}

template<typename Edge, int N, bool TT, typename El, bool O>
template<typename Element, typename ElEType>
void AdjacentEdges<Edge, N, TT, El, O>::importPtrsFrom(
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

/**
 * @brief Checks if the given Element has AdjacentEdges available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that satisfies
 * the ElementConcept.
 * @return `true` if the element has AdjacentEdges available, `false` otherwise.
 */
bool isAdjacentEdgesAvailableOn(const ElementConcept auto& element)
{
	return isComponentAvailableOn<ADJACENT_EDGES>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_EDGES_H
