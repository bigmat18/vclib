/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_MESH_CONTAIER_T_H
#define MGP_MESH_MESH_CONTAIER_T_H

#include "../vertex.h"
#include "../face.h"

namespace mgp {

namespace mesh {

class EnabledContainers {
public:
	EnabledContainers() : verticesEnabled(false), facesEnabled(false), edgesEnabled(false), halfEdgesEnabled(false) {}
protected:
	bool verticesEnabled;
	bool facesEnabled;
	bool edgesEnabled;
	bool halfEdgesEnabled;
};

template<class T, typename = void>
class Container
{
};

}

}

#endif // MGP_MESH_MESH_CONTAIER_T_H
