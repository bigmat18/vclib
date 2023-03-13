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

#ifndef VCL_ALGORITHM_DISTANCE_FUNCTIONS_H
#define VCL_ALGORITHM_DISTANCE_FUNCTIONS_H

#include "element.h"
#include "misc.h"

#include <vclib/mesh/requirements.h>

namespace vcl {

// generic case - nothing is done here. It will fail because a specialization must exist
template<typename Obj1, typename Obj2, typename RetScalarType>
struct DistFunctionStruct
{
	static_assert(
		sizeof(Obj1) != sizeof(Obj1), // always fail
		"There is no a DistFunctionStruct specialization for the given types.");
};

/**
 * @brief Return a proper dist function between a Obj1 object and an Obj2 object.
 *
 * The function will return a function defined in a specialization of the struct DistFunctionStruct.
 *
 * The returned function is a std::function that takes in input two const references of the desired
 * types and returns a scalar that is the distance between the two objects.
 *
 * If the distance function for your types is not defined, you can write your own DistFunctionStruct
 * specialization that defines a proper `static const inline` object called `distFun` of
 * `std::function` type.
 */
template<typename Obj1, typename Obj2, typename RetScalarType = double>
auto distFunction()
{
	return DistFunctionStruct<Obj1, Obj2, RetScalarType>::distFun;
}

/********* DistFunctionStruct Specializations *********/

// Specialization for distance between two points
template<PointConcept Obj1, PointConcept Obj2, typename RetScalarType>
	requires (Obj1::DIM == Obj2::DIM)
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static inline const std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return o1.dist(o2);
	};
};

// Specialization for distance between a point and a segment
template<PointConcept Obj1, SegmentConcept Obj2, typename RetScalarType>
	requires (Obj1::DIM == Obj2::DIM)
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static inline const std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return pointSegmentDistance(o1, o2);
	};
};

// Specialization for distance between a segment and a point
template<SegmentConcept Obj1, PointConcept Obj2, typename RetScalarType>
	requires (Obj1::DIM == Obj2::DIM)
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static inline const std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return pointSegmentDistance(o2, o1);
	};
};

// Specialization for distance between Vertex and Point
template<VertexConcept Obj1, Point3Concept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static inline const std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return o1.coord().dist(o2);
	};
};

// Specialization for distance between Vertex* and Point
template<VertexConcept Obj1, Point3Concept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1*, Obj2, RetScalarType>
{
	static inline const std::function<RetScalarType(const Obj1* const&, const Obj2&)> distFun =
		[](const Obj1* const& o1, const Obj2& o2)
	{
		return o1->coord().dist(o2);
	};
};

// Specialization for distance between Point and Vertex
template<Point3Concept Obj1, VertexConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static inline const std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return o1.dist(o2.coord());
	};
};

// Specialization for distance between Point and Vertex*
template<Point3Concept Obj1, VertexConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2*, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1&, const Obj2* const&)> distFun =
		[](const Obj1& o1, const Obj2* const& o2)
	{
		return o1.dist(o2->coord());
	};
};

// Specialization for distance between Point and Face
template<Point3Concept Obj1, FaceConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return pointFaceDistance(o1, o2);
	};
};

// Specialization for distance between Point and Face*
template<Point3Concept Obj1, FaceConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2*, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1&, const Obj2* const&)> distFun =
		[](const Obj1& o1, const Obj2* const& o2)
	{
		return pointFaceDistance(o1, *o2);
	};
};

// Specialization for distance between Face and Point
template<FaceConcept Obj1, Point3Concept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return pointFaceDistance(o2, o1);
	};
};

// Specialization for distance between Face* and Point
template<FaceConcept Obj1, Point3Concept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1*, Obj2, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1* const&, const Obj2&)> distFun =
		[](const Obj1* const& o1, const Obj2& o2)
	{
		return pointFaceDistance(o2, *o1);
	};
};

// Specialization for distance between Vertex and Face
template<VertexConcept Obj1, FaceConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return pointFaceDistance(o1.coord(), o2);
	};
};

// Specialization for distance between Vertex* and Face
template<VertexConcept Obj1, FaceConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1*, Obj2, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1* const&, const Obj2&)> distFun =
		[](const Obj1* const& o1, const Obj2& o2)
	{
		return pointFaceDistance(o1->coord(), o2);
	};
};

// Specialization for distance between Vertex and Face*
template<VertexConcept Obj1, FaceConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2*, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1&, const Obj2* const&)> distFun =
		[](const Obj1& o1, const Obj2* const& o2)
	{
		return pointFaceDistance(o1.coord(), *o2);
	};
};

// Specialization for distance between Vertex* and Face*
template<VertexConcept Obj1, FaceConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1*, Obj2*, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1* const&, const Obj2* const&)> distFun =
		[](const Obj1* const& o1, const Obj2* const& o2)
	{
		return pointFaceDistance(o1->coord(), *o2);
	};
};

// Specialization for distance between Face and Vertex
template<FaceConcept Obj1, VertexConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return pointFaceDistance(o2.coord(), o1);
	};
};

// Specialization for distance between Face* and Vertex
template<FaceConcept Obj1, VertexConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1*, Obj2, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1* const&, const Obj2&)> distFun =
		[](const Obj1* const& o1, const Obj2& o2)
	{
		return pointFaceDistance(o2.coord(), *o1);
	};
};


// Specialization for distance between Face and Vertex*
template<FaceConcept Obj1, VertexConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2*, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1&, const Obj2* const&)> distFun =
		[](const Obj1& o1, const Obj2* const& o2)
	{
		return pointFaceDistance(o2->coord(), o1);
	};
};

// Specialization for distance between Face* and Vertex*
template<FaceConcept Obj1, VertexConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1*, Obj2*, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1* const&, const Obj2* const&)> distFun =
		[](const Obj1* const& o1, const Obj2* const& o2)
	{
		return pointFaceDistance(o2->coord(), *o1);
	};
};

} // namespace vcl

#endif // VCL_ALGORITHM_DISTANCE_FUNCTIONS_H
