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

#include <glm/gtc/matrix_transform.hpp>
#include "Guardian.hxx"
#include "GuardianCommon.hxx"

#include "Verdandi/GLVertexBuffer.hxx"

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

static const GLenum fb_bufs[1] = {
        GL_COLOR_ATTACHMENT0 };

GuardianCanvas::GuardianCanvas() : m_vert_buffer(
        Verdandi::VertexBufferSingle<VertFormatGuardianGeneral>
        ::createWithData(Verdandi::BufferUsage::UStatic,
        quad_vertex_buffer_data, sizeof(quad_vertex_buffer_data))) {
    glGenFramebuffers(1, &this->m_fbo);
}

GuardianCanvas::~GuardianCanvas() {
    glDeleteFramebuffers(1, &this->m_fbo);
}

void GuardianCanvas::setTarget(std::shared_ptr<Verdandi::TextureImage> target) {
    ASSERT(target);
    ASSERT(target->ok());

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target->id(), 0);
    glViewport(0, 0, static_cast<int>(target->size.x),
            static_cast<int>(target->size.y));
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ASSERT_FOUNDATION(); }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_target = target;
    m_mvp = Helper::generateMVPForTarget(target.get());
}

glm::mat4 Helper::generateMVPForTarget(const Verdandi::TextureImage *target) {
    ASSERT(target);
    ASSERT(target->ok());

    glm::mat4 projection = glm::ortho(0.f, (float) target->size.x,
            (float) target->size.y, 0.f, -16384.f, 16384.f);
    return projection;
}

}
}