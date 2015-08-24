//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/23.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_HAMMERACTORNODE_HXX
#define HOWARD11_HAMMERACTORNODE_HXX

#include "../Dwight/Node.hxx"
#include "../Dwight/Event.hxx"
#include "Hammer.hxx"
#include "HammerPreDef.hxx"
#include "HammerTransform.hxx"

namespace Howard {
namespace Hammer {

enum ConstructInternal {
    CONSTRUCT_INTERNAL
};

class HammerActorNode : public HNode {
public:

    HammerActorNode(RootNode *scene, const Transform& transform);

    void addToScene(HammerScene *scene);

    inline physx::PxRigidDynamic *actor() {
        return m_actor; }

    Transform transform() const;

    void addForce(const HAnyCoord& force);
    void addImpulse(const HAnyCoord& impulse);
    void addAcceleration(const HAnyCoord& acc);

    void setVelocity(const HAnyCoord& velocity);
    void setKinematicTarget(const Transform& transform);
    void setTransform(const Transform& transform);

    bool kinematic() const;
    // donot need it
    bool dynamic() const;

    void setKinematic();
    void setDynamic();

    float mass() const;
    float invMass() const;

protected:

    HammerActorNode(RootNode *scene, const Transform& transform, ConstructInternal);
    physx::PxRigidDynamic *m_actor;

private:

    friend class HammerScene;
    Transform m_transform;

};

class HammerTransformEvent : public HEvent {
public:

    HammerTransformEvent(HammerActorNode *node, const Transform& t)
            : HEvent(node), transform(t) { }

    EventType event_type() const override {
        return EHammerTransformEvent; }
    EventTypeExt event_type_ext() const override {
        return EventType::EHammerTransformEvent; }

    Transform transform;

};

class HammerCCTBoxNode : public HammerActorNode {
public:
    HammerCCTBoxNode(RootNode *scene, const Transform& transform, const HAnyCoord& extents);
};

}
}

#endif // HOWARD11_HAMMERACTORNODE_HXX
