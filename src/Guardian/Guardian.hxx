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
#include <functional>

namespace Howard {

namespace Stannum {
class StannumRenderer;
}

namespace Guardian {

class GuardianCanvas {
public:

    GuardianCanvas(Stannum::StannumRenderer *renderer);
    ~GuardianCanvas();

    void setTarget(std::shared_ptr<Verdandi::TextureImage> target);
    void paintBlock(std::function<void (GuardianCanvas *)> paintfunc);
    inline HPixel size() const {
        return m_size; }

    VGLIDX m_fbo = 0;
    glm::mat4 m_mvp;
    std::shared_ptr<Verdandi::TextureImage> m_target;
    std::shared_ptr<Verdandi::VertexBufferSingle<VertFormatGuardianGeneral>> m_vert_buffer;

    Stannum::StannumRenderer *m_renderer;

private:
    HPixel m_size;
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

namespace Dune {
    class DuneLayer;
}

namespace Guardian {

class GuardianElementTerrainLayersGroup : public GuardianElement {
public:

    GuardianElementTerrainLayersGroup() :
            GuardianElementTerrainLayersGroup(glm::u8vec2()) { }
    GuardianElementTerrainLayersGroup(const glm::u8vec2& idx) {
        setCacheIndex(idx); }

    void paint(GuardianCanvas *canvas) override;

    void setCacheIndex(const glm::u8vec2& idx) {
        ASSERT(idx.x >= 0 && idx.x < 8);
        ASSERT(idx.y >= 0 && idx.y < 8);
        this->m_position = idx;
    }

    inline size_t numberOfLayers() const {
        return m_layers.size(); }
    void addLayer(std::weak_ptr<Dune::DuneLayer> layer) {
        ASSERT(numberOfLayers() < 4);
        ASSERT(!layer.expired());
        m_layers.push_back(layer);
    }

private:
    std::vector<std::weak_ptr<Dune::DuneLayer>> m_layers;
};

}
}

#endif // HOWARD11_GUARDIAN_HXX
