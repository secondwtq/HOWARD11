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

#include "Guardian.hxx"
#include "GuardianCommon.hxx"
#include "GuardianShader.hxx"
#include "GuardianCacheShader.hxx"

#include <glm/gtc/matrix_transform.hpp>
#include "Stannum/Stannum.hxx"
#include "Verdandi/GLFoundation.hxx"
#include "Verdandi/GLVertexBuffer.hxx"
#include "Verdandi/GLVertexBufferExt.hxx"

#include "Dune/Dune.hxx"
#include <functional>

namespace Howard {
namespace Guardian {

static const VertFormatGuardianGeneral quad_vertex_buffer_data[] = {
    { { -1.0f, -1.0f }, { 0.f, 0.f } },
	{ { 1.0f, -1.0f }, {  1.f, 0.f } },
	{ { -1.0f,  1.0f }, { 0.f, 1.f } },

	{ { -1.0f,  1.0f }, { 0.f, 1.f } },
	{ { 1.0f, -1.0f }, {  1.f, 0.f } },
	{ { 1.0f,  1.0f }, { 1.f, 1.f } },
};

GuardianCanvas::GuardianCanvas(Stannum::StannumRenderer *renderer)
        : m_vert_buffer(
        Verdandi::VertexBufferSingle<VertFormatGuardianGeneral>
        ::createWithData(Verdandi::BufferUsage::UStatic,
        quad_vertex_buffer_data, sizeof(quad_vertex_buffer_data))),
        m_renderer(renderer) {
    glGenFramebuffers(1, &this->m_fbo);
}

GuardianCanvas::~GuardianCanvas() {
    glDeleteFramebuffers(1, &this->m_fbo);
}

void GuardianCanvas::paintBlock(std::function<void (GuardianCanvas *)> paintfunc) {
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_fbo);
    glViewport(0, 0, static_cast<int>(size().x),
            static_cast<int>(size().y));
    paintfunc(this);
    Verdandi::resetRenderTarget();
}

void GuardianCanvas::setTarget(std::shared_ptr<Verdandi::TextureImage> target) {
    ASSERT(target);
    ASSERT(target->ok());
    //static const GLenum fb_bufs[1] = {
    //        GL_COLOR_ATTACHMENT0 };
    m_size = target->size;

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target->id(), 0);
//    glDrawBuffers(1, fb_bufs);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ASSERT_FOUNDATION(); }
    Verdandi::resetRenderTarget();

    m_target = target;
    m_mvp = Helper::generateMVPForTarget(target.get());
}

glm::mat4 Helper::generateMVPForTarget(const Verdandi::TextureImage *target) {
    ASSERT(target);
    ASSERT(target->ok());

    glm::mat4 projection = glm::ortho(0.f, (float) target->size.x,
            0.f, (float) target->size.y, -16384.f, 16384.f);
//    glm::mat4 projection = glm::ortho(0.f, (float) target->size.x,
//            (float) target->size.y, 0.f, -16384.f, 16384.f);
    return projection;
}

void GuardianElementTerrainLayersGroup::paint(GuardianCanvas *canvas) {
    using namespace Stannum;
    using namespace Verdandi;
    using namespace Dune;

    auto shader = canvas->m_renderer->shaders()->get_shader
            <GuardianCacheShader>(ShaderType::GuardianCache);
    shader->use();
    VertexBufferScope<VertexBufferSingle<VertFormatGuardianGeneral>>
            buffer(canvas->m_vert_buffer);
    VertexAttributeArrayScope<2> attrs({ 0, 1 });
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
            sizeof(VertFormatGuardianGeneral), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
            sizeof(VertFormatGuardianGeneral), (void *) sizeof(glm::vec2));

    glm::mat4 uvscales { 0 };
    if (numberOfLayers() >= 1) {
        auto layer = m_layers[0].lock();
        BIND_TEXTUREP(shader, texture1, layer->textureSet()
                ->texture(DuneTextureType::DColor)->id(), 0);
        BIND_TEXTUREP(shader, mask1, layer->mask()->id(), 1);
        uvscales[0][0] = layer->uvscale.x;
    }
    if (numberOfLayers() >= 2) {
        auto layer = m_layers[1].lock();
        BIND_TEXTUREP(shader, texture2, layer->textureSet()
                ->texture(DuneTextureType::DColor)->id(), 2);
        BIND_TEXTUREP(shader, mask2, layer->mask()->id(), 3);
        uvscales[0][1] = layer->uvscale.x;
    }
    if (numberOfLayers() >= 3) {
        auto layer = m_layers[2].lock();
        BIND_TEXTUREP(shader, texture3, layer->textureSet()
                ->texture(DuneTextureType::DColor)->id(), 4);
        BIND_TEXTUREP(shader, mask3, layer->mask()->id(), 5);
        uvscales[0][2] = layer->uvscale.x;
    }
    if (numberOfLayers() >= 4) {
        auto layer = m_layers[3].lock();
        BIND_TEXTUREP(shader, texture4, layer->textureSet()
                ->texture(DuneTextureType::DColor)->id(), 6);
        BIND_TEXTUREP(shader, mask4, layer->mask()->id(), 7);
        uvscales[0][3] = layer->uvscale.x;
    }

    glm::vec2 center { m_position.x * 512 + 256,
            m_position.y * 512 + 256 };
    SET_UNIFORMAT4P(shader, model_view_and_projection, canvas->m_mvp);
    SET_UNIFORM2P(shader, brush_center, center);
    SET_UNIFORMAT4P(shader, uvscales, uvscales);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

}
}