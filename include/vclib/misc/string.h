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

#ifndef VCL_STRING_H
#define VCL_STRING_H

#include <string>

namespace vcl::str {

bool containsCaseInsensitive(const std::string& input, const std::string& substr);

std::string::const_iterator
findCaseInsensitive(const std::string& input, const std::string& substr);

std::string toLower(const std::string& s);

std::string toUpper(const std::string& s);

void removeWindowsNewLine(std::string& s);

} // namespace vcl::str

#include "string.cpp"

#endif // VCL_STRING_H