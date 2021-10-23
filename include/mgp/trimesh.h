/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_TRIMESH_H
#define MGP_MESH_TRIMESH_H

#include "mesh/mesh.h"

namespace mgp {

namespace trimesh {

class Vertex;
class Face;

class Vertex :
		public mgp::Vertex<
			mgp::vert::BitFlags,
			mgp::vert::Coordinate3d,
			mgp::vert::Normal3d,
			mgp::vert::OptionalColor<Vertex>,
			mgp::vert::Scalard,
			mgp::vert::FaceAdjacencyRef<Face>>
{
};

class Face :
		public mgp::
			Face<
				mgp::face::BitFlags,
				mgp::face::TriangleVertexRefsArray<Vertex>,
				mgp::vert::Normal3d,
				mgp::vert::OptionalScalard<Face>,
				mgp::face::OptionalColor<Face>>
{
};

} // namespace trimesh

class TriMesh : public mgp::Mesh<trimesh::Vertex, trimesh::Face, mgp::common::Color>
{
};

} // namespace mgp

#endif // MGP_MESH_FACE_H
