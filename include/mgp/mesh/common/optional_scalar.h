/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef  MGP_MESH_COMMON_OPTIONAL_SCALAR_H
#define  MGP_MESH_COMMON_OPTIONAL_SCALAR_H

#include "optional_info.h"

namespace mgp::common {

template <typename T>
class OptionalScalarTrigger {};

template<typename S, typename T>
class OptionalScalar : public OptionalScalarTrigger<T>, public virtual OptionalInfo<T>
{
private:
	typedef OptionalInfo<T> B;
public:
	typedef S ScalarType;
	const ScalarType& scalar() const { return B::containerPointer->scalar(B::id()); }
	ScalarType&       scalar() { return B::containerPointer->scalar(B::id()); }
	
	void __optional_scalar__() {} // dummy member used just for detection of an OpionalScalar object
};

template<typename T>
class OptionalScalarf : public OptionalScalar<float, T> {};

template<typename T>
class OptionalScalard : public OptionalScalar<double, T> {};

/**
 * Detector to check if a class has (inherits) OpionalScalar
 */

template <typename T>
using hasOptionalScalarT = std::is_base_of<OptionalScalarTrigger<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalScalar = typename std::enable_if<hasOptionalScalarT<U>::value, T>::type;

template <typename T>
bool constexpr hasOptionalScalar() {return hasOptionalScalarT<T>::value;}

} // namespace mgp::common

#endif //  MGP_MESH_COMMON_OPTIONAL_SCALAR_H
