/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MATH_TRANSFORM_H
#define VCL_MATH_TRANSFORM_H

#include "base.h"

#include <vclib/concepts/space/matrix.h>
#include <vclib/concepts/space/point.h>

namespace vcl {

/**
 * @brief Given an 3D axis and an angle expressed in radiants, fills the given
 * matrix with a transform matrix that represents the rotation matrix of the
 * given axis/angle.
 *
 * The given matrix must be at least a 3x3 matrix. If the matrix is a higher
 * than 3x3 (e.g. 4x4), only the 3x3 submatrix will be set, leaving unchanged
 * the other values.
 *
 * @param matrix
 * @param axis
 * @param angleRad
 *
 * @ingroup math
 */
template<MatrixConcept MatrixType, Point3Concept PointType, typename ScalarType>
void setTransformMatrixRotation(
    MatrixType&       matrix,
    PointType         axis,
    const ScalarType& angleRad)
{
    ScalarType c = std::cos(angleRad);
    ScalarType s = std::sin(angleRad);
    ScalarType q = 1 - c;
    axis.normalize();
    matrix(0, 0) = axis[0] * axis[0] * q + c;
    matrix(0, 1) = axis[0] * axis[1] * q - axis[2] * s;
    matrix(0, 2) = axis[0] * axis[2] * q + axis[1] * s;
    matrix(1, 0) = axis[1] * axis[0] * q + axis[2] * s;
    matrix(1, 1) = axis[1] * axis[1] * q + c;
    matrix(1, 2) = axis[1] * axis[2] * q - axis[0] * s;
    matrix(2, 0) = axis[2] * axis[0] * q - axis[1] * s;
    matrix(2, 1) = axis[2] * axis[1] * q + axis[0] * s;
    matrix(2, 2) = axis[2] * axis[2] * q + c;
}

/**
 * @brief Given an 3D axis and an angle expressed in degrees, fills the given
 * matrix with a transform matrix that represents the rotation matrix of the
 * given axis/angle.
 *
 * The given matrix must be at least a 3x3 matrix. If the matrix is a higher
 * than 3x3 (e.g. 4x4), only the 3x3 submatrix will be set, leaving unchanged
 * the other values.
 *
 * @param matrix
 * @param axis
 * @param angleDeg
 *
 * @ingroup math
 */
template<MatrixConcept MatrixType, Point3Concept PointType, typename ScalarType>
void setTransformMatrixRotationDeg(
    MatrixType&       matrix,
    PointType         axis,
    const ScalarType& angleDeg)
{
    setTransformMatrixRotation(matrix, axis, toRad(angleDeg));
}

/**
 * @brief Given two 3D vectors, fills the given matrix with a transform matrix
 * that represents the rotation matrix from the first vector to the second
 * vector.
 *
 * The given matrix must be at least a 3x3 matrix. If the matrix is a higher
 * than 3x3 (e.g. 4x4), only the 3x3 submatrix will be set, leaving unchanged
 * the other values.
 *
 * @param matrix
 * @param fromVector
 * @param toVector
 *
 * @ingroup math
 */
template<MatrixConcept MatrixType, Point3Concept PointType>
void setTransformMatrixRotation(
    MatrixType&      matrix,
    const PointType& fromVector,
    const PointType& toVector)
{
    if (fromVector == toVector) {
        matrix.block(0, 0, 3, 3).setIdentity();
        return;
    }
    PointType axis  = fromVector.cross(toVector);
    auto      angle = std::acos(
        fromVector.dot(toVector) / (fromVector.norm() * toVector.norm()));
    setTransformMatrixRotation(matrix, axis, angle);
}

template<MatrixConcept MatrixType, Point3Concept PointType>
void setTransformMatrixTranslation(
    MatrixType&      matrix,
    const PointType& translation)
{
    matrix(0, 3) = translation[0];
    matrix(1, 3) = translation[1];
    matrix(2, 3) = translation[2];
}

template<MatrixConcept MatrixType, typename ScalarType>
void setTransformMatrixScale(MatrixType& matrix, const ScalarType& scale)
{
    matrix(0, 0) = scale;
    matrix(1, 1) = scale;
    matrix(2, 2) = scale;
    matrix(3, 3) = 1;
}

template<MatrixConcept MatrixType, Point3Concept PointType>
void setTransformMatrixScale(MatrixType& matrix, const PointType& scale)
{
    matrix(0, 0) = scale[0];
    matrix(1, 1) = scale[1];
    matrix(2, 2) = scale[2];
    matrix(3, 3) = 1;
}

/**
 * @brief Given an 3D axis and an angle expressed in radiants, returns a
 * transform matrix that represents the rotation matrix of the given axis/angle.
 *
 * The MatrixType must be at least a 3x3 matrix having the setIdentity() member
 * function. If the matrix is a higher than 3x3 (e.g. 4x4), only the 3x3
 * submatrix will be set, leaving the identity values in the other cells of the
 * matrix.
 *
 * @param axis
 * @param angleRad
 * @return
 *
 * @ingroup math
 */
template<MatrixConcept MatrixType, Point3Concept PointType, typename ScalarType>
MatrixType rotationMatrix(const PointType& axis, const ScalarType& angleRad)
{
    MatrixType matrix;
    matrix.setIdentity();
    setTransformMatrixRotation(matrix, axis, angleRad);
    return matrix;
}

/**
 * @brief Given an 3D axis and an angle expressed in degrees, fills the given
 * matrix with a transform matrix that represents the rotation matrix of the
 * given axis/angle.
 *
 * The MatrixType must be at least a 3x3 matrix having the setIdentity() member
 * function. If the matrix is a higher than 3x3 (e.g. 4x4), only the 3x3
 * submatrix will be set, leaving the identity values in the other cells of the
 * matrix.
 *
 * @param axis
 * @param angleDeg
 * @return
 *
 * @ingroup math
 */
template<MatrixConcept MatrixType, Point3Concept PointType, typename ScalarType>
MatrixType rotationMatrixDeg(const PointType& axis, const ScalarType& angleDeg)
{
    return rotationMatrix<MatrixType>(axis, toRad(angleDeg));
}

/**
 * @brief Given two 3D vectors, returns a transform matrix that represents the
 * rotation matrix from the first vector to the second vector.
 *
 * The MatrixType must be at least a 3x3 matrix having the setIdentity() member
 * function. If the matrix is a higher than 3x3 (e.g. 4x4), only the 3x3
 * submatrix will be set, leaving the identity values in the other cells of the
 * matrix.
 *
 * @param fromVector
 * @param toVector
 * @return
 *
 * @ingroup math
 */
template<MatrixConcept MatrixType, Point3Concept PointType>
MatrixType rotationMatrix(
    const PointType& fromVector,
    const PointType& toVector)
{
    MatrixType matrix;
    matrix.setIdentity();
    setTransformMatrixRotation(matrix, fromVector, toVector);
    return matrix;
}

} // namespace vcl

#endif // VCL_MATH_TRANSFORM_H
