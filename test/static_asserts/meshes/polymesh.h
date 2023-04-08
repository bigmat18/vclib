#ifndef POLYMESH_H
#define POLYMESH_H

#include "mesh_views.h"

#include <vclib/mesh/poly_mesh.h>

void polymeshStaticAsserts()
{
	using namespace vcl;

	static_assert(MeshConcept<PolyMesh>, "The PolyMesh is not a valid Mesh.");
	static_assert(FaceMeshConcept<PolyMesh>, "The PolyMesh is not a valid Mesh with Faces.");
	static_assert(!TriangleMeshConcept<PolyMesh>, "The PolyMesh is a static Triangle Mesh.");
	static_assert(!QuadMeshConcept<PolyMesh>, "The PolyMesh is a static Quad Mesh.");
	static_assert(!EdgeMeshConcept<PolyMesh>, "The PolyMesh is an Edge Mesh.");
	static_assert(PolygonMeshConcept<PolyMesh>, "The PolyMesh is not a valid Polygon Mesh.");
	static_assert(!DcelMeshConcept<PolyMesh>, "The PolyMesh is a Dcel Mesh.");

	static_assert(HasVertices<PolyMesh>, "");
	static_assert(HasFaces<PolyMesh>, "");
	static_assert(!HasEdges<PolyMesh>, "");
	static_assert(!HasHalfEdges<PolyMesh>, "");

	static_assert(!HasTriangles<PolyMesh>, "");
	static_assert(!HasQuads<PolyMesh>, "");
	static_assert(HasPolygons<PolyMesh>, "");

	meshViewsStaticAsserts<PolyMesh>();
}

#endif // POLYMESH_H