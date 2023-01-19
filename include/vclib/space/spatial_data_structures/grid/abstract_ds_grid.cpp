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

#include "abstract_ds_grid.h"

namespace vcl {

template<typename GridType, typename ValueType, typename DerivedGrid>
bool AbstractDSGrid<GridType, ValueType, DerivedGrid>::cellEmpty(const KeyType& k) const
{
	auto p = static_cast<const DerivedGrid*>(this)->valuesInCell(k);
	return p.first == p.second;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
std::size_t AbstractDSGrid<GridType, ValueType, DerivedGrid>::countInCell(const KeyType& k) const
{
	auto p = static_cast<const DerivedGrid*>(this)->valuesInCell(k);
	return std::distance(p.first, p.second);
}

template<typename GridType, typename ValueType, typename DerivedGrid>
uint AbstractDSGrid<GridType, ValueType, DerivedGrid>::countInSphere(
	const Sphere<typename GridType::ScalarType>& s) const
{
	return valuesInSphere(s).size();
}

template<typename GridType, typename ValueType, typename DerivedGrid>
auto AbstractDSGrid<GridType, ValueType, DerivedGrid>::valuesInSphere(
	const Sphere<typename GridType::ScalarType>& s)
{
	std::vector<typename DerivedGrid::Iterator> resVec;

	// interval of cells containing the sphere
	KeyType first = GridType::cell(s.center() - s.radius());
	KeyType last = GridType::cell(s.center() + s.radius());

	unMarkAll();

	for (const KeyType& c : GridType::cells(first, last)) { // for each cell in the intervall
		// p is a pair of iterators
		const auto& p = static_cast<DerivedGrid*>(this)->valuesInCell(c);
		for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
			if (valueIsInSpehere(it, s))
				resVec.push_back(it);
		}
	}
	return resVec;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
auto AbstractDSGrid<GridType, ValueType, DerivedGrid>::valuesInSphere(
	const Sphere<typename GridType::ScalarType>& s) const
{
	std::vector<typename DerivedGrid::ConstIterator> resVec;

	// interval of cells containing the sphere
	KeyType first = GridType::cell(s.center() - s.radius());
	KeyType last = GridType::cell(s.center() + s.radius());

	unMarkAll();

	for (const KeyType& c : GridType::cells(first, last)) { // for each cell in the intervall
		// p is a pair of iterators
		const auto& p = static_cast<const DerivedGrid*>(this)->valuesInCell(c);
		for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
			if (valueIsInSpehere(it, s))
				resVec.push_back(it);
		}
	}
	return resVec;
}

/**
 * @brief Inserts the given element in the AbstractDSGrid.
 *
 * If the ValueType is Puntual (a Point or a Vertex), the element will be inserted in just one
 * cell of the grid. If the element is a spatial object having a bounding box with min != max, the
 * element will be stored in all the cells where its bounding box lies.
 *
 * @param v
 * @return
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
bool AbstractDSGrid<GridType, ValueType, DerivedGrid>::insert(const ValueType& v)
{
	auto vv = getCleanValueTypePointer(v);

	if (vv) { // if vv is a valid pointer (ValueType, or ValueType* if ValueType is not a pointer)
		if constexpr (PointConcept<VT>) { // if the ValueType was a Point (or Point*)
			typename GridType::CellCoord cell = GridType::cell(*vv);
			return static_cast<DerivedGrid*>(this)->insertInCell(cell, v);
		}
		else if constexpr (VertexConcept<VT>) { // if the ValueType was a Vertex (or Vertex*)
			typename GridType::CellCoord cell = GridType::cell(vv->coord());
			return static_cast<DerivedGrid*>(this)->insertInCell(cell, v);
		}
		else { // else, call the boundingBox function
			//return insert(v, intersects);
			typename GridType::BBoxType bb = vcl::boundingBox(*vv);

			typename GridType::CellCoord bmin = GridType::cell(bb.min);
			typename GridType::CellCoord bmax = GridType::cell(bb.max);

			bool ins = false;
			for (const auto& cell : GridType::cells(bmin, bmax)) {
				ins |= static_cast<DerivedGrid*>(this)->insertInCell(cell, v);
			}
			return ins;
		}
	}
	return false;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
bool AbstractDSGrid<GridType, ValueType, DerivedGrid>::insert(
	const ValueType&        v,
	const IsInCellFunction& intersects)
{
	auto vv = getCleanValueTypePointer(v);

	if (vv) { // if vv is a valid pointer (ValueType, or ValueType* if ValueType is not a pointer)
		if constexpr (PointConcept<VT>) { // if the ValueType was a Point (or Point*)
			typename GridType::CellCoord cell = GridType::cell(*vv);
			if (intersects(GridType::cellBox(cell), v)) {
				return static_cast<DerivedGrid*>(this)->insertInCell(cell, v);
			}
		}
		else if constexpr (VertexConcept<VT>) { // if the ValueType was a Vertex (or Vertex*)
			typename GridType::CellCoord cell = GridType::cell(vv->coord());
			if (intersects(GridType::cellBox(cell), v)) {
				return static_cast<DerivedGrid*>(this)->insertInCell(cell, v);
			}
		}
		else { // else, call the boundingBox function
			typename GridType::BBoxType bb = vcl::boundingBox(*vv);

			typename GridType::CellCoord bmin = GridType::cell(bb.min);
			typename GridType::CellCoord bmax = GridType::cell(bb.max);

			bool ins = false;
			for (const auto& cell : GridType::cells(bmin, bmax)) {
				if (intersects(GridType::cellBox(cell), v)) {
					ins |= static_cast<DerivedGrid*>(this)->insertInCell(cell, v);
				}
			}
			return ins;
		}
	}
	return false;
}

/**
 * @brief Inserts all the elements from `begin` to `end`. The type referenced by the iterator must
 * be the ValueType of the AbstractDSGrid.
 * @param begin
 * @param end
 * @return The number of inserted elements.
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename ObjIterator>
uint AbstractDSGrid<GridType, ValueType, DerivedGrid>::insert(ObjIterator begin, ObjIterator end)
{
	uint cnt = 0;
	for (ObjIterator it = begin; it != end; ++it)
		if (insert(*it))
			cnt++;
	return cnt;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename ObjIterator>
uint AbstractDSGrid<GridType, ValueType, DerivedGrid>::insert(
	ObjIterator             begin,
	ObjIterator             end,
	const IsInCellFunction& intersects)
{
	uint cnt = 0;
	for (ObjIterator it = begin; it != end; ++it)
		if (insert(*it, intersects))
			cnt++;
	return cnt;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
bool AbstractDSGrid<GridType, ValueType, DerivedGrid>::erase(const ValueType& v)
{
	auto vv = getCleanValueTypePointer(v);

	if (vv) {
		if constexpr (PointConcept<VT>) {
			typename GridType::CellCoord cell = GridType::cell(*vv);
			return static_cast<DerivedGrid*>(this)->eraseInCell(cell, v);
		}
		else if constexpr (VertexConcept<VT>) {
			typename GridType::CellCoord cell = GridType::cell(vv->coord());
			return static_cast<DerivedGrid*>(this)->eraseInCell(cell, v);
		}
		else {
			typename GridType::BBoxType bb = vcl::boundingBox(*vv);

			typename GridType::CellCoord bmin = GridType::cell(bb.min);
			typename GridType::CellCoord bmax = GridType::cell(bb.max);

			bool found = false;
			for (const auto& cell : GridType::cells(bmin, bmax)) {
				found |= static_cast<DerivedGrid*>(this)->eraseInCell(cell, v);
			}
			return found;
		}
	}
	return false;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
bool AbstractDSGrid<GridType, ValueType, DerivedGrid>::eraseAllInCell(const KeyType& k)
{
	bool res = false;
	auto& p = static_cast<DerivedGrid*>(this)->valuesInCell(k);
	for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
		res |= static_cast<DerivedGrid*>(this)->eraseInCell(k, it->second);
	}
	return res;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
void AbstractDSGrid<GridType, ValueType, DerivedGrid>::eraseInSphere(
	const Sphere<typename GridType::ScalarType>& s)
{
	// vector of iterators
	auto toDel = valuesInSphere(s);
	for (auto& it : toDel)
		eraseInCell(it->first, it->second);
}

/**
 * @brief Empty constructor, creates an usable AbstractDSGrid, since the Grid is not initialized.
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
AbstractDSGrid<GridType, ValueType, DerivedGrid>::AbstractDSGrid()
{
}

/**
 * @brief Creates a AbstractDSGrid that allows to store ValueType values on the given grid.
 * @param grid
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
AbstractDSGrid<GridType, ValueType, DerivedGrid>::AbstractDSGrid(const GridType& grid) :
		GridType(grid)
{
}

/**
 * @brief Creates a AbstractDSGrid that allows to store ValueType values on a Grid having `min` as
 * minimum coordinte of the Grid, `max` as maximum coordinate of the grid, and the number of cells
 * per dimension given by `sizes`.
 * @param min
 * @param max
 * @param sizes
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<PointConcept PointType>
AbstractDSGrid<GridType, ValueType, DerivedGrid>::AbstractDSGrid(
	const PointType& min,
	const PointType& max,
	const KeyType&   sizes) :
		GridType(min, max, sizes)
{
}

/**
 * @brief Creates a AbstractDSGrid that allows to store ValueType values on a Grid bounded by
 * `bbox`, and having the number of cells per dimension given by `sizes`.
 * @param bbox
 * @param sizes
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename BoxType>
AbstractDSGrid<GridType, ValueType, DerivedGrid>::AbstractDSGrid(
	const BoxType& bbox,
	const KeyType& sizes) :
		GridType(bbox, sizes)
{
}

/**
 * @brief Creates an AbstractDSGrid having a proper Grid to store the elements.
 *
 * The bounding box and the sizes of the Grid are automatically computed.
 * Bounding box is computed starting from the bounding box of all the iterated elements, and then
 * inflated. The number of cells per dimension is computed using the `vcl::bestGridSize` function.
 *
 * @note This constructor **does not insert the elements** in the Grid Data Structure.
 * Pure virtual classes cannot call pure virtual member functions from constuctors. It is duty of
 * each derived class to insert the elements after calling this constructor.
 *
 * @param begin
 * @param end
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename ObjIterator>
AbstractDSGrid<GridType, ValueType, DerivedGrid>::AbstractDSGrid(ObjIterator begin, ObjIterator end)
{
	using ScalarType = typename GridType::ScalarType;
	using BBoxType = typename GridType::BBoxType;
	using CellCoord = typename GridType::CellCoord;

	BBoxType bbox = boundingBox(begin, end);
	uint nElements = std::distance(begin, end);

	if (nElements > 0) {
		// inflated bb
		ScalarType infl = bbox.diagonal()/nElements;
		bbox.min -= infl;
		bbox.max += infl;

		CellCoord sizes = bestGridSize(bbox.size(), nElements);

		GridType::set(bbox, sizes);
	}
}

template<typename GridType, typename ValueType, typename DerivedGrid>
bool AbstractDSGrid<GridType, ValueType, DerivedGrid>::isMarked(
	const vcl::Markable<ValueType>& v) const
{
	return v.mark() == m;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
void AbstractDSGrid<GridType, ValueType, DerivedGrid>::mark(
	const vcl::Markable<ValueType>& v) const
{
	v.mark() = m;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
void AbstractDSGrid<GridType, ValueType, DerivedGrid>::unMarkAll() const
{
	m++;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename Iterator>
bool AbstractDSGrid<GridType, ValueType, DerivedGrid>::valueIsInSpehere(
	Iterator it,
	const Sphere<typename GridType::ScalarType>& s) const
{
	auto vv = getCleanValueTypePointer(it->second);

	bool test = false;
	if constexpr(PointConcept<VT>) { // check if the point value is inside the sphere
		test = vv && s.isInside(*vv);
	}
	else if constexpr(VertexConcept<VT>) { // check if the vertex coord is inside the sphere
		test = vv && s.isInside(vv->coord());
	}
	else { // check if the bbox of the value intersects the sphere
		if (!isMarked(it.markableValue())) {
			test = vv && s.intersects(vcl::boundingBox(*vv));
			mark(it.markableValue());
		}
	}
	return test;
}

/**
 * @brief This function returns a clean pointer (VT*) to the given valuetype.
 * It is just an utility function used to avoid code duplication. It allows equal treatment of
 * ValueType variable, whether that v is a pointer or not.
 *
 * Example: you have a ValueType v, and you want to compute its bounding box, but you don't know if
 * ValueType is a pointer or not.
 * Instead of doing:
 * if constexpr(std::is_pointer<ValueType>::value) {
 *     if (vv) vcl::boundingBox(*v);
 * }
 * else{
 *     vcl::boundingBox(v);
 * }
 *
 * you can do:
 * const VT* vv = getCleanValueTypePointer(v);
 * if (vv) vcl::boundingBox(*vv);
 *
 * @param v
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
const typename AbstractDSGrid<GridType, ValueType, DerivedGrid>::VT*
AbstractDSGrid<GridType, ValueType, DerivedGrid>::getCleanValueTypePointer(const ValueType& v)
{
	const VT* vv = nullptr; // vv is a pointer to VT
	if constexpr(std::is_pointer<ValueType>::value) {
		vv = v;
	}
	else {
		vv = &v;
	}

	return vv;
}

} // namespace vcl
