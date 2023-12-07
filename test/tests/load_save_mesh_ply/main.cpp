/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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
#include <vclib/load_save.h>
#include <vclib/meshes.h>

std::istringstream plyPolyCube()
{
    // string containing a cube in PLY format

    std::string s =
        "ply\n"
        "format ascii 1.0\n"
        "comment Created by VCLib\n"
        "element vertex 8\n"
        "property float x\n"
        "property float y\n"
        "property float z\n"
        "element face 6\n"
        "property list uchar int vertex_indices\n"
        "end_header\n"
        "-0.500000 -0.500000 0.500000\n"
        "0.500000 -0.500000 0.500000\n"
        "-0.500000 0.500000 0.500000\n"
        "0.500000 0.500000 0.500000\n"
        "-0.500000 0.500000 -0.500000\n"
        "0.500000 0.500000 -0.500000\n"
        "-0.500000 -0.500000 -0.500000\n"
        "0.500000 -0.500000 -0.500000\n"
        "4 0 1 3 2\n"
        "4 2 3 5 4\n"
        "4 4 5 7 6\n"
        "4 6 7 1 0\n"
        "4 1 7 5 3\n"
        "4 6 0 2 4\n";

    std::istringstream ss(s);
    return ss;
}

std::istringstream plyTriCube()
{
    // string containing a triangulted cube in OBJ format

    std::string s =
        "ply\n"
        "format ascii 1.0\n"
        "comment Created by VCLib\n"
        "element vertex 8\n"
        "property float x\n"
        "property float y\n"
        "property float z\n"
        "element face 12\n"
        "property list uchar int vertex_indices\n"
        "end_header\n"
        "-0.500000 -0.500000 0.500000\n"
        "0.500000 -0.500000 0.500000\n"
        "-0.500000 0.500000 0.500000\n"
        "0.500000 0.500000 0.500000\n"
        "-0.500000 0.500000 -0.500000\n"
        "0.500000 0.500000 -0.500000\n"
        "-0.500000 -0.500000 -0.500000\n"
        "0.500000 -0.500000 -0.500000\n"
        "3 0 1 2\n"
        "3 1 3 2\n"
        "3 2 3 4\n"
        "3 3 5 4\n"
        "3 4 5 6\n"
        "3 5 7 6\n"
        "3 6 7 0\n"
        "3 7 1 0\n"
        "3 1 7 3\n"
        "3 7 5 3\n"
        "3 5 7 6\n"
        "3 7 5 4\n";

    std::istringstream ss(s);
    return ss;
}

// Test to load obj from a istringstream
TEST_CASE("Load PLY cube from istringstream")
{
    SECTION("TriMesh - PolyCube")
    {
        vcl::TriMesh tm;
        auto         ss = plyPolyCube();
        vcl::loadPly(tm, ss);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    SECTION("TriMesh - TriCube")
    {
        vcl::TriMesh tm;
        auto         ss = plyTriCube();
        vcl::loadPly(tm, ss);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    SECTION("PolyMesh - PolyCube")
    {
        vcl::PolyMesh pm;
        auto          ss = plyPolyCube();
        vcl::loadPly(pm, ss);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 6);
    }

    SECTION("PolyMesh - TriCube")
    {
        vcl::PolyMesh pm;
        auto          ss = plyTriCube();
        vcl::loadPly(pm, ss);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 12);
    }
}
