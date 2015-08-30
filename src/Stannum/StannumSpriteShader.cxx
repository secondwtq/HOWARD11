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

#include "StannumSpriteShader.hxx"

#include "Verdandi/GLShader.hxx"
#include "Verdandi/GLShaderExt.hxx"
#include "Verdandi/GLShaderExtDef.hxx"

#include "Debug.hxx"

namespace Howard {
namespace Stannum {

void SpriteShader::init_shader() {
    DEF_ATTRIBUTE_NT(position);
    DEF_ATTRIBUTE_NT(normal);
    DEF_ATTRIBUTE_NT(texcoord);
    DEF_ATTRIBUTE_NT(blendweights);
    DEF_ATTRIBUTE_NT(texindexes);

    DEF_UNIFORM(mvp);
    DEF_SAMPLER(texture_major);
}

void SpriteShader::attribute_attr(size_t sid) {
    switch (sid) {
        case 0: {
            SET_ATTRIBUTE3_NTX(position, 0, A, 0);
            SET_ATTRIBUTE3_NTX(texcoord, 1, A, 3);
            SET_ATTRIBUTE2_NTX(location, 2, A, 6);
            SET_ATTRIBUTE4_NTX(multiply, 3, A, 8);
        }
            break;
        default:
            ASSERT_FOUNDATION();
    }
}

void SpriteShader::disable_attributes() {

    DISABLE_ATTRIBUTE(position);
    DISABLE_ATTRIBUTE(texcoord);
    DISABLE_ATTRIBUTE(location);
    DISABLE_ATTRIBUTE(multiply);

}

}
}
