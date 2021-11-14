/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_PLY_HEADER_H
#define MGP_PLY_HEADER_H

#include <vector>
#include <string>
#include <fstream>

#include "../file_mesh_info.h"
#include "ply.h"

namespace mgp {

class Tokenizer;

namespace ply {

/**
 * @brief The PlyHeader class allows to manage the header of a ply file.
 *
 * Following the content of an header, a ply file can be loaded or saved.
 */
class PlyHeader
{
public:
	PlyHeader();
	PlyHeader(ply::Format f, const ply::Element& vElement, const ply::Element fElement);
	PlyHeader(ply::Format f, const ply::Element& vElement, const ply::Element fElement, const ply::Element eElement);
	PlyHeader(ply::Format f, const io::FileMeshInfo& info);
	PlyHeader(std::ifstream& file);

	void clear();
	bool isValid() const;

	ply::Format format() const;

	io::FileMeshInfo getInfo() const;

	bool hasVertices() const;
	bool hasFaces() const;
	bool hasEdges() const;

	unsigned int numberVertices() const;
	unsigned int numberFaces() const;
	unsigned int numberEdges() const;

	const std::list<ply::Property>& vertexProperties() const;
	const std::list<ply::Property>& faceProperties() const;
	const std::list<ply::Property>& edgeProperties() const;

	bool errorWhileLoading() const;

	void setNumberVertices(unsigned long int nV);
	void setNumberFaces(unsigned long int nF);
	void setNumberEdges(unsigned long int nE);

	void setInfo(const io::FileMeshInfo& info, bool binary = true);

	std::string toString() const;

	void addElement(const ply::Element& e);
	void setFormat(ply::Format f);

	typedef std::vector<ply::Element>::const_iterator iterator;

	iterator begin() const;
	iterator end() const;


private:
	ply::Element readElement(const mgp::Tokenizer& lineTokenizer) const;
	ply::Property readProperty(const mgp::Tokenizer& lineTokenizer) const;
	ply::PropertyName stringToName(const std::string &name) const;
	ply::PropertyType stringToType(const std::string &type) const;
	std::string nameToString(ply::PropertyName n) const;
	std::string typeToString(ply::PropertyType t) const;

	ply::Format _format;
	std::vector<ply::Element> elements;
	bool valid;
	int v, f, e; // say which is the id in the elements vector for vertices, faces and edges
	int nextElementID = 0;
};

} //namespace mgp::ply
} //namespace mgp

#include "ply_header.cpp"

#endif // MGP_PLY_HEADER_H
