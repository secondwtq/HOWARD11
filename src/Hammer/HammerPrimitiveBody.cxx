//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-23
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "HammerPrimitiveBody.hxx"

#include "Hammer.hxx"
#include "HammerActorNode.hxx"
#include "../Dwight/Foundation.hxx"
#include "../thirdpt/physx.hxx"

#include "HammerGlue.hxx"

namespace Howard {
namespace Hammer {

Material::Material(HammerFoundation& foundation, float staticF, float dynamicF, float restitution) :
        m_material(foundation.physics().createMaterial(staticF, dynamicF, restitution)) { }

Material::Material(float staticF, float dynamicF, float restitution) :
    Material(Foundation.hammerFoundation(), staticF, dynamicF, restitution) { }

Material::~Material() {
    m_material->release(); }

PrimitiveBody::PrimitiveBody() :
        PrimitiveBody(Foundation.hammerFoundation().defaultMaterial()) { }

void PrimitiveHelper::attachPrimitivesToActor(
        const PrimitiveBody& body, HammerActorNodeBase *node) {
    for (size_t i = 0; i < body.numberOfShapes(); i++) {
        std::unique_ptr<physx::PxGeometry> geo = Glue::convertPrimitive(*body.shape(i));
        physx::PxShape *shape = node->actor()->createShape(*geo.get(), *body.material()->material());
        shape->setLocalPose(Glue::pxTransform(body.shape(i)->transform));
    }

    if (node->node_typeid() == HowardNodeType::NHammerActorNode) {
        physx::PxRigidBodyExt::updateMassAndInertia(*reinterpret_cast
                <HammerActorNode *>(node)->actorDynamic(), 1);
    }
}

}
}
