/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "drawable_mesh.h"
#include "draw_objects3.h"

#include <iostream>

namespace vcl {

template<MeshConcept MeshType>
DrawableMesh<MeshType>::DrawableMesh()
{
}

template<MeshConcept MeshType>
DrawableMesh<MeshType>::DrawableMesh(const MeshType &m) :
		GenericDrawableMesh(m), mrb(m)
{
}


template<MeshConcept MeshType>
void DrawableMesh<MeshType>::updateBuffers(const MeshType& m)
{
	mrb = MeshRenderBuffers<MeshType>(m);
	mrs.setRenderCapabilityFrom(m);
}

template<MeshConcept MeshType>
void DrawableMesh<MeshType>::draw() const
{
	if (mrs.isVisible()) {
		if (mrs.isWireframeVisible()) {
			if (mrs.isPointCloudVisible()) {
				glDisable(GL_LIGHTING);
				glShadeModel(GL_FLAT);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDepthRange(0.0, 1.0);
				renderPass();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			if (mrs.isSurfaceVisible()) {
				if (mrs.isSurfaceShadingFlat()) {
					glEnable(GL_LIGHTING);
					glShadeModel(GL_FLAT);
					glDepthRange(0.01, 1.0);
					renderPass();

					glDisable(GL_LIGHTING);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDepthRange(0.0, 1.0);
					glDepthFunc(GL_LEQUAL);
					renderPass();
					glDepthFunc(GL_LESS);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				else if (mrs.isSurfaceShadingSmooth()) {
					glEnable(GL_LIGHTING);
					glShadeModel(GL_SMOOTH);
					glDepthRange(0.01, 1.0);
					renderPass();

					glDisable(GL_LIGHTING);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDepthRange(0.0, 1.0);
					glDepthFunc(GL_LEQUAL);
					renderPass();
					glDepthFunc(GL_LESS);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}
			else {
				glDisable(GL_LIGHTING);
				glShadeModel(GL_FLAT);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDepthRange(0.0, 1.0);
				renderPass();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
		else { // no wireframe
			if (mrs.isPointCloudVisible()) {
				glDisable(GL_LIGHTING);
				renderPass();
			}
			if (mrs.isSurfaceVisible()) {
				if (mrs.isSurfaceShadingFlat()) {
					glEnable(GL_LIGHTING);
					glShadeModel(GL_FLAT);
					renderPass();
				}
				else if (mrs.isSurfaceShadingSmooth()) {
					glEnable(GL_LIGHTING);
					glShadeModel(GL_SMOOTH);
					renderPass();
				}
			}
		}
		if (mrs.isBboxEnabled()) {
			vcl::drawBox3(mrb.bbMin(), mrb.bbMax(), vcl::Color(0, 0, 0));
		}
	}
}

template<MeshConcept MeshType>
Point3d DrawableMesh<MeshType>::sceneCenter() const
{
	return (mrb.bbMin() + mrb.bbMax()) / 2;
}

template<MeshConcept MeshType>
double DrawableMesh<MeshType>::sceneRadius() const
{
	return (mrb.bbMax() - mrb.bbMin()).norm() / 2;
}

template<MeshConcept MeshType>
DrawableMesh<MeshType>* DrawableMesh<MeshType>::clone() const
{
	return new DrawableMesh(*this);
}

template<MeshConcept MeshType>
void DrawableMesh<MeshType>::renderPass() const
{
	uint nv = mrb.vertexNumber();
	uint nt = mrb.triangleNumber();

	const float* coords = mrb.vertexBufferData();
	const int*   triangles = mrb.triangleBufferData();
	const float* vertexNormals = mrb.vertexNormalBufferData();
	const float* vertexColors = mrb.vertexColorBufferData();
	const float* triangleNormals = mrb.triangleNormalBufferData();
	const float* triangleColors = mrb.triangleColorBufferData();

	if (mrs.isPointCloudVisible()) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, coords);

		if (mrs.isPointCloudColorPerVertex()) {
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, vertexColors);
		}
		else if (mrs.isPointCloudColorPerMesh()) {
			glColor4fv(mrb.meshColorBufferData());
		}
		else if (mrs.isPointCloudColorUserDefined()) {
			glColor4fv(mrs.pointCloudUserColorData());
		}

		glPointSize(mrs.pointWidth());

		glDrawArrays(GL_POINTS, 0, nv);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	if (mrs.isSurfaceVisible()) {
		// Old fashioned, verbose and slow rendering.
		if (mrs.isSurfaceColorPerFace()) {
			int n_tris = nt;
			for (int tid = 0; tid < n_tris; ++tid) {
				int tid_ptr  = 3 * tid;
				int vid0     = triangles[tid_ptr + 0];
				int vid1     = triangles[tid_ptr + 1];
				int vid2     = triangles[tid_ptr + 2];
				int vid0_ptr = 3 * vid0;
				int vid1_ptr = 3 * vid1;
				int vid2_ptr = 3 * vid2;

				if (mrs.isSurfaceShadingSmooth()) {
					glBegin(GL_TRIANGLES);
					glColor3fv(&(triangleColors[tid_ptr]));
					glNormal3fv(&(vertexNormals[vid0_ptr]));
					glVertex3fv(&(coords[vid0_ptr]));
					glNormal3fv(&(vertexNormals[vid1_ptr]));
					glVertex3fv(&(coords[vid1_ptr]));
					glNormal3fv(&(vertexNormals[vid2_ptr]));
					glVertex3fv(&(coords[vid2_ptr]));
					glEnd();
				}
				else {
					glBegin(GL_TRIANGLES);
					glColor3fv(&(triangleColors[tid_ptr]));
					glNormal3fv(&(triangleNormals[tid_ptr]));
					glVertex3fv(&(coords[vid0_ptr]));
					glNormal3fv(&(triangleNormals[tid_ptr]));
					glVertex3fv(&(coords[vid1_ptr]));
					glNormal3fv(&(triangleNormals[tid_ptr]));
					glVertex3fv(&(coords[vid2_ptr]));
					glEnd();
				}
			}
		}
		else if (mrs.isSurfaceColorPerVertex()) {
			if (mrs.isSurfaceShadingSmooth()) {
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, coords);

				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, 0, vertexNormals);

				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(3, GL_FLOAT, 0, vertexColors);

				glDrawElements(GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
			else {
				glShadeModel(GL_SMOOTH);
				int n_tris = nt;
				for (int tid = 0; tid < n_tris; ++tid) {
					int tid_ptr  = 3 * tid;
					int vid0     = triangles[tid_ptr + 0];
					int vid1     = triangles[tid_ptr + 1];
					int vid2     = triangles[tid_ptr + 2];
					int vid0_ptr = 3 * vid0;
					int vid1_ptr = 3 * vid1;
					int vid2_ptr = 3 * vid2;

					glBegin(GL_TRIANGLES);
					glColor3fv(&(vertexColors[vid0_ptr]));
					glNormal3fv(&(triangleNormals[tid_ptr]));
					glVertex3fv(&(coords[vid0_ptr]));
					glColor3fv(&(vertexColors[vid1_ptr]));
					glNormal3fv(&(triangleNormals[tid_ptr]));
					glVertex3fv(&(coords[vid1_ptr]));
					glColor3fv(&(vertexColors[vid2_ptr]));
					glNormal3fv(&(triangleNormals[tid_ptr]));
					glVertex3fv(&(coords[vid2_ptr]));
					glEnd();
				}
			}
		}
		else if (mrs.isSurfaceColorPerMesh() || mrs.isSurfaceColorUserDefined()) {
			if (mrs.isSurfaceShadingSmooth()) {
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, coords);

				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, 0, vertexNormals);

				if (mrs.isSurfaceColorPerMesh()) {
					glColor4fv(mrb.meshColorBufferData());
				}
				else {
					glColor4fv(mrs.surfaceUserColorData());
				}

				glDrawElements(GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
			else {
				if (mrs.isSurfaceColorPerMesh()) {
					glColor4fv(mrb.meshColorBufferData());
				}
				else {
					glColor4fv(mrs.surfaceUserColorData());
				}
				int n_tris = nt;
				for (int tid = 0; tid < n_tris; ++tid) {
					int tid_ptr  = 3 * tid;
					int vid0     = triangles[tid_ptr + 0];
					int vid1     = triangles[tid_ptr + 1];
					int vid2     = triangles[tid_ptr + 2];
					int vid0_ptr = 3 * vid0;
					int vid1_ptr = 3 * vid1;
					int vid2_ptr = 3 * vid2;

					glBegin(GL_TRIANGLES);
					glNormal3fv(&(triangleNormals[tid_ptr]));
					glVertex3fv(&(coords[vid0_ptr]));
					glNormal3fv(&(triangleNormals[tid_ptr]));
					glVertex3fv(&(coords[vid1_ptr]));
					glNormal3fv(&(triangleNormals[tid_ptr]));
					glVertex3fv(&(coords[vid2_ptr]));
					glEnd();
				}
			}
		}
	}

	if (mrs.isWireframeVisible()) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, coords);

		glLineWidth(mrs.wireframeWidth());

		if (mrs.isWireframeColorPerMesh()) {
			glColor4fv(mrb.meshColorBufferData());
		}
		else {
			glColor4fv(mrs.wireframeUserColorData());
		}

		glDrawElements(GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

inline void _check_gl_error(const char* file, int line)
{
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
		case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
#ifdef unix
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
#endif
		}

		std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
		err = glGetError();
	}
}

} // namespace vcl