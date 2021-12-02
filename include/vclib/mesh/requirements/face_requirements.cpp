/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
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

#include "face_requirements.h"

#include <vclib/exception/mesh_exception.h>

#include "../container/face_container.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

template<typename MeshType>
bool constexpr hasPerFaceAdjacentFaces()
{
	return hasFaces<MeshType>() &&
		   vcl::face::hasAdjacentFaces<typename MeshType::FaceType>();
}

template<typename MeshType>
bool isPerFaceAdjacentFacesEnabled(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() &&
		vcl::face::hasOptionalAdjacentFaces<typename MeshType::FaceType>()) {
		return m.isPerFaceAdjacentFacesEnabled();
	}
	else if constexpr (
		hasFaces<MeshType>() && vcl::face::hasAdjacentFaces<typename MeshType::FaceType>()) {
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceAdjacentFacesOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() &&
		vcl::face::hasOptionalAdjacentFaces<typename MeshType::FaceType>()) {
		m.enablePerFaceAdjacentFaces();
		return true;
	}
	else {
		return hasPerFaceAdjacentFaces<MeshType>();
	}
}

template<typename MeshType>
bool constexpr hasPerFaceColor()
{
	return hasFaces<MeshType>() && (vcl::face::hasColor<typename MeshType::FaceType>() ||
									vcl::face::hasOptionalColor<typename MeshType::FaceType>());
}

template<typename MeshType>
bool isPerFaceColorEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>() && vcl::face::hasColor<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && vcl::face::hasOptionalColor<typename MeshType::FaceType>()) {
		return m.isPerFaceColorEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceColorOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && vcl::face::hasOptionalColor<typename MeshType::FaceType>()) {
		m.enablePerFaceColor();
		return true;
	}
	else {
		return hasPerFaceColor<MeshType>();
	}
}

template<typename MeshType>
bool constexpr hasPerFaceMark()
{
	return hasFaces<MeshType>() && (vcl::face::hasMark<typename MeshType::FaceType>() ||
									vcl::face::hasOptionalMark<typename MeshType::FaceType>());
}

template<typename MeshType>
bool isPerFaceMarkEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>() && vcl::face::hasMark<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && vcl::face::hasOptionalMark<typename MeshType::FaceType>()) {
		return m.isPerFaceMarkEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceMarkOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && vcl::face::hasOptionalMark<typename MeshType::FaceType>()) {
		m.enablePerFaceMark();
		return true;
	}
	else {
		return hasPerFaceMark<MeshType>();
	}
}

template<typename MeshType>
bool constexpr hasPerFaceMutableBitFlags()
{
	return hasFaces<MeshType>() &&
		   (vcl::face::hasMutableBitFlags<typename MeshType::FaceType>() ||
			vcl::face::hasOptionalMutableBitFlags<typename MeshType::FaceType>());
}

template<typename MeshType>
bool isPerFaceMutableBitFlagsEnabled(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && vcl::face::hasMutableBitFlags<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() &&
		vcl::face::hasOptionalMutableBitFlags<typename MeshType::FaceType>()) {
		return m.isPerFaceMutableBitFlagsEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceMutableBitFlagsOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() &&
		vcl::face::hasOptionalMutableBitFlags<typename MeshType::FaceType>()) {
		m.enablePerFaceMutableBitFlags();
		return true;
	}
	else {
		return hasPerFaceMutableBitFlags<MeshType>();
	}
}

template<typename MeshType>
bool constexpr hasPerFaceNormal()
{
	return hasFaces<MeshType>() && (vcl::face::hasNormal<typename MeshType::FaceType>() ||
									vcl::face::hasOptionalNormal<typename MeshType::FaceType>());
}

template<typename MeshType>
bool isPerFaceNormalEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>() && vcl::face::hasNormal<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && vcl::face::hasOptionalNormal<typename MeshType::FaceType>()) {
		return m.isPerFaceNormalEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceNormalOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && vcl::face::hasOptionalNormal<typename MeshType::FaceType>()) {
		m.enablePerFaceNormal();
		return true;
	}
	else {
		return hasPerFaceNormal<MeshType>();
	}
}

template<typename MeshType>
bool constexpr hasPerFacePrincipalCurvature()
{
	return hasFaces<MeshType>() &&
		   (vcl::face::hasPrincipalCurvature<typename MeshType::FaceType>() ||
			vcl::face::hasOptionalPrincipalCurvature<typename MeshType::FaceType>());
}

template<typename MeshType>
bool isPerFacePrincipalCurvatureEnabled(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && vcl::face::hasPrincipalCurvature<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() &&
		vcl::face::hasOptionalPrincipalCurvature<typename MeshType::FaceType>()) {
		return m.isPerFacePrincipalCurvatureEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFacePrincipalCurvatureOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() &&
		vcl::face::hasOptionalPrincipalCurvature<typename MeshType::FaceType>()) {
		m.enablePerFacePrincipalCurvature();
		return true;
	}
	else {
		return hasPerFacePrincipalCurvature<MeshType>();
	}
}

template<typename MeshType>
bool constexpr hasPerFaceScalar()
{
	return hasFaces<MeshType>() && (vcl::face::hasScalar<typename MeshType::FaceType>() ||
									vcl::face::hasOptionalScalar<typename MeshType::FaceType>());
}

