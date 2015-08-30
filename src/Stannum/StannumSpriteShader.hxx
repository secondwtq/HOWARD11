//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-30
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_STANNUMSPRITESHADER_HXX
#define HOWARD11_STANNUMSPRITESHADER_HXX

#include "StannumCommon.hxx"

#include "Verdandi/GLShader.hxx"
#include "Verdandi/GLShaderExt.hxx"

namespace Howard {
namespace Stannum {

class SpriteShader : public Verdandi::gl_shader_ext {
public:

    ATTR_OBJECT(A, Stannum::VertFormatSprite);

    void init_shader();
    void attribute_attr(size_t sid);
    void disable_attributes();

    DEF_ATTRIBUTE(position);
    DEF_ATTRIBUTE(texcoord);
    DEF_ATTRIBUTE(location);
    DEF_ATTRIBUTE(multiply);

    DEF_UNIFORM(mvp);
    DEF_SAMPLER(texture_major);

};

}
}

#endif // HOWARD11_STANNUMSPRITESHADER_HXX
