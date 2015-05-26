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

#include "Stannum.hxx"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Debug.hxx"

#include "AtTheVeryBeginning.hxx"

namespace Howard {

namespace Stannum {

MappedVertexBuffer<VertFormatSprite> *StannumCommandTest::m_buf = nullptr;

template<typename T>
MappedVertexBuffer<T> *SharedVertexBuffer::map(size_t count) {
    size_t sz_rounded = this->mapped_end;
    for (size_t i = 0; i < sizeof(T); i++) {
        if (sz_rounded % sizeof(T) == 0) {
            break; }
        sz_rounded++;
    }
    if (count * sizeof(T) + sz_rounded <= this->m_size) {
        MappedVertexBuffer<T> *ret = new MappedVertexBuffer<T>(this, count, sz_rounded);
        this->mapped_end = sz_rounded + count * sizeof(T);
        this->m_mappings.push_back(ret);
        return ret;
    }
    return nullptr;
}

void StannumRenderer::init() {
    this->m_shader_cache.load_shaders();
    this->m_shared_vb.init(2048 * sizeof(glm::vec3) * 3);

    StannumCommandTest::m_buf = this->m_shared_vb.map<VertFormatSprite>(128);
}

void StannumCommandTest::execute(StannumRenderer *renderer) {

    using namespace AtTheVeryBeginning;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto *shader = renderer->shaders()->get_shader<Verdandi::SpriteShader>
            (ShaderType::CommonSprite);
    shader->use();

//    glm::mat4 projection = glm::ortho(0.f, (float) Setting<WindowSetting>::instance()->actual_width,
//            (float) Setting<WindowSetting>::instance()->actual_height, 0.f, -16384.f, 16384.f);
    glm::mat4 projection = glm::ortho(0.f, (float) 1600, (float) 1200, 0.f, -16384.f, 16384.f);
    SET_UNIFORMAT4(*shader, mvp, projection);

    size_t idx = 0;
    for (size_t i = 0; i < 6; i++)
        idx = StannumCommandTest::m_buf->push(&this->m_data->data[i]);

    StannumCommandTest::m_buf->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertFormatSprite),
                          (void *) StannumCommandTest::m_buf->m_start);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertFormatSprite),
                          (void *) (StannumCommandTest::m_buf->m_start + sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertFormatSprite),
                          (void *) (StannumCommandTest::m_buf->m_start + 2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertFormatSprite),
                          (void *) (StannumCommandTest::m_buf->m_start + 2 * sizeof(glm::vec3) +
                            sizeof(glm::vec2)));
    StannumCommandTest::m_buf->upload();
    BIND_TEXTUREP(shader, texture_major, m_data->texture->id(), 0);
    glDrawArrays(GL_TRIANGLES, idx-5, 6);

    shader->disable_attributes();
}

}

}
