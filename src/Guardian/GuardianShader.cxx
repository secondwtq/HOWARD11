//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-28
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "GuardianShader.hxx"

#include "Verdandi/GLShader.hxx"
#include "Verdandi/GLShaderExt.hxx"
#include "Verdandi/GLShaderExtDef.hxx"

namespace Howard {
namespace Guardian {

void GuardianShader::init_shader() {
    DEF_ATTRIBUTE_NT(position);
    DEF_ATTRIBUTE_NT(texcoord);

    DEF_UNIFORM(brush_center);
    DEF_UNIFORM(brush_size);

    DEF_UNIFORM(texcoord_start);
    DEF_UNIFORM(texcoord_end);

    DEF_UNIFORM(color_multiply);

    DEF_SAMPLER(brush_texture);
    DEF_SAMPLER(brush_mask);
}

}
}
