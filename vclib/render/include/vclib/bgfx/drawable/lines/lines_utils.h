#pragma once
#include <iostream>
#include <stdint.h>
#include <cmath>

namespace vcl::lines {

    enum LinesTypes {
        CPU_GENERATED,      // vertex buffer generated by CPU
        GPU_GENERATED,      // vertex buffer generated by GPU using compute shader
        INSTANCING_BASED,   // generated lines by instancing buffer
        INDIRECT_BASED,     // instancing with indirect buffer filled with compute 
        TEXTURE_BASED,      // instancing with indirect usning text buffer
    };

    struct LinesVertex {
        float X, Y, Z;
        float color;
        float xN, yN, zN;

        LinesVertex(float x, float y, float z, 
                    uint32_t color = COLOR(1, 1, 1, 1), 
                    float xn = 0, float yn = 0, float zn = 0) :
            X(x), Y(y), Z(z),
            color(std::bit_cast<float>(color)),
            xN(xn), yN(yn), zN(zn) {}


        uint32_t getUintColor() const {
            return std::bit_cast<uint32_t>(color);
        }

        float getReverseColor() const {
            uint32_t int_color = getUintColor();

            return std::bit_cast<float>(
                ((int_color & 0xFF000000) >> 24) | 
                ((int_color & 0x00FF0000) >> 8)  |
                ((int_color & 0x0000FF00) << 8)  |
                ((int_color & 0x000000FF) << 24)
            );
        }

        static inline uint32_t COLOR(float r, float g, float b, float a) {
            return static_cast<uint32_t>(
                    (static_cast<uint8_t>(std::round((r) * 255)) << 24) |
                    (static_cast<uint8_t>(std::round((g) * 255)) << 16) |
                    (static_cast<uint8_t>(std::round((b) * 255)) << 8)  | 
                    (static_cast<uint8_t>(std::round((a) * 255)))
                );
        }
    };
}