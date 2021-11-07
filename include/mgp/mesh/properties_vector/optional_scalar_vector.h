/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_PROPERTIES_VECTOR_OPTIONAL_SCALAR_VECTOR_H
#define MGP_MESH_PROPERTIES_VECTOR_OPTIONAL_SCALAR_VECTOR_H

#include <mgp/mesh/properties_optional/optional_scalar.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

template<typename, typename = void>
class OptionalScalarVector
{
public:
	void clear() {}
	void reserve(unsigned int) {}
	void resize(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalScalarVector<T, std::enable_if_t<prop::hasOptionalScalar<T>()>> :
		private OptionalGenericVector<typename T::ScalarType>
{
	using ScalarType = typename T::ScalarType;
	using Base = OptionalGenericVector<ScalarType>;

public:
	using Base::clear;
	using Base::reserve;
	using Base::resize;
	using Base::compact;
	bool              isScalarEnabled() const { return Base::isEnabled(); };
	void              enableScalar(unsigned int size) { Base::enable(size); }
	void              disableScalar() { Base::disable(); }
	ScalarType&       scalar(unsigned int i) { return Base::at(i); }
	const ScalarType& scalar(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::internal

#endif // MGP_MESH_PROPERTIES_VECTOR_OPTIONAL_SCALAR_VECTOR_H