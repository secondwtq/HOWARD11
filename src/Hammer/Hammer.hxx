//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-22
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_HAMMER_HXX
#define HOWARD11_HAMMER_HXX

#include <glm/glm.hpp>

#include "HammerPreDef.hxx"

namespace Howard {
namespace Hammer {

class HammerFoundation {
public:
    void initialize();

    inline physx::PxPhysics& physics() {
        return *m_physics; }

    inline physx::PxCooking& cooking() {
        return *m_cooking; }

    inline Material *defaultMaterial() {
        return m_default_mat; }

    physx::PxFoundation *m_foundation = nullptr;
    physx::PxProfileZoneManager *m_profilezone = nullptr;
    physx::PxPhysics *m_physics = nullptr;
    physx::PxCooking *m_cooking = nullptr;
    physx::debugger::comm::PvdConnection *m_pvdconn = nullptr;
    Material *m_default_mat = nullptr;
};

struct HammerSceneCreateArgs {
    size_t dispatcherThreads = 4;
    glm::vec3 gravity { .0f, .0f, -9.81f };
};

class HammerScene {
public:

    void initialize(HammerFoundation *foundation, const HammerSceneCreateArgs& args);

    void createGroundPlane();

    void simulate(double time_step);
    void fetchSync();

    physx::PxScene *scene() {
        return m_pxscene; }

    HammerFoundation *m_found = nullptr;
    physx::PxDefaultCpuDispatcher *m_dispatcher = nullptr;
    physx::PxScene *m_pxscene = nullptr;
    physx::PxRigidStatic *m_groundPlane = nullptr;
    physx::PxControllerManager *m_cctmanager = nullptr;

protected:

    inline physx::PxPhysics& physics() {
        return m_found->physics(); }
};

}
}

#endif // HOWARD11_HAMMER_HXX
