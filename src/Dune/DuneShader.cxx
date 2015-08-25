//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/25.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "DuneShader.hxx"

#include "Verdandi/GLShader.hxx"
#include "Verdandi/GLShaderExt.hxx"
#include "Verdandi/GLShaderExtDef.hxx"

namespace Howard {
namespace Dune {

void DuneTerrainShader::init_shader() {
    DEF_ATTRIBUTE_NT(position);

    DEF_UNIFORM(model_view_and_projection);
    DEF_UNIFORM(chunk_position);

    DEF_SAMPLER(heightmap);
    DEF_SAMPLER(texcache_diffuse);
    DEF_SAMPLER(texcache_normal);
}

}
}
