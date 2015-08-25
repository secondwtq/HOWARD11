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

#include "StannumShaderCache.hxx"

#include "StannumSprite.hxx"

namespace Howard {

namespace Stannum {

void ShaderCache::load_shaders() {
    SpriteShader *shader_sprite = new SpriteShader();
    shader_sprite->load_file(Verdandi::SHADERTYPE::VERTEX, "stannum.vert");
    shader_sprite->load_file(Verdandi::SHADERTYPE::FRAG, "stannum.frag");
    shader_sprite->create();
    shader_sprite->init_shader();

    std::cout << shader_sprite->log(Verdandi::SHADERTYPE::VERTEX);
    std::cout << shader_sprite->log(Verdandi::SHADERTYPE::FRAG);

    m_shaders[ShaderType::CommonSprite] = shader_sprite;

}

void ShaderCache::destroy_shaders() {
    for (auto& shader : m_shaders) {
        if (shader) {
            shader->destroy(); }
    }
}

}

}