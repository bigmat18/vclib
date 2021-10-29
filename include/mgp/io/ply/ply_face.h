/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_IO_PLY_FACE_H
#define MGP_IO_PLY_FACE_H

#include "ply_header.h"
#include <fstream>
#include <mgp/misc/tokenizer.h>

namespace mgp {
namespace ply {

namespace internal {

template<typename A, typename D>
void saveFaceIndices(
	std::ofstream&          file,
	Property                p,
	uint                    f,
	uint&                   startingIndex,
	const A                 faces[],
	const io::FileMeshInfo& meshType,
	const D                 polygonSizes[],
	bool                    bin);

template<template<typename... Args> class Container, typename A, typename D>
bool loadFaceIndicesTxt(
	const mgp::Tokenizer&     spaceTokenizer,
	mgp::Tokenizer::iterator& token,
	Property                  p,
	uint                      f,
	Container<A>&             faces,
	D                         polygonSizes[]);

template<typename A, typename B, typename C, typename D>
bool loadFacesTxt(
	std::ifstream&              file,
	const PlyHeader&            header,
	A&                          faces, // container with push_back method
	io::FileMeshInfo::MeshType& meshType,
	B                           faceNormals[],
	io::FileMeshInfo::ColorMode colorMod,
	C                           faceColors[],
	D                           polygonSizes[]);

template<template<typename... Args> class Container, typename A, typename D>
bool loadFaceIndicesBin(
	std::ifstream& file,
	Property       p,
	uint           f,
	Container<A>&  faces,
	D              polygonSizes[]);

template<typename A, typename B, typename C, typename D>
bool loadFacesBin(
	std::ifstream&              file,
	const PlyHeader&            header,
	A&                          faces, // container with push_back method
	io::FileMeshInfo::MeshType& meshType,
	B                           faceNormals[],
	io::FileMeshInfo::ColorMode colorMod,
	C                           faceColors[],
	D                           polygonSizes[]);

} // namespace internal

template<typename A, typename B, typename C, typename D>
void saveFaces(
	std::ofstream&              file,
	const PlyHeader&            header,
	const A                     faces[],
	io::FileMeshInfo            meshMode,
	const B                     faceNormals[],
	io::FileMeshInfo::ColorMode colorMod,
	const C                     faceColors[],
	const D                     polygonSizes[]);

template<typename A, typename B, typename C, typename D>
bool loadFaces(
	std::ifstream&              file,
	const PlyHeader&            header,
	A&                          faces,
	io::FileMeshInfo::MeshType& meshType,
	B                           faceNormals[],
	io::FileMeshInfo::ColorMode colorMod,
	C                           faceColors[],
	D                           polygonSizes[]);

} // namespace ply
} // namespace mgp

#include "ply_face.cpp"

#endif // MGP_IO_PLY_FACE_H