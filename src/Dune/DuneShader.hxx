//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-25
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_DUNESHADER_HXX
#define HOWARD11_DUNESHADER_HXX

#include "DuneCommon.hxx"

#include "Verdandi/GLShader.hxx"
#include "Verdandi/GLShaderExt.hxx"

namespace Howard {
namespace Dune {

class DuneTerrainShader : public Verdandi::gl_shader_ext {
public:

    ATTR_OBJECT(A, VertFormatDuneTerrain);

    void init_shader();

    DEF_ATTRIBUTE(position);

    DEF_UNIFORM(model_view_and_projection);
    DEF_UNIFORM(chunk_position);
    DEF_UNIFORM(total_size);
    DEF_UNIFORM(cache_position);
    DEF_UNIFORM(chunk_size);

    DEF_SAMPLER(heightmap);
    DEF_SAMPLER(texcache_diffuse);
    DEF_SAMPLER(texcache_normal);
};

class DuneCacheShader : public Verdandi::gl_shader_ext {
public:

};

}
}

#endif // HOWARD11_DUNESHADER_HXX
