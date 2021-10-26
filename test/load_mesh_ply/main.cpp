/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/trimesh.h>
#include <mgp/io/load_ply.h>

int main()
{
	mgp::TriMesh m;

	mgp::io::FileMeshInfo finfo;
	mgp::io::loadPly(m, "/home/alessandro/tmp/cube.ply", finfo);

	return 0;
}
