#ifndef TRIMESH_H
#define TRIMESH_H

#include "mesh_views.h"

#include <vclib/meshes/tri_mesh.h>

void trimeshStaticAsserts()
{
	using namespace vcl;

	// mesh concepts
	static_assert(MeshConcept<TriMesh>, "The TriMesh is not a valid Mesh.");
	static_assert(FaceMeshConcept<TriMesh>, "The TriMesh is not a valid Mesh with Faces.");
	static_assert(TriangleMeshConcept<TriMesh>, "The TriMesh is not a static Triangle Mesh.");
	static_assert(!QuadMeshConcept<TriMesh>, "The TriMesh is a static Quad Mesh.");
	static_assert(!EdgeMeshConcept<TriMesh>, "The TriMesh is an Edge Mesh.");
	static_assert(!PolygonMeshConcept<TriMesh>, "The TriMesh is a valid Polygon Mesh.");
	static_assert(!DcelMeshConcept<TriMesh>, "The TriMesh is a Dcel Mesh.");

	static_assert(HasVertices<TriMesh>, "");
	static_assert(HasFaces<TriMesh>, "");
	static_assert(!HasEdges<TriMesh>, "");
	static_assert(!HasHalfEdges<TriMesh>, "");

	static_assert(HasTriangles<TriMesh>, "");
	static_assert(!HasQuads<TriMesh>, "");
	static_assert(!HasPolygons<TriMesh>, "");

	// mesh views
	meshViewsStaticAsserts<TriMesh>();
}


#endif // TRIMESH_H
