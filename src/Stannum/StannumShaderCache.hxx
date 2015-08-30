//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-24
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_STANNUMSHADERCACHE_HXX
#define HOWARD11_STANNUMSHADERCACHE_HXX

#include "StannumCommon.hxx"

#include "Verdandi/GLShaderExt.hxx"

namespace Howard {

namespace Stannum {

class ShaderCache {

    public:

    void load_shaders();

    void destroy_shaders();

    template <typename T>
    T *get_shader(ShaderType type) {
        return reinterpret_cast<T *>(m_shaders[type]); }

    private:

    Verdandi::gl_shader_ext *m_shaders[ShaderType::End];

};

}

}

#endif // HOWARD11_STANNUMSHADERCACHE_HXX
