/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "ply_face.h"

#include <vclib/algorithms/polygon.h>
#include <vclib/exception/io_exception.h>
#include <vclib/mesh/requirements.h>

namespace vcl::ply {

namespace internal {

template<typename MeshType, typename FaceType>
void saveFaceIndices(
	std::ofstream&          file,
	Property                p,
	const MeshType&         m,
	const std::vector<int>& vIndices,
	const FaceType&         f,
	bool                    bin)
{
	using VertexType = typename MeshType::VertexType;

	uint fsize = f.vertexNumber();
	internal::writeProperty(file, fsize, p.listSizeType, bin);
	for (const VertexType* v : f.vertices()) {
		internal::writeProperty(file, vIndices[m.index(v)], p.type, bin);
	}
}

template<typename MeshType, typename FaceType>
void setFaceIndices(FaceType& f, MeshType& m, const std::vector<uint>& vids)
{
	bool splitFace = false;
	// we have a polygonal mesh
	if constexpr (FaceType::VERTEX_NUMBER < 0) {
		f.resizeVertices(vids.size()); // need to resize the face to the right number of verts
	}
	else if (FaceType::VERTEX_NUMBER != vids.size()) {
		// we have faces with static sizes (triangles), but we are loading faces with number of
		// verts > 3. Need to split the face we are loading in n faces!
		splitFace = true;
	}

	if (!splitFace) { // classic load, no split needed
		for (uint i = 0; i < vids.size(); ++i) {
			if (vids[i] >= m.vertexNumber()) {
				throw vcl::MalformedFileException("Bad vertex index for face " + std::to_string(i));
			}
			f.vertex(i) = &m.vertex(vids[i]);
		}
	}
	else { // split needed
		addTriangleFacesFromPolygon(m, f, vids);
	}
}

template<typename MeshType, typename FaceType, typename Scalar>
void setFaceWedgeTexCoords(
	FaceType&                                     f,
	MeshType&                                     m,
	const std::vector<uint>&                      vids,
	const std::vector<std::pair<Scalar, Scalar>>& wedges)
{
	bool splitFace = false;
	if (FaceType::VERTEX_NUMBER > 0 && FaceType::VERTEX_NUMBER != wedges.size())
		splitFace = true;
	if (!splitFace) {
		for (uint i = 0; i < wedges.size(); ++i) {
			f.wedgeTexCoord(i).u() = wedges[i].first;
			f.wedgeTexCoord(i).v() = wedges[i].second;
		}
	}
	else { // the face has been splitted
		// for each triangle generated by the polygon
		for (uint ff = m.index(f); ff < m.faceNumber(); ++ff) {
			// for each vertex/wedge of the triangle
			for (uint i = 0; i < m.face(ff).vertexNumber(); ++i) {
				// get the position of this vertex in the original polygon
				uint vid = m.index(m.face(ff).vertex(i));
				auto it  = std::find(vids.begin(), vids.end(), vid);
				if (it == vids.end()) { // should never happen
					// if this happens, it means that this triangle was not generated by the polygon
					throw vcl::MalformedFileException(
						"Bad vertex index for face " + std::to_string(ff));
				}
				uint p = it - vids.begin(); // p is the position of the vertex in the polygon
				m.face(ff).wedgeTexCoord(i).u() = wedges[p].first;
				m.face(ff).wedgeTexCoord(i).v() = wedges[p].second;
			}
		}
	}
}

template<typename MeshType, typename FaceType, typename Stream>
void loadFaceProperty(Stream& file, MeshType& mesh, FaceType& f, ply::Property p)
{
	bool              hasBeenRead = false;
	std::vector<uint> vids; // contains the vertex ids of the actual face
	if (p.name == ply::vertex_indices) { // loading vertex indices
		uint fSize = internal::readProperty<uint>(file, p.listSizeType);
		vids.resize(fSize);
		for (uint i = 0; i < fSize; ++i) {
			vids[i] = internal::readProperty<size_t>(file, p.type);
		}
		hasBeenRead = true;
		// will manage the case of loading a polygon in a triangle mesh
		setFaceIndices(f, mesh, vids);
	}
	if (p.name == ply::texcoord) { // loading wedge texcoords
		if constexpr (vcl::hasPerFaceWedgeTexCoords<MeshType>()) {
			if (vcl::isPerFaceWedgeTexCoordsEnabled(mesh)) {
				using Scalar = typename FaceType::WedgeTexCoordType::ScalarType;
				uint uvSize  = internal::readProperty<uint>(file, p.listSizeType);
				uint fSize   = uvSize / 2;
				std::vector<std::pair<Scalar, Scalar>> wedges(fSize);
				for (uint i = 0; i < fSize; ++i) {
					Scalar u         = internal::readProperty<Scalar>(file, p.type);
					Scalar v         = internal::readProperty<Scalar>(file, p.type);
					wedges[i].first  = u;
					wedges[i].second = v;
				}
				hasBeenRead = true;
				setFaceWedgeTexCoords(f, mesh, vids, wedges);
			}
		}
	}
	if (p.name == ply::texnumber) { // loading texture id associated to ALL the wedges
		if constexpr (vcl::hasPerFaceWedgeTexCoords<MeshType>()) {
			if (vcl::isPerFaceWedgeTexCoordsEnabled(mesh)) {
				uint n      = internal::readProperty<uint>(file, p.type);
				hasBeenRead = true;
				// in case the loaded polygon has been triangulated in the last n triangles of mesh
				for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff) {
					for (uint i = 0; i < mesh.face(ff).vertexNumber(); ++i) {
						mesh.face(ff).wedgeTexCoord(i).nTexture() = n;
					}
				}
			}
		}
	}
	if (p.name >= ply::nx && p.name <= ply::nz) { // loading one of the normal components
		if constexpr (vcl::hasPerFaceNormal<MeshType>()) {
			if (vcl::isPerFaceNormalEnabled(mesh)) {
				using Scalar = typename FaceType::NormalType::ScalarType;
				int    a     = p.name - ply::nx;
				Scalar n     = internal::readProperty<Scalar>(file, p.type);
				hasBeenRead  = true;
				// in case the loaded polygon has been triangulated in the last n triangles of mesh
				for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff) {
					mesh.face(ff).normal()[a] = n;
				}
			}
		}
	}
	if (p.name >= ply::red && p.name <= ply::alpha) { // loading one of the color components
		if constexpr (vcl::hasPerFaceColor<MeshType>()) {
			if (vcl::isPerFaceColorEnabled(mesh)) {
				int           a = p.name - ply::red;
				unsigned char c = internal::readProperty<unsigned char>(file, p.type);
				hasBeenRead     = true;
				// in case the loaded polygon has been triangulated in the last n triangles of mesh
				for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff) {
					mesh.face(ff).color()[a] = c;
				}
			}
		}
	}
	if (p.name == ply::scalar) { // loading the scalar component
		if constexpr (vcl::hasPerFaceScalar<MeshType>()) {
			using Scalar = typename FaceType::ScalarType;
			if (vcl::isPerFaceScalarEnabled(mesh)) {
				Scalar s    = internal::readProperty<Scalar>(file, p.type);
				hasBeenRead = true;
				// in case the loaded polygon has been triangulated in the last n triangles of mesh
				for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff) {
					mesh.face(ff).scalar() = s;
				}
			}
		}
	}
	// if nothing has been read, it means that there is some data we don't know
	// we still need to read and discard what we read
	if (!hasBeenRead) {
		if (p.list) {
			uint s = internal::readProperty<int>(file, p.listSizeType);
			for (uint i = 0; i < s; ++i)
				internal::readProperty<int>(file, p.type);
		}
		else {
			internal::readProperty<int>(file, p.type);
		}
	}
}

