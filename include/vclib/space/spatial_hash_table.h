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

#ifndef VCL_SPACE_SPATIAL_HASH_TABLE_H
#define VCL_SPACE_SPATIAL_HASH_TABLE_H

#include "grid.h"

#include <set>
#include <unordered_map>

namespace vcl {

template<typename GridType, typename ValueType>
class SpatialHashTable : public GridType
{
public:
	using KeyType = typename GridType::CellCoord;
	using iterator = typename std::unordered_multimap<KeyType, ValueType>::iterator;
	using const_iterator = typename std::unordered_multimap<KeyType, ValueType>::const_iterator;

	SpatialHashTable();

	template<typename BoxType>
	SpatialHashTable(const BoxType& bbox, const KeyType& size);

	bool empty() const;
	std::size_t size() const;
	bool cellEmpty(const KeyType& k) const;
	std::size_t cellSize(const KeyType& k) const;

	std::set<KeyType> cells() const;

	void clear();

	void insert(const KeyType& k, const ValueType& v);
	void insert(const ValueType& v) requires (GridType::DIM == 2);
	void insert(const ValueType& v) requires (GridType::DIM == 3);
	void insert(const ValueType& v);

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

private:
	using MapValueType = typename std::unordered_multimap<KeyType, ValueType>::value_type;

	std::unordered_multimap<KeyType, ValueType> map;
};

template<typename ValueType, typename ScalarType = double>
using SpatialHashTable2 = SpatialHashTable<Grid2<ScalarType>, ValueType>;

template<typename ValueType, typename ScalarType = double>
using SpatialHashTable3 = SpatialHashTable<Grid3<ScalarType>, ValueType>;

} // namespace vcl

#include "spatial_hash_table.cpp"

#endif // VCL_SPACE_SPATIAL_HASH_TABLE_H
