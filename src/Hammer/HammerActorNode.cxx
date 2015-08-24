//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/23.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "HammerActorNode.hxx"
#include "Hammer.hxx"

#include "../Dwight/Node.hxx"
#include "../Dwight/Foundation.hxx"

#include "../thirdpt/physx.hxx"

#include "HammerTransform.hxx"
#include "HammerGlue.hxx"

namespace Howard {
namespace Hammer {

HammerActorNode::HammerActorNode(RootNode *scene, const Transform& transform)
        : HNode(scene), m_actor(Foundation.hammerFoundation().physics().
        createRigidDynamic(Glue::pxTransform(transform))), m_transform(transform) {
    m_actor->userData = this;
}

void HammerActorNode::addToScene(HammerScene *scene) {
        scene->scene()->addActor(*m_actor); }

Transform HammerActorNode::transform() const {
    return m_transform; }

void HammerActorNode::addForce(const HAnyCoord& coord) {
    m_actor->addForce(Glue::pxCoord(coord));
}

void HammerActorNode::addAcceleration(const HAnyCoord& acc) {
    m_actor->addForce(Glue::pxCoord(acc), physx::PxForceMode::eACCELERATION);
}

void HammerActorNode::addImpulse(const HAnyCoord& impulse) {
    m_actor->addForce(Glue::pxCoord(impulse), physx::PxForceMode::eIMPULSE);
}

void HammerActorNode::setVelocity(const HAnyCoord& velocity) {
    m_actor->setLinearVelocity(Glue::pxCoord(velocity));
}

float HammerActorNode::mass() const {
    return m_actor->getMass(); }

float HammerActorNode::invMass() const {
    return m_actor->getInvMass(); }

}
}