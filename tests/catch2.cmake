#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2025                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the Mozilla Public License Version 2.0 as published *
#* by the Mozilla Foundation; either version 2 of the License, or            *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* Mozilla Public License Version 2.0                                        *
#* (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
#****************************************************************************/

find_package(Catch2 3 QUIET)

if(VCLIB_ALLOW_SYSTEM_CATCH2 AND TARGET Catch2::Catch2WithMain)
    message(STATUS "- Catch2 - using system-provided library")
elseif (VCLIB_ALLOW_DOWNLOAD_CATCH2)
    message(STATUS "- Catch2 - using downloaded source")

    FetchContent_Declare(Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.3.2)

    FetchContent_MakeAvailable(Catch2)
else()
    message(
        FATAL_ERROR
        "Catch2 is required to build tests - VCLIB_ALLOW_DOWNLOAD_CATCH2 must be enabled and found.")
endif()

target_link_libraries(vclib-tests-examples-common INTERFACE Catch2::Catch2WithMain)

