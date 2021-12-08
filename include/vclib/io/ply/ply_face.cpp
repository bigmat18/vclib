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
#include <vclib/mesh/requirements.h>
#include <vclib/exception/io_exception.h>

namespace vcl::ply {

namespace internal {

template<typename MeshType, typename FaceType>
void saveFaceIndices(
	std::ofstream&  file,
	Property        p,
	const MeshType& m,
	const std::vector<int>& vIndices,
	const FaceType& f,
	bool            bin)
{
	using VertexType = typename MeshType::VertexType;

	uint fsize = f.vertexNumber();
	internal::writeProperty(file, fsize, p.listSizeType, bin);
	for (const VertexType* v : f.vertices()){
		internal::writeProperty(file, vIndices[m.index(v)], p.type, bin);
	}
}

template<typename MeshType, typename FaceType>
void setFaceIndices(
	FaceType& f,
	MeshType& m,
	const std::vector<uint>& vids)
{
	bool splitFace = false;
	// we have a polygonal mesh
	if constexpr(FaceType::VERTEX_NUMBER < 0){
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
				throw vcl::MalformedFileException(
					"Bad vertex index for face " + std::to_string(i));
			}
			f.vertex(i) = &m.vertex(vids[i]);
		}
	}
	else { // split needed
		using VertexType = typename FaceType::VertexType;
		using CoordType = typename VertexType::CoordType;

		// put the vertices of the loaded polygon in a vector
		std::vector<CoordType> pol(vids.size());
		for (uint i = 0; i < vids.size(); ++i){
			if (vids[i] >= m.vertexNumber()) {
				throw vcl::MalformedFileException(
					"Bad vertex index for face " + std::to_string(i));
			}
			pol[i] = m.vertex(vids[i]).coord();
		}
		// triangulate the polygon
		std::vector<uint> tris = vcl::earCut(pol);
		// set the first triangle of the loaded polygon
		f.vertex(0) = &m.vertex(vids[tris[0]]);
		f.vertex(1) = &m.vertex(vids[tris[1]]);
		f.vertex(2) = &m.vertex(vids[tris[2]]);
		// remaining triangles, need to create more faces in the mesh
		for (uint i = 3; i < tris.size(); i+=3){
			uint ff = m.addFace();
			m.face(ff).vertex(0) = &m.vertex(vids[tris[i]]);
			m.face(ff).vertex(1) = &m.vertex(vids[tris[i+1]]);
			m.face(ff).vertex(2) = &m.vertex(vids[tris[i+2]]);
		}
	}
}

