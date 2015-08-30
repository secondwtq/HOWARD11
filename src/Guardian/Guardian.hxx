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

#ifndef HOWARD11_GUARDIAN_HXX
#define HOWARD11_GUARDIAN_HXX

#include "GuardianCommon.hxx"

#include "Verdandi/GLVertexBuffer.hxx"
#include "Stannum/StannumTexture.hxx"

#include <memory>

namespace Howard {
namespace Guardian {

class GuardianCanvas {
public:

    GuardianCanvas();
    ~GuardianCanvas();

    void setTarget(std::shared_ptr<Verdandi::TextureImage> target);

    VGLIDX m_fbo = 0;
    glm::mat4 m_mvp;
    std::shared_ptr<Verdandi::TextureImage> m_target;
    std::shared_ptr<Verdandi::VertexBufferSingle<VertFormatGuardianGeneral>> m_vert_buffer;
};

class Helper {
public:
    static glm::mat4 generateMVPForTarget(const Verdandi::TextureImage *target);
};

class GuardianElement {
public:

    virtual void paint(GuardianCanvas *canvas) = 0;

    HPixel m_position;
    HColor m_color;
};

}
}

#endif // HOWARD11_GUARDIAN_HXX
