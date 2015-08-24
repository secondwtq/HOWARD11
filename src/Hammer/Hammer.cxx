//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/22.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "Hammer.hxx"
#include "../Misc/hassert.hxx"

#include "../thirdpt/physx.hxx"

#include "HammerPrimitiveBody.hxx"
#include "HammerActorNode.hxx"
#include "HammerGlue.hxx"

using namespace physx;

static physx::PxDefaultErrorCallback pxx_def_errorcb;
static physx::PxDefaultAllocator pxx_def_allocator;

namespace Howard {
namespace Hammer {

void HammerFoundation::initialize() {
    m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, // to check version mismatch
            pxx_def_allocator, pxx_def_errorcb);
    ASSERT(m_foundation);
    m_profilezone = &physx::PxProfileZoneManager::createProfileZoneManager(m_foundation);

    physx::PxTolerancesScale scale; // world scale
    bool record_mem_alloc = true; // optional, for PVD
    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION,
            *m_foundation, scale, record_mem_alloc, m_profilezone);
    ASSERT(m_physics);

    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION,
            *m_foundation, physx::PxCookingParams(scale));
    ASSERT(m_cooking);

    ASSERT(PxInitExtensions(*m_physics));

    m_default_mat = new Material(*this, 0.5, 0.5, 0.6);

    if (m_physics->getPvdConnectionManager()) {
        m_physics->getVisualDebugger()->setVisualizeConstraints(true);
        m_physics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONSTRAINTS, true);
        m_physics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
        m_physics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
        m_pvdconn = PxVisualDebuggerExt::createConnection(m_physics->getPvdConnectionManager(), "10.211.55.3", 5425, 1000);
    }
}

void HammerScene::initialize(HammerFoundation *foundation,
        const HammerSceneCreateArgs& args) {
    m_found = foundation;
    m_dispatcher = physx::PxDefaultCpuDispatcherCreate(args.dispatcherThreads);
    assert(m_dispatcher);
    physx::PxTolerancesScale scale;
    physx::PxSceneDesc desc(scale);
    desc.gravity = physx::PxVec3(args.gravity.x, args.gravity.y, args.gravity.z);
    desc.filterShader = physx::PxDefaultSimulationFilterShader;
    desc.cpuDispatcher = m_dispatcher;
    desc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
    m_pxscene = foundation->m_physics->createScene(desc);
    assert(m_pxscene);
}

void HammerScene::createGroundPlane() {
    PxMaterial *mat = physics().createMaterial(.5f, .5f, .6f);
    PxRigidStatic *ground_plane = PxCreatePlane(physics(), PxPlane(0, 0, 1, 0), *mat);
    m_pxscene->addActor(*ground_plane);
    m_groundPlane = ground_plane;
}

void HammerScene::simulate(double time_step) {
    m_pxscene->simulate(time_step);
}

void HammerScene::fetchSync() {
    m_pxscene->fetchResults(true);

    PxU32 n_active_transforms;
    const PxActiveTransform *active_transforms =
            m_pxscene->getActiveTransforms(n_active_transforms);
    for (PxU32 i = 0; i < n_active_transforms; i++) {
        HammerActorNode *n = reinterpret_cast<HammerActorNode *>(active_transforms[i].userData);
        Transform t = Glue::hammerTransform(active_transforms[i].actor2World);
        n->invoke_event(std::make_shared<HammerTransformEvent>(n, t));
        n->m_transform = t;
    }
}

}
}
