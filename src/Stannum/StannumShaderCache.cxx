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

#include "Stannum/StannumSpriteShader.hxx"
#include "Dune/DuneShader.hxx"
#include "Guardian/GuardianShader.hxx"

#include <iostream>

namespace Howard {
namespace Stannum {

void ShaderCache::load_shaders() {

    // TODO: too much duplication
    // another TODO: output log with FSM instead of iostream
    // another TODO: lazy loading
    // another TODO: async loading
    {
        SpriteShader *shader_sprite = new SpriteShader();
        shader_sprite->load_file(Verdandi::SHADERTYPE::VERTEX, "stannum.vert");
        shader_sprite->load_file(Verdandi::SHADERTYPE::FRAG, "stannum.frag");
        shader_sprite->create();
        shader_sprite->init_shader();

        std::cout << shader_sprite->log(Verdandi::SHADERTYPE::VERTEX);
        std::cout << shader_sprite->log(Verdandi::SHADERTYPE::FRAG);

        m_shaders[ShaderType::CommonSprite] = shader_sprite;
    }

    {
        Dune::DuneTerrainShader *shader_terrain = new Dune::DuneTerrainShader();
        shader_terrain->load_file(Verdandi::SHADERTYPE::VERTEX, "dune_terrain.vert");
        shader_terrain->load_file(Verdandi::SHADERTYPE::FRAG, "dune_terrain.frag");
        shader_terrain->create();
        shader_terrain->init_shader();

        std::cout << shader_terrain->log(Verdandi::SHADERTYPE::VERTEX);
        std::cout << shader_terrain->log(Verdandi::SHADERTYPE::FRAG);

        m_shaders[ShaderType::DuneTerrain] = shader_terrain;
    }

    {
        Guardian::GuardianShader *shader_guardian = new Guardian::GuardianShader();
        shader_guardian->load_file(Verdandi::SHADERTYPE::VERTEX, "guardian.vert");
        shader_guardian->load_file(Verdandi::SHADERTYPE::FRAG, "guardian.frag");
        shader_guardian->create();
        shader_guardian->init_shader();

        std::cout << shader_guardian->log(Verdandi::SHADERTYPE::VERTEX);
        std::cout << shader_guardian->log(Verdandi::SHADERTYPE::FRAG);

        m_shaders[ShaderType::Guardian] = shader_guardian;
    }

}

void ShaderCache::destroy_shaders() {
    for (auto& shader : m_shaders) {
        if (shader) {
            shader->destroy(); }
    }
}

}

}