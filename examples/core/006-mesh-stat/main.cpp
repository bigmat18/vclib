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

#include <iostream>

#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

int main()
{
    vcl::TriMesh m = vcl::loadPly<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH
                                                "/bunny_textured.ply");

    auto bar = vcl::barycenter(m);

    std::cerr << "Barycenter: " << bar << "\n";

    bar = vcl::shellBarycenter(m);

    std::cerr << "Shell Barycenter: " << bar << "\n";

    m = vcl::loadPly<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bone.ply");

    double vol = vcl::volume(m);

    std::cerr << "Mesh volume: " << vol << "\n";

    double surf = vcl::surfaceArea(m);

    std::cerr << "Surface area: " << surf << "\n";

    return 0;
}
