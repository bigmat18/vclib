/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <catch2/catch_test_macros.hpp>
#include <vclib/meshes.h>

SCENARIO("TriMesh usage")
{
    GIVEN("An empty TriMesh")
    {
        using TriMeshPoint = vcl::TriMesh::VertexType::CoordType;
        vcl::TriMesh m;

        THEN("The size for each container start at 0")
        {
            REQUIRE(m.vertexNumber() == 0);
            REQUIRE(m.faceNumber() == 0);
        }

        THEN("The optional components are all disabled")
        {
            REQUIRE(m.isPerVertexQualityEnabled() == false);
            REQUIRE(m.isPerVertexColorEnabled() == false);
            REQUIRE(m.isPerVertexAdjacentFacesEnabled() == false);
            REQUIRE(m.isPerVertexAdjacentVerticesEnabled() == false);
            REQUIRE(m.isPerVertexPrincipalCurvatureEnabled() == false);
            REQUIRE(m.isPerVertexTexCoordEnabled() == false);
            REQUIRE(m.isPerVertexMarkEnabled() == false);
            REQUIRE(m.perVertexCustomComponentNames().size() == 0);
            REQUIRE(m.isPerFaceQualityEnabled() == false);
            REQUIRE(m.isPerFaceColorEnabled() == false);
            REQUIRE(m.isPerFaceAdjacentFacesEnabled() == false);
            REQUIRE(m.isPerFaceWedgeTexCoordsEnabled() == false);
            REQUIRE(m.isPerFaceMarkEnabled() == false);
            REQUIRE(m.perFaceCustomComponentNames().size() == 0);
        }

        WHEN("Enabling and disabling all the per vertex optional components")
        {
            m.enableAllPerVertexOptionalComponents();

            REQUIRE(m.isPerVertexQualityEnabled() == true);
            REQUIRE(m.isPerVertexColorEnabled() == true);
            REQUIRE(m.isPerVertexAdjacentFacesEnabled() == true);
            REQUIRE(m.isPerVertexAdjacentVerticesEnabled() == true);
            REQUIRE(m.isPerVertexPrincipalCurvatureEnabled() == true);
            REQUIRE(m.isPerVertexTexCoordEnabled() == true);
            REQUIRE(m.isPerVertexMarkEnabled() == true);

            m.disableAllPerVertexOptionalComponents();

            REQUIRE(m.isPerVertexQualityEnabled() == false);
            REQUIRE(m.isPerVertexColorEnabled() == false);
            REQUIRE(m.isPerVertexAdjacentFacesEnabled() == false);
            REQUIRE(m.isPerVertexAdjacentVerticesEnabled() == false);
            REQUIRE(m.isPerVertexPrincipalCurvatureEnabled() == false);
            REQUIRE(m.isPerVertexTexCoordEnabled() == false);
            REQUIRE(m.isPerVertexMarkEnabled() == false);
        }

        WHEN("Adding and removing vertices")
        {
            uint vi0 = m.addVertex();

            REQUIRE(m.vertexNumber() == 1);
            REQUIRE(m.faceNumber() == 0);
            REQUIRE(m.vertex(0).coord() == TriMeshPoint(0, 0, 0));
            REQUIRE(&m.vertex(vi0) == &m.vertex(0));

            uint vi1 = m.addVertex();

            REQUIRE(m.vertexNumber() == 2);
            REQUIRE(m.vertex(1).coord() == TriMeshPoint(0, 0, 0));
            REQUIRE(&m.vertex(vi0) == &m.vertex(0));
            REQUIRE(&m.vertex(vi1) == &m.vertex(1));

            uint vi2 = m.addVertices(5);
            REQUIRE(m.vertexNumber() == 7);
            REQUIRE(&m.vertex(vi0) == &m.vertex(0));
            REQUIRE(&m.vertex(vi1) == &m.vertex(1));
            REQUIRE(&m.vertex(vi2) == &m.vertex(2));
        }

        WHEN("Adding and removing vertices and faces")
        {
            REQUIRE(m.vertexNumber() == 0);
            REQUIRE(m.faceNumber() == 0);
            m.addVertices(3);
            REQUIRE(m.vertexNumber() == 3);
            uint fi0 = m.addFace();
            REQUIRE(m.faceNumber() == 1);
            REQUIRE(&m.face(fi0) == &m.face(0));
            m.addFace(0, 1, 2);
            REQUIRE(m.faceNumber() == 2);
            REQUIRE(m.face(1).vertexIndex(0) == 0);
            REQUIRE(m.face(1).vertexIndex(1) == 1);
            REQUIRE(m.face(1).vertexIndex(2) == 2);
            REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
            REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
            REQUIRE(m.face(1).vertex(2) == &m.vertex(2));

            // force reallocation of vertex container
            m.addVertices(100);
            REQUIRE(m.vertexNumber() == 103);
            REQUIRE(m.vertexContainerSize() == 103);
            REQUIRE(m.face(1).vertexIndex(0) == 0);
            REQUIRE(m.face(1).vertexIndex(1) == 1);
            REQUIRE(m.face(1).vertexIndex(2) == 2);
            REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
            REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
            REQUIRE(m.face(1).vertex(2) == &m.vertex(2));

            m.face(1).setVertex(2, 3);
            m.deleteVertex(2);
            REQUIRE(m.vertexNumber() == 102);
            REQUIRE(m.vertexContainerSize() == 103);
            REQUIRE(m.face(1).vertexIndex(0) == 0);
            REQUIRE(m.face(1).vertexIndex(1) == 1);
            REQUIRE(m.face(1).vertexIndex(2) == 3);
            REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
            REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
            REQUIRE(m.face(1).vertex(2) == &m.vertex(3));
            m.compactVertices();
            REQUIRE(m.vertexNumber() == 102);
            REQUIRE(m.vertexContainerSize() == 102);
            REQUIRE(m.face(1).vertexIndex(0) == 0);
            REQUIRE(m.face(1).vertexIndex(1) == 1);
            REQUIRE(m.face(1).vertexIndex(2) == 2);
            REQUIRE(m.face(1).vertex(0) == &m.vertex(0));
            REQUIRE(m.face(1).vertex(1) == &m.vertex(1));
            REQUIRE(m.face(1).vertex(2) == &m.vertex(2));
        }

        WHEN("Adding Faces with setVertices")
        {
            m.addVertices(5);
            m.addFaces(4);

            m.face(0).setVertices(0, 1, 2);
            REQUIRE(m.face(0).vertexIndex(0) == 0);
            REQUIRE(m.face(0).vertexIndex(1) == 1);
            REQUIRE(m.face(0).vertexIndex(2) == 2);
            REQUIRE(m.face(0).vertex(0) == &m.vertex(0));
            REQUIRE(m.face(0).vertex(1) == &m.vertex(1));
            REQUIRE(m.face(0).vertex(2) == &m.vertex(2));

            m.face(1).setVertices(&m.vertex(2), &m.vertex(0), &m.vertex(1));
            REQUIRE(m.face(1).vertexIndex(0) == 2);
            REQUIRE(m.face(1).vertexIndex(1) == 0);
            REQUIRE(m.face(1).vertexIndex(2) == 1);
            REQUIRE(m.face(1).vertex(0) == &m.vertex(2));
            REQUIRE(m.face(1).vertex(1) == &m.vertex(0));
            REQUIRE(m.face(1).vertex(2) == &m.vertex(1));

            std::list<uint> l1 = {4, 3, 1};
            m.face(2).setVertices(l1);
            REQUIRE(m.face(2).vertexIndex(0) == 4);
            REQUIRE(m.face(2).vertexIndex(1) == 3);
            REQUIRE(m.face(2).vertexIndex(2) == 1);
            REQUIRE(m.face(2).vertex(0) == &m.vertex(4));
            REQUIRE(m.face(2).vertex(1) == &m.vertex(3));
            REQUIRE(m.face(2).vertex(2) == &m.vertex(1));

            std::list<vcl::TriMesh::Vertex*> l2 = {
                &m.vertex(2), &m.vertex(4), &m.vertex(3)};
            m.face(3).setVertices(l2);
            REQUIRE(m.face(3).vertexIndex(0) == 2);
            REQUIRE(m.face(3).vertexIndex(1) == 4);
            REQUIRE(m.face(3).vertexIndex(2) == 3);
            REQUIRE(m.face(3).vertex(0) == &m.vertex(2));
            REQUIRE(m.face(3).vertex(1) == &m.vertex(4));
            REQUIRE(m.face(3).vertex(2) == &m.vertex(3));
        }
    }
}
