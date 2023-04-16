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

#include <iostream>

#include <vclib/space.h>

#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/miscellaneous.h>
#include <vclib/algorithms.h>

int main()
{
	vcl::RegularGrid3<double> g(vcl::Point3d(0,0,0), vcl::Point3d(1,1,1), vcl::Point3<uint>(10, 10, 10));

	vcl::Point3<uint> first(2,2,2), last(5, 4, 7);

	vcl::HashTableGrid3<vcl::Point<double, 3>, false> sht(g);

	sht.insert(vcl::Point3d(0.05, 0.15, 0.25));
	sht.insert(vcl::Point3d(0.05, 0.15, 0.25)); // duplicate won't be inserted
	sht.insert(vcl::Point3d(0.02, 0.12, 0.29));
	sht.insert(vcl::Point3d(0.12, 0.09, 0.32));

	sht.insert(vcl::Point3d(0.24, 0.52, 0.29));

	std::cerr << "Non Empty cells in HashTableGrid:\n";

	for (const auto& c : sht.nonEmptyCells()) {
		std::cerr << c << "\n";
	}

	std::cerr << "Values in HashTableGrid: \n";

	for (const auto& p : sht) {
		std::cerr << p.first << ": " << p.second << "\n";
	}

	std::cerr << "Values in cell 0, 1, 2: \n";

	auto p = sht.valuesInCell(vcl::Point3<uint>(0,1,2));
	for (auto& it = p.first; it != p.second; ++it) {
		std::cerr << it->second << "; ";
	}
	std::cerr << "\n";

	auto set  = sht.valuesInSphere({vcl::Point3d(0.05, 0.15, 0.25), 0.2});

	std::cerr << "Values in sphere: \n";
	for (auto it : set) {
		std::cerr << it->second << "; ";
	}
	std::cerr << "\n\n";

	auto it = sht.closestValue(vcl::Point3d(0.09, 0.09, 0.29));

	std::cerr << "Closest is: " << it->second << "\n\n";

	sht.eraseInSphere({vcl::Point3d(0.05, 0.15, 0.25), 0.2});

	std::cerr << "Values in HashTableGrid: \n";

	for (const auto& p : sht) {
		std::cerr << p.first << ": " << p.second << "\n";
	}

	std::cerr << "\n==================================\n\n";

	vcl::StaticGrid3<vcl::Point<double, 3>> sg(g);

	sg.insert(vcl::Point3d(0.05, 0.15, 0.25));
	sg.insert(vcl::Point3d(0.05, 0.15, 0.25));
	sg.insert(vcl::Point3d(0.02, 0.12, 0.29));
	sg.insert(vcl::Point3d(0.12, 0.09, 0.32));

	sg.insert(vcl::Point3d(0.24, 0.52, 0.29));
	sg.build();

	std::cerr << "Non Empty cells in StaticGrid:\n";

	for (const auto& c : sg.nonEmptyCells()) {
		std::cerr << c << "\n";
	}

	std::cerr << "Values in StaticGrid: \n";

	for (const auto& p : sg) {
		std::cerr << p.first << ": " << p.second << "\n";
	}

	std::cerr << "Values in cell 0, 1, 2: \n";

	auto pp = sg.valuesInCell(vcl::Point3<uint>(0,1,2));
	for (auto& it = pp.first; it != pp.second; ++it) {
		std::cerr << it->second << "; ";
	}
	std::cerr << "\n";

	auto sset  = sg.valuesInSphere({vcl::Point3d(0.05, 0.15, 0.25), 0.2});

	std::cerr << "Values in sphere: \n";
	for (auto it : sset) {
		std::cerr << it->second << "; ";
	}
	std::cerr << "\n\n";

	std::cerr << "\n==================================\n\n";

	vcl::TriMesh m = vcl::createHexahedron<vcl::TriMesh>();

	auto intersects = vcl::intersectFunction<vcl::Box3d, const vcl::TriMesh::Face*>();
#ifdef VCLIB_USES_RANGES
	vcl::HashTableGrid3<const vcl::TriMesh::Face*> fsht(m.faces() | vcl::views::reference, intersects);

	std::cerr << "Values in HashTableGrid: \n";

	for (const auto& p : fsht) {
		std::cerr << p.first << ": " << m.index(p.second) << "\n";
	}

	std::cerr << "\nValues in Sphere: \n";

	auto sv  = fsht.valuesInSphere({vcl::Point3d(-1, -1, -1), 0.5});

	for (const auto& p : sv) {
		std::cerr << p->first << ": " << m.index(p->second) << "\n";
	}

	std::cerr << "\n==================================\n\n";

	vcl::StaticGrid3<const vcl::TriMesh::Face*> fsg(m.faces() | vcl::views::reference, intersects);

	std::cerr << "Values in Static Grid : \n";

	for (const auto& p : fsg) {
		std::cerr << p.first << ": " << m.index(p.second) << "\n";
	}

	std::cerr << "\nValues in Sphere: \n";

	auto fsv  = fsg.valuesInSphere({vcl::Point3d(-1, -1, -1), 0.5});

	for (const auto& p : fsv) {
		std::cerr << p->first << ": " << m.index(p->second) << "\n";
	}

	std::cerr << "\n==================================\n\n";

	std::cerr << "\nK closest values: \n";

	m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bone.ply");


	vcl::StaticGrid3<const vcl::TriMesh::Vertex*> vmsg(m.vertices() | vcl::views::reference);

	const vcl::Point3d qv(0.5, 0.5, 0.5);

	auto vec = vmsg.kClosestValues(qv, 5);

	for (const auto& p : vec) {
		std::cerr << p->first << ": " << m.index(p->second) << "\n";
	}
#endif
	return 0;
}
