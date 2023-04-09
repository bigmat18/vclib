/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_MISC_PARALLEL_H
#define VCL_MISC_PARALLEL_H

#include <vclib/types.h>

// Apple clang does not support c++17 parallel algorithms.
// To compensate this lack, waiting for Apple to support them, we use pstld
// (https://github.com/mikekazakov/pstld) that implements them in the stl namespace
#if defined(__clang__) && defined(__APPLE__)
    #if __has_include(<pstld/pstld.h>)
        #include <pstld/pstld.h>
    #else
        // inclusion for usage of vclib without cmake
        #define PSTLD_HEADER_ONLY // no prebuilt library, only the header
        #include "../../../external/pstld-master/pstld/pstld.h"
    #endif
#else
    // tbb and qt conflicts: if both are linked, we need to first undef Qt's emit
    // see: https://github.com/oneapi-src/oneTBB/issues/547
    #ifndef Q_MOC_RUN
        #if defined(emit)
            #undef emit
            #include <execution>
            #define emit // restore the macro definition of "emit", as it was defined in gtmetamacros.h
        #else
            #include <execution>
        #endif
    #endif
#endif

namespace vcl {

template<typename Iterator, typename Lambda>
void parallelFor(Iterator&& begin, Iterator&& end, Lambda&& F);

template<typename Iterator, typename Lambda>
void parallelFor(const Iterator& begin, const Iterator& end, Lambda&& F);

template<std::ranges::range Rng, typename Lambda>
void parallelFor(Rng&& r, Lambda&& F);

template<std::ranges::range Rng, typename Lambda>
void parallelFor(const Rng& r, Lambda&& F);

} // namespace vcl

#include "parallel.cpp"

#endif // VCL_MISC_PARALLEL_H
