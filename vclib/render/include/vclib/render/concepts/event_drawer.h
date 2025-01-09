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

#ifndef CONCEPTS_EVENT_DRAWER_H
#define CONCEPTS_EVENT_DRAWER_H

#include "drawer.h"

#include <vclib/render/input.h>

namespace vcl {

template<typename T>
concept CantBlockEventDrawerConcept = DrawerConcept<T> && requires (T&& obj) {
    requires RemoveRef<T>::CAN_BLOCK_EVENT_PROPAGATION == false;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.onKeyPress(Key::Enum(), KeyModifiers()) } -> std::same_as<void>;
        { obj.onKeyRelease(Key::Enum(), KeyModifiers()) } -> std::same_as<void>;
        {
            obj.onMouseMove(double(), double(), KeyModifiers())
        } -> std::same_as<void>;
        {
            obj.onMousePress(
                MouseButton::Enum(), double(), double(), KeyModifiers())
        } -> std::same_as<void>;
        {
            obj.onMouseRelease(
                MouseButton::Enum(), double(), double(), KeyModifiers())
        } -> std::same_as<void>;
        {
            obj.onMouseDoubleClick(
                MouseButton::Enum(), double(), double(), KeyModifiers())
        } -> std::same_as<void>;
        {
            obj.onMouseScroll(double(), double(), KeyModifiers())
        } -> std::same_as<void>;
    };
};

template<typename T>
concept CanBlockEventDrawerConcept = DrawerConcept<T> && requires (T&& obj) {
    requires RemoveRef<T>::CAN_BLOCK_EVENT_PROPAGATION == true;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.onKeyPress(Key::Enum(), KeyModifiers()) } -> std::same_as<bool>;
        { obj.onKeyRelease(Key::Enum(), KeyModifiers()) } -> std::same_as<bool>;
        {
            obj.onMouseMove(double(), double(), KeyModifiers())
        } -> std::same_as<bool>;
        {
            obj.onMousePress(
                MouseButton::Enum(), double(), double(), KeyModifiers())
        } -> std::same_as<bool>;
        {
            obj.onMouseRelease(
                MouseButton::Enum(), double(), double(), KeyModifiers())
        } -> std::same_as<bool>;
        {
            obj.onMouseDoubleClick(
                MouseButton::Enum(), double(), double(), KeyModifiers())
        } -> std::same_as<bool>;
        {
            obj.onMouseScroll(double(), double(), KeyModifiers())
        } -> std::same_as<bool>;
    };
};

template<typename T>
concept EventDrawerConcept =
    CanBlockEventDrawerConcept<T> || CantBlockEventDrawerConcept<T>;

} // namespace vcl

#endif // CONCEPTS_EVENT_DRAWER_H
