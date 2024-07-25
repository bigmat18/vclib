/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_CONCEPTS_SERIALIZATION_H
#define VCL_CONCEPTS_SERIALIZATION_H

#include <istream>
#include <ostream>

namespace vcl {

/**
 * @brief Concept that is evaluated true if T is an output streamable type.
 * 
 * A type T is output streamable if it can be written to an output stream, i.e.,
 * it has an overloaded operator<<.
 * 
 * @ingroup util_concepts
 */
template<typename T>
concept OutputStreamable = requires (std::ostream& os, T value) {
    // clang-format off
    { os << value } -> std::convertible_to<std::ostream&>;
    // clang-format on
};

/**
 * @brief Concept that is evaluated true if T is an input streamable type.
 * 
 * A type T is input streamable if it can be read from an input stream, i.e.,
 * it has an overloaded operator>>.
 * 
 * @ingroup util_concepts
 */
template<typename T>
concept InputStreamable = requires (std::istream& is, T& value) {
    // clang-format off
    { is >> value } -> std::convertible_to<std::istream&>;
    // clang-format on
};

/**
 * @brief Concept that is evaluated true if T is serializable.
 * 
 * A type T is serializable if it can be written to an output stream and read
 * from an input stream, through the methods `serialize` and `deserialize`.
 * 
 * @ingroup util_concepts
 */
template<typename T>
concept Serializable =
    requires (T& o, const T& co, std::ostream& os, std::istream& is) {
        // clang-format off
    { co.serialize(os) } -> std::same_as<void>;
    { o.deserialize(is) } -> std::same_as<void>;
        // clang-format on
    };

} // namespace vcl

#endif // VCL_CONCEPTS_SERIALIZATION_H
