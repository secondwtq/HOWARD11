//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/30.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_GLVERTEXBUFFEREXT_HXX
#define HOWARD11_GLVERTEXBUFFEREXT_HXX

#include "GLCommon.hxx"

#include <array>

namespace Howard {
namespace Verdandi {

template <size_t N>
class VertexAttributeArrayScope {
public:

    VertexAttributeArrayScope(const std::array<VGLIDX, N>& locations)
        : m_locations(locations) {
        for (auto i : m_locations) {
            glEnableVertexAttribArray(i); }
    }

    ~VertexAttributeArrayScope() {
        for (auto i : m_locations) {
            glDisableVertexAttribArray(i); }
    }

private:
    std::array<VGLIDX, N> m_locations;
};

}
}

#endif // HOWARD11_GLVERTEXBUFFEREXT_HXX
