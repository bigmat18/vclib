/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_references.h"

namespace mgp::components {

template<class Face, int N>
FaceReferences<Face, N>::FaceReferences()
{
	if constexpr (VARIANT_ID == 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		refs = std::array<Face*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		refs = std::vector<Face*>();
	}
}

template<class Face, int N>
unsigned int FaceReferences<Face, N>::faceNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return std::get<VARIANT_ID>(refs).size();
	}
}

template<class Face, int N>
unsigned int FaceReferences<Face, N>::sizeMod(unsigned int i) const
{
	return i % faceNumber();
}

template<class Face, int N>
Face*& FaceReferences<Face, N>::f(unsigned int i)
{
	assert(i < vertexNumber());
	return std::get<VARIANT_ID>(refs)[i];
}

template<class Face, int N>
const Face* FaceReferences<Face, N>::f(unsigned int i) const
{
	assert(i < vertexNumber());
	return std::get<VARIANT_ID>(refs)[i];
}

template<class Face, int N>
void FaceReferences<Face, N>::setFace(Face* f, unsigned int i)
{
	assert(i < vertexNumber());
	std::get<VARIANT_ID>(refs)[i] = f;
}

template<class Face, int N>
void FaceReferences<Face, N>::setFaces(const std::vector<Face*>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		unsigned int i = 0;
		for (const auto& f : list) {
			setFace(f, i);
			++i;
		}
	}
	else {
		std::get<VARIANT_ID>(refs) = list;
	}
}

template<class Face, int N>
template<int U>
internal::ReturnIfIsVector<U, void> FaceReferences<Face, N>::pushFace(Face* f)
{
	std::get<VARIANT_ID>(refs).push_back(f);
}

template<class Face, int N>
template<int U>
internal::ReturnIfIsVector<U, void>
FaceReferences<Face, N>::insertFace(unsigned int i, Face* f)
{
	assert(i < vertexNumber());
	std::get<VARIANT_ID>(refs).insert(std::get<VARIANT_ID>(refs).begin() + i, f);
}

template<class Face, int N>
template<int U>
internal::ReturnIfIsVector<U, void> FaceReferences<Face, N>::eraseFace(unsigned int i)
{
	assert(i < vertexNumber());
	std::get<VARIANT_ID>(refs).erase(std::get<VARIANT_ID>(refs).begin() + i);
}

template<class Face, int N>
void FaceReferences<Face, N>::updateFaceReferences(
	const Face* oldBase,
	const Face* newBase)
{
	for (unsigned int j = 0; j < faceNumber(); ++j) {
		if (f(j) != nullptr) {
			size_t diff = f(j) - oldBase;
			f(j)        = (Face*) newBase + diff;
		}
	}
}

} // namespace mgp::components