template<typename MeshType>
void loadFacesTxt(std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
	using FaceType       = typename MeshType::FaceType;

	mesh.reserveFaces(header.numberFaces());
	for (uint fid = 0; fid < header.numberFaces(); ++fid) {
		vcl::Tokenizer spaceTokenizer = vcl::io::internal::nextNonEmptyTokenizedLine(file);
		vcl::Tokenizer::iterator token = spaceTokenizer.begin();
		mesh.addFace();
		FaceType& f = mesh.face(mesh.faceNumber() - 1);
		for (ply::Property p : header.faceProperties()) {
			if (token == spaceTokenizer.end()) {
				throw vcl::MalformedFileException("Unexpected end of line.");
			}
			loadFaceProperty(token, mesh, f, p);
		}
	}
}

template<typename MeshType>
void loadFacesBin(std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
	using FaceType = typename MeshType::FaceType;
	mesh.reserveFaces(header.numberFaces());
	for (uint fid = 0; fid < header.numberFaces(); ++fid) {
		uint      ffid = mesh.addFace();
		FaceType& f    = mesh.face(ffid);
		for (ply::Property p : header.faceProperties()) {
			loadFaceProperty(file, mesh, f, p);
		}
	}
}

} // namespace internal

template<typename MeshType>
void saveFaces(std::ofstream& file, const PlyHeader& header, const MeshType mesh)
{
	using FaceType = typename MeshType::FaceType;

	bool bin = header.format() == ply::BINARY;

	// indices of vertices that do not consider deleted vertices
	std::vector<int> vIndices = mesh.vertexCompactIndices();

	for (const FaceType& f : mesh.faces()) {
		for (ply::Property p : header.faceProperties()) {
			bool hasBeenWritten = false;
			if (p.name == ply::vertex_indices) {
				internal::saveFaceIndices(file, p, mesh, vIndices, f, bin);
				hasBeenWritten = true;
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (vcl::hasPerFaceNormal<MeshType>()) {
					if (vcl::isPerFaceNormalEnabled(mesh)) {
						internal::writeProperty(file, f.normal()[p.name - ply::nx], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (vcl::hasPerFaceColor<MeshType>()) {
					if (vcl::isPerFaceColorEnabled(mesh)) {
						internal::writeProperty(file, f.color()[p.name - ply::red], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name == ply::scalar) {
				if constexpr (vcl::hasPerFaceScalar<MeshType>()) {
					if (vcl::isPerFaceScalarEnabled(mesh)) {
						internal::writeProperty(file, f.scalar(), p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name == ply::texcoord) {
				if constexpr (vcl::hasPerFaceWedgeTexCoords<MeshType>()) {
					if (vcl::isPerFaceWedgeTexCoordsEnabled(mesh)) {
						internal::writeProperty(file, f.vertexNumber() * 2, p.listSizeType, bin);
						for (const auto& tc : f.wedgeTexCoords()) {
							internal::writeProperty(file, tc.u(), p.type, bin);
							internal::writeProperty(file, tc.v(), p.type, bin);
						}
						hasBeenWritten = true;
					}
				}
			}
			if (p.name == ply::texnumber) {
				if constexpr (vcl::hasPerFaceWedgeTexCoords<MeshType>()) {
					if (vcl::isPerFaceWedgeTexCoordsEnabled(mesh)) {
						internal::writeProperty(file, f.wedgeTexCoord(0).nTexture(), p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (!hasBeenWritten) {
				// be sure to write something if the header declares some property that is not
				// in the mesh
				internal::writeProperty(file, 0, p.type, bin);
			}
		}
		if (!bin)
			file << std::endl;
	}
}

template<typename MeshType>
void loadFaces(std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
	if (header.format() == ply::ASCII) {
		internal::loadFacesTxt(file, header, mesh);
	}
	else {
		internal::loadFacesBin(file, header, mesh);
	}
}

} // namespace vcl::ply
