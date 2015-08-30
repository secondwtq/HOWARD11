//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/30.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "GuardianCacheShader.hxx"

#include "Verdandi/GLShader.hxx"
#include "Verdandi/GLShaderExt.hxx"
#include "Verdandi/GLShaderExtDef.hxx"

namespace Howard {
namespace Guardian {

void GuardianCacheShader::init_shader() {
    DEF_ATTRIBUTE_NT(position);
    DEF_ATTRIBUTE_NT(texcoord);

    DEF_UNIFORM(model_view_and_projection);

    DEF_UNIFORM(brush_center);
//    DEF_UNIFORM(brush_size);

//    DEF_UNIFORM(texcoord_start);
//    DEF_UNIFORM(texcoord_end);

//    DEF_UNIFORM(color_multiply);

    DEF_UNIFORM(uvscales);

    DEF_SAMPLER(texture1);
    DEF_SAMPLER(mask1);
    DEF_SAMPLER(texture2);
    DEF_SAMPLER(mask2);
    DEF_SAMPLER(texture3);
    DEF_SAMPLER(mask3);
    DEF_SAMPLER(texture4);
    DEF_SAMPLER(mask4);
}

}
}
