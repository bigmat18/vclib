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

#ifndef VCL_TYPE_TRAITS_H
#define VCL_TYPE_TRAITS_H

#include <assert.h>
#include <type_traits>

/**
 * Utility macro used to enable functions only when a particular static condition is true.
 *
 * Usage:
 *
 * @code{.cpp}
 * template <int N>
 * VCL_ENABLE_IF(N < 0, int) getSize() { ... }
 * @endcode
 *
 * The first argument is the condition, the second argument is the return type of the function.
 *
 * When building the documentation, this syntax will be hidden and will appear just the return type.
 */
#define VCL_ENABLE_IF(Test, Type1) typename std::enable_if<Test, Type1>::type

// used for templates given as parameters to macros
// https://stackoverflow.com/questions/44268316/passing-a-template-type-into-a-macro
#define VCL_COMMA ,

using uint = unsigned int;

namespace vcl {

/*
 * Utility type that makes possible to treat const pointers in a templated class that can treat a
 * both const and non-const pointer type.
 */

template<typename T>
struct MakeConstPointer
{
	typedef T type;
};

template<typename T>
struct MakeConstPointer<T*>
{
	typedef const T* type;
};

/*
 * Full deduction for the possibility to re-use same code for const and non-const member functions
 * https://stackoverflow.com/a/47369227/5851101
 */

template<typename T>
constexpr T& asConst(T const& value) noexcept
{
	return const_cast<T&>(value);
}
template<typename T>
constexpr T* asConst(T const* value) noexcept
{
	return const_cast<T*>(value);
}
template<typename T>
constexpr T* asConst(T* value) noexcept
{
	return value;
}
template<typename T>
void asConst(T const&&) = delete;

} // namespace vcl

#endif // VCL_TYPE_TRAITS_H
