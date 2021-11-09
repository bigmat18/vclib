/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_PROPERTIES_VECTOR_OPTIONAL_ADJACENT_VERTEX_REF_VECTOR_H
#define MGP_MESH_PROPERTIES_VECTOR_OPTIONAL_ADJACENT_VERTEX_REF_VECTOR_H

#include <mgp/mesh/properties_optional/optional_vertex_references.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

template<typename, typename = void>
class OptionalVertexReferencesVector
{
public:
	void clear() {}
	void resize(unsigned int) {}
	void reserve(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalVertexReferencesVector<
	T,
	std::enable_if_t<prop::hasOptionalVertexReferences<T>()>> :
		private OptionalGenericVector<typename T::VertRefsContainer>
{
private:
	using VertRefsContainer = typename T::VertRefsContainer;
	using Base = OptionalGenericVector<VertRefsContainer>;

public:
	using Base::clear;
	using Base::reserve;
	using Base::resize;
	using Base::compact;
	bool             isVertexReferencesEnabled() const { return Base::isEnabled(); };
	void             enableVertexReferences(unsigned int size) { Base::enable(size); }
	void             disableVertexReferences() { Base::disable(); }
	VertRefsContainer&       vertRefs(unsigned int i) { return Base::at(i); }
	const VertRefsContainer& vertRefs(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::internal

#endif // MGP_MESH_PROPERTIES_VECTOR_OPTIONAL_ADJACENT_VERTEX_REF_VECTOR_H