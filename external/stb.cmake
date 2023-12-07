#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2023                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the GNU General Public License as published by      *
#* the Free Software Foundation; either version 3 of the License, or         *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
#* for more details.                                                         *
#****************************************************************************/

set(VCLIB_STB_DIR ${CMAKE_CURRENT_LIST_DIR}/stb-master)

if (VCLIB_ALLOW_BUNDLED_STB AND EXISTS ${VCLIB_STB_DIR}/stb/stb_image.h)
    message(STATUS "- STB - using bundled source")

    set(STB_INCLUDE_DIRS ${VCLIB_STB_DIR})

    add_library(vclib-external-stb SHARED src/stb_src.cpp)

    target_include_directories(vclib-external-stb PUBLIC ${STB_INCLUDE_DIRS})

    list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-stb)
endif()
