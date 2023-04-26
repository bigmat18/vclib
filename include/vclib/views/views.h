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

#ifndef VCL_VIEWS_VIEWS_H
#define VCL_VIEWS_VIEWS_H

#include <ranges>

namespace vcl::views{

namespace internal {

struct NotNullView
{
	constexpr NotNullView() = default;

#ifdef VCLIB_USES_RANGES
	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, NotNullView)
	{
		return std::views::filter(r, [](auto* p) { return p != nullptr; });
	}
#endif
};

struct DereferenceView
{
	constexpr DereferenceView() = default;

#ifdef VCLIB_USES_RANGES
	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, DereferenceView)
	{
		return std::views::transform(r, [](auto p) { return *p; });
	}
#endif
};

struct ReferenceView
{
	constexpr ReferenceView() = default;

#ifdef VCLIB_USES_RANGES
	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, ReferenceView)
	{
		return std::views::transform(r, [](auto& o) { return &o; });
	}
#endif
};

struct ConstReferenceView
{
	constexpr ConstReferenceView() = default;

#ifdef VCLIB_USES_RANGES
	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, ConstReferenceView)
	{
		return std::views::transform(r, [](const auto& o) { return &o; });
	}
#endif
};

} // namespace vcl::views::internal

/**
 * @brief The notNull view allows to filter the pointers of a range. The resulting
 * view will iterate only on the pointers that are not `nullptr`.
 */
inline constexpr internal::NotNullView notNull;

/**
 * @brief The dereference view allow to dereference the pointers of a range. The resulting
 * view will iterate over the objects pointed by the range of pointers.
 *
 * @note: no check on the validity of the pointers is performed. If you know that in your range
 * there is the possibility to have `nullptr` pointers, use first the `notNull` view:
 *
 * auto resView = inputRange | notNull | dereference;
 */
inline constexpr internal::DereferenceView dereference;

/**
 * @brief The reference view allow to reference the objects of a range. The resulting view will
 * iterate over the pointers pointing to the object of the input range.
 */
inline constexpr internal::ReferenceView reference;

/**
 * @brief The constReference view allow to const reference the objects of a range. The resulting
 * view will iterate over the const pointers pointing to the object of the input range (that may be
 * also not const).
 */
inline constexpr internal::ConstReferenceView constReference;

} // namespace vcl::views

#endif // VCL_VIEWS_VIEWS_H