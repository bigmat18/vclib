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

#ifndef VCL_RENDER_DRAWABLE_DRAWABLE_OBJECT_VECTOR_H
#define VCL_RENDER_DRAWABLE_DRAWABLE_OBJECT_VECTOR_H

#include <vclib/render/interfaces/drawable_object_i.h>

#include <vclib/space/box.h>
#include <vclib/space/polymorphic_object_vector.h>

namespace vcl {

class DrawableObjectVector : public PolymorphicObjectVector<DrawableObjectI>
{
    using Base = PolymorphicObjectVector<DrawableObjectI>;
public:
    DrawableObjectVector() = default;

    vcl::Box3d boundingBox(bool onlyVisible = true) const;

private:
    uint firstVisibleObject() const;
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_DRAWABLE_OBJECT_VECTOR_H
