//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-09-01
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "DuneDispatchCommand.hxx"

#include "Dune.hxx"
#include "DuneTextureCache.hxx"
#include "DuneShader.hxx"

#include "Stannum/Stannum.hxx"
#include "Misc/Transform.hxx"
#include "Misc/Frustum.hxx"
#include "Misc/AtTheVeryBeginning.hxx"

#include "Verdandi/GLCommon.hxx"
#include "Verdandi/GLVertexBuffer.hxx"
#include "Verdandi/GLVertexArray.hxx"

#include "thirdpt/howardgl.hxx"
#include <glm/gtc/matrix_transform.hpp>

namespace Howard {
namespace Dune {

using namespace Verdandi;

void DispatchCommandDuneTerrain::execute(Stannum::StannumRenderer *renderer) {
    using namespace Stannum;
    using namespace AtTheVeryBeginning;

    glm::mat4 view_projection;
    {
        glm::mat4 projection = glm::ortho(0.f, (float) Setting<WindowSetting>::instance()->actual_width,
                (float) Setting<WindowSetting>::instance()->actual_height, 0.f, -16384.f, 16384.f);
        view_projection = scale(projection, glm::vec3(m_camera->scale_factor)) * m_camera->view_mat;
    }

    std::vector<DuneChunk *> chunks_to_render;
    Dolly::FrustumBox frustum = Dolly::FrustumBox::createFromMatrix(view_projection);
    for (auto chunks : m_terrain->m_chunks) {
        for (auto chunk : chunks) {
            // TODO: more accurate AABB, maybe with physics?
            HAnyCoord center(chunk->position());
            center += HAnyCoord((glm::vec2) m_terrain->sizePerChunk2D() / 2.0f,
                    Constants::heightSpanMax / 2.0f);
            HAnyCoord half_extends { (glm::vec2) m_terrain->sizePerChunk2D() / 2.0f,
                    Constants::heightSpanMax / 2.0f };
            if (frustum.checkAABB(center, half_extends)) {
                chunks_to_render.push_back(chunk.get()); }
        }
    }

    size_t numChunkToRender = chunks_to_render.size(),
            numChunkCached = 0, numChunkToCache = 0;
    for (auto chunk : chunks_to_render) {
        if (chunk->cached()) {
            numChunkCached++;
            chunk->cacheMarkUsed();
        }
    }
    for (auto chunk : chunks_to_render) {
        if (!chunk->cached()) {
            numChunkToCache++;
            chunk->cache();
        }
    }

    auto *shader = renderer->shaders()->get_shader
            <DuneTerrainShader>(ShaderType::DuneTerrain);
    shader->use();
    VertexArrayScope vao(m_terrain->m_vao);

    SET_UNIFORMAT4P(shader, model_view_and_projection, view_projection);
    if (m_terrain->heightmap() && m_terrain->heightmap()->ok()) {
        BIND_TEXTUREP(shader, heightmap, m_terrain->heightmap()->id(), 0); }
    SET_UNIFORM2P(shader, total_size, m_terrain->m_total_size);
    SET_UNIFORM2P(shader, chunk_size, m_terrain->sizePerChunk2D());

    // TODO: culling, without which the cache
    //  just does not work
    // TODO: instancing
    for (auto chunk : chunks_to_render) {
        // anyone has something like "multicache"?
        //  this assert surprisingly works for FIFO solution
        ASSERT(chunk->cached());

        SET_UNIFORM3P(shader, chunk_position, chunk->position());
        SET_UNIFORM2P(shader, cache_position, chunk->cacheData()->texcoord());
        BIND_TEXTUREP(shader, texcache_diffuse,
                chunk->cacheData()->cache.lock()->textures()
                [DuneTextureType::DColor]->id(), 1);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(
                m_terrain->vertexBuffer()->countElements()));
    }
}

}
}
