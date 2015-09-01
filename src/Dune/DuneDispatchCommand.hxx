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

#ifndef HOWARD11_DUNEDISPATCHCOMMAND_HXX
#define HOWARD11_DUNEDISPATCHCOMMAND_HXX

#include "Stannum/StannumCommon.hxx"
#include "Stannum/Stannum.hxx"

namespace Howard {
namespace Dolly {
class Camera;
}
namespace Dune {

class DuneTerrain;

class DispatchCommandDuneTerrain : public Stannum::StannumDispatchCommand {
public:

    DispatchCommandDuneTerrain(DuneTerrain *terrain, Dolly::Camera *camera)
            : m_terrain(terrain), m_camera(camera) { }

    Stannum::DispatchCommandType commandType() const override {
        return Stannum::DispatchCommandType::DDuneTerrain; }

    void execute(Stannum::StannumRenderer *renderer) override;

private:
    DuneTerrain *m_terrain;
    Dolly::Camera *m_camera;
};

}
}

#endif // HOWARD11_DUNEDISPATCHCOMMAND_HXX