template<typename MeshType, typename FaceType, typename Stream>
void loadFaceProperty(
	Stream& file,
	MeshType& mesh,
	FaceType& f,
	ply::Property p
	)
{
	bool hasBeenRead = false;
	if (p.name == ply::vertex_indices) {
		uint fSize = internal::readProperty<uint>(file, p.listSizeType);
		std::vector<uint> vids(fSize);
		for (uint i = 0; i < fSize; ++i){
			vids[i] = internal::readProperty<size_t>(file, p.type);
		}
		hasBeenRead = true;
		// will manage the case of loading a polygon in a triangle mesh
		setFaceIndices(f, mesh, vids);
	}
	if (p.name == ply::texcoord) {
		if constexpr (vcl::hasPerFaceWedgeTexCoords<MeshType>()) {
			if (vcl::isPerFaceWedgeTexCoordsEnabled(mesh)) {
				using Scalar = typename FaceType::WedgeTexCoordType::ScalarType;
				uint uvSize = internal::readProperty<uint>(file, p.listSizeType);
				uint fSize = uvSize/2;
				bool splitFace = false;
				// in case of reading texcoords before vertex indices
				if constexpr(FaceType::VERTEX_NUMBER < 0){
					f.resizeVertices(fSize);
				}
				else if (FaceType::VERTEX_NUMBER != fSize)
					splitFace = true;
				if (!splitFace) {
					for (uint i = 0; i < fSize; ++i) {
						assert(token != spaceTokenizer.end());
						Scalar u = internal::readProperty<Scalar>(file, p.type);
						Scalar v = internal::readProperty<Scalar>(file, p.type);
						f.wedgeTexCoord(i).u() = u;
						f.wedgeTexCoord(i).v() = v;
					}
					hasBeenRead = true;
				}
				else { /// @todo manage case when split polygonal face
					throw std::runtime_error("Cannot handle this file for this mesh type.");
				}
			}
		}
	}
	if (p.name == ply::texnumber) {
		if constexpr (vcl::hasPerFaceWedgeTexCoords<MeshType>()) {
			if (vcl::isPerFaceWedgeTexCoordsEnabled(mesh)) {
				uint n = internal::readProperty<uint>(file, p.type);
				hasBeenRead = true;
				for (uint i = 0; i < f.vertexNumber(); ++i){
					f.wedgeTexCoord(i).nTexture() = n;
				}
			}
		}
	}
	if (p.name >= ply::nx && p.name <= ply::nz) {
		if constexpr (vcl::hasPerFaceNormal<MeshType>()) {
			if (vcl::isPerFaceNormalEnabled(mesh)) {
				using Scalar = typename FaceType::NormalType::ScalarType;
				int a = p.name - ply::nx;
				Scalar n = internal::readProperty<Scalar>(file, p.type);
				hasBeenRead = true;
				// in case the loaded polygon has been triangulated in the last n triangles of mesh
				for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff){
					mesh.face(ff).normal()[a] = n;
				}
			}
		}
	}
	if (p.name >= ply::red && p.name <= ply::alpha) {
		if constexpr (vcl::hasPerFaceColor<MeshType>()) {
			if (vcl::isPerFaceColorEnabled(mesh)) {
				int a = p.name - ply::red;
				unsigned char c = internal::readProperty<unsigned char>(file, p.type);
				hasBeenRead = true;
				// in case the loaded polygon has been triangulated in the last n triangles of mesh
				for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff){
					mesh.face(ff).color()[a] = c;
				}
			}
		}
	}
	if (p.name == ply::scalar) {
		if constexpr (vcl::hasPerFaceScalar<MeshType>()) {
			using Scalar = typename FaceType::ScalarType;
			if (vcl::isPerFaceScalarEnabled(mesh)) {
				Scalar s = internal::readProperty<Scalar>(file, p.type);
				hasBeenRead = true;
				// in case the loaded polygon has been triangulated in the last n triangles of mesh
				for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff){
					mesh.face(ff).scalar() = s;
				}
			}
		}
	}
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
void loadFacesTxt(
	std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
	using FaceType = typename MeshType::FaceType;
	bool error     = false;
	vcl::Tokenizer spaceTokenizer;

	error = !internal::nextLine(file, spaceTokenizer);

	vcl::Tokenizer::iterator token = spaceTokenizer.begin();

	mesh.reserveFaces(header.numberFaces());
	for (uint fid = 0; fid < header.numberFaces(); ++fid) {
		mesh.addFace();
		FaceType& f = mesh.face(mesh.faceNumber() -1);
		for (ply::Property p : header.faceProperties()) {
			if (token == spaceTokenizer.end()) {
				error = !nextLine(file, spaceTokenizer);
				token = spaceTokenizer.begin();
			}
			if (error)
				throw std::runtime_error("Malformed file");
			loadFaceProperty(token, mesh, f, p);
		}
	}
}

template<typename MeshType>
void loadFacesBin(
	std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
	using FaceType = typename MeshType::FaceType;
	mesh.reserveFaces(header.numberFaces());
	for (uint fid = 0; fid < header.numberFaces(); ++fid) {
		uint ffid = mesh.addFace();
		FaceType& f = mesh.face(ffid);
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

	for (const FaceType& f : mesh.faces()){
		for (ply::Property p : header.faceProperties()) {
			bool hasBeenWritten = false;
			if (p.name == ply::vertex_indices){
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
						internal::writeProperty(file, f.vertexNumber()*2, p.listSizeType, bin);
						for (const auto& tc : f.wedgeTexCoords()){
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
			if (!hasBeenWritten){
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

} // namespace vcl