template<typename MeshType>
bool isPerFaceScalarEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>() && vcl::face::hasScalar<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && vcl::face::hasOptionalScalar<typename MeshType::FaceType>()) {
		return m.isPerFaceScalarEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceScalarOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && vcl::face::hasOptionalScalar<typename MeshType::FaceType>()) {
		m.enablePerFaceScalar();
		return true;
	}
	else {
		return hasPerFaceScalar<MeshType>();
	}
}

template<typename MeshType>
bool constexpr hasPerFaceWedgeColors()
{
	return hasFaces<MeshType>() &&
		   (vcl::face::hasWedgeColors<typename MeshType::FaceType>() ||
			vcl::face::hasOptionalWedgeColors<typename MeshType::FaceType>());
}

template<typename MeshType>
bool isPerFaceWedgeColorsEnabled(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && vcl::face::hasWedgeColors<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && vcl::face::hasOptionalWedgeColors<typename MeshType::FaceType>()) {
		return m.isPerFaceWedgeColorsEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceWedgeColorsOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && vcl::face::hasOptionalWedgeColors<typename MeshType::FaceType>()) {
		m.enablePerFaceWedgeColors();
		return true;
	}
	else {
		return hasPerFaceWedgeColors<MeshType>();
	}
}

template<typename MeshType>
bool constexpr hasPerFaceWedgeTexCoords()
{
	return hasFaces<MeshType>() &&
		   (vcl::face::hasWedgeTexCoords<typename MeshType::FaceType>() ||
			vcl::face::hasOptionalWedgeTexCoords<typename MeshType::FaceType>());
}

template<typename MeshType>
bool isPerFaceWedgeTexCoordsEnabled(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && vcl::face::hasWedgeTexCoords<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() &&
		vcl::face::hasOptionalWedgeTexCoords<typename MeshType::FaceType>()) {
		return m.isPerFaceWedgeTexCoordsEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceWedgeTexCoordsOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() &&
		vcl::face::hasOptionalWedgeTexCoords<typename MeshType::FaceType>()) {
		m.enablePerFaceWedgeTexCoords();
		return true;
	}
	else {
		return hasPerFaceWedgeTexCoords<MeshType>();
	}
}

template<typename MeshType>
bool constexpr hasPerFaceCustomComponents()
{
	return hasFaces<MeshType>() && vcl::face::hasCustomComponents<typename MeshType::FaceType>();
}

template<typename MeshType>
bool constexpr hasPerFaceCustomComponents(const MeshType&)
{
	return hasPerFaceCustomComponents<MeshType>();
}

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void requirePerFaceAdjacentFaces(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceAdjacentFaces<MeshType>(), "Mesh has no per face adjacent faces.");
	if (!isPerFaceAdjacentFacesEnabled(m))
		throw vcl::MissingComponentException("Per face adjacent faces not enabled.");
}

template<typename MeshType>
void requirePerFaceColor(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceColor<MeshType>(), "Mesh has no face colors.");
	if (!isPerFaceColorEnabled(m))
		throw vcl::MissingComponentException("Face colors not enabled.");
}

template<typename MeshType>
void requirePerFaceMark(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceMark<MeshType>(), "Mesh has no face mark.");
	if (!isPerFaceMarkEnabled(m))
		throw vcl::MissingComponentException("Face mark not enabled.");
}

template<typename MeshType>
void requirePerFaceMutableBitFlags(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceMutableBitFlags<MeshType>(), "Mesh has no per face mutable bit flags.");
	if (!isPerFaceMutableBitFlagsEnabled(m))
		throw vcl::MissingComponentException("Face mutable bit flags not enabled.");
}

template<typename MeshType>
void requirePerFaceNormal(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceNormal<MeshType>(), "Mesh has no face normals.");
	if (!isPerFaceNormalEnabled(m))
		throw vcl::MissingComponentException("Face normals not enabled.");
}

template<typename MeshType>
void requirePerFacePrincipalCurvature(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFacePrincipalCurvature<MeshType>(), "Mesh has no face principal curvature.");
	if (!isPerFacePrincipalCurvatureEnabled(m))
		throw vcl::MissingComponentException("Face principal curvature not enabled.");
}

template<typename MeshType>
void requirePerFaceScalar(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceScalar<MeshType>(), "Mesh has no face scalars.");
	if (!isPerFaceScalarEnabled(m))
		throw vcl::MissingComponentException("Face scalars not enabled.");
}

template<typename MeshType>
void requirePerFaceWedgeColors(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceWedgeColors<MeshType>(), "Mesh has no face wedge colors.");
	if (!isPerFaceWedgeColorsEnabled(m))
		throw vcl::MissingComponentException("Face wedge colors not enabled.");
}

template<typename MeshType>
void requirePerFaceWedgeTexCoords(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceWedgeTexCoords<MeshType>(), "Mesh has no face wedge texcoords.");
	if (!isPerFaceWedgeTexCoordsEnabled(m))
		throw vcl::MissingComponentException("Face wedge texcoords not enabled.");
}

template<typename MeshType>
void constexpr requirePerFaceCustomComponents()
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceCustomComponents<MeshType>(), "Mesh has no face custom components.");
}

template<typename MeshType>
void requirePerFaceCustomComponents(const MeshType&)
{
	requirePerFaceCustomComponents<MeshType>();
}

} // namespace vcl
