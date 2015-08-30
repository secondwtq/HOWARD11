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

#ifndef HOWARD11_GUARDIANSHADER_HXX
#define HOWARD11_GUARDIANSHADER_HXX

#include "GuardianCommon.hxx"

#include "Verdandi/GLShader.hxx"
#include "Verdandi/GLShaderExt.hxx"

namespace Howard {
namespace Guardian {

class GuardianShader : public Verdandi::gl_shader_ext {
public:

    ATTR_OBJECT(A, VertFormatGuardianGeneral);

    void init_shader();

    DEF_ATTRIBUTE(position);
    DEF_ATTRIBUTE(texcoord);

    DEF_UNIFORM(model_view_and_projection);

    DEF_UNIFORM(brush_center);
    DEF_UNIFORM(brush_size);

    DEF_UNIFORM(texcoord_start);
    DEF_UNIFORM(texcoord_end);

    DEF_UNIFORM(color_multiply);

    DEF_SAMPLER(brush_texture);
    DEF_SAMPLER(brush_mask);

};

}
}

#endif // HOWARD11_GUARDIANSHADER_HXX
