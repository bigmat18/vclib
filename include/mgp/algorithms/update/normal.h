/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

/**
 * This is a refactoring of VCGLib: https://github.com/cnr-isti-vclab/vcglib/
 * Copyright(C): Visual Computing Lab
 * ISTI - Italian National Research Council
 */

#ifndef MGP_ALGORITHMS_NORMAL_H
#define MGP_ALGORITHMS_NORMAL_H

namespace mgp {

template<typename Triangle>
double triangleNormal(const Triangle& t);

template<typename Polygon>
double polygonNormal(const Polygon& p);

template<typename MeshType>
void normalizePerFaceNormals(MeshType& m);

template<typename MeshType>
void updatePerFaceNormals(MeshType& m, bool normalize = true);

} // namespace mgp

#include "normal.cpp"

#endif // MGP_ALGORITHMS_NORMAL_H
