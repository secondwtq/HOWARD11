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

#ifndef HOWARD11_HAMMERACTORNODE_HXX
#define HOWARD11_HAMMERACTORNODE_HXX

#include "../Dwight/Node.hxx"
#include "../Dwight/Event.hxx"
#include "HammerPreDefExternal.hxx"
#include "HammerTransform.hxx"

namespace Howard {
namespace Hammer {

class HammerScene;

// may be just for CCT.
enum ConstructInternal {
    CONSTRUCT_INTERNAL };

class HammerActorNodeBase : public HNode {
public:

    HammerActorNodeBase(RootNode *scene, const Transform& transform)
            : HNode(scene) { }

    void addToScene(HammerScene *scene);
    void setTransform(const Transform& transform);

    // for scripts ...
    virtual physx::PxRigidActor *actor() {
        return nullptr; };
    virtual Transform transform() const { return { }; };
};

class HammerActorNodeStatic : public HammerActorNodeBase {
public:

    HammerActorNodeStatic(RootNode *scene, const Transform& transform);
    HowardNodeType node_typeid() const override {
        return HowardNodeType::NHammerActorNodeStatic; }
    const char *node_type() const override {
        return HammerActorNodeStatic::m_node_type; }
    static constexpr const char m_node_type[] = "HammerActorNodeStatic";

    physx::PxRigidActor *actor() override;
    inline Transform transform() const override {
        return m_transform; }

private:

    Transform m_transform;
    physx::PxRigidStatic *m_actor;
};

class HammerActorNode : public HammerActorNodeBase {
public:

    HammerActorNode(RootNode *scene, const Transform& transform);
    HowardNodeType node_typeid() const override {
        return HowardNodeType::NHammerActorNode; }
    const char *node_type() const override {
        return HammerActorNode::m_node_type; }
    static constexpr const char m_node_type[] = "HammerActorNode";

    physx::PxRigidActor* actor() override;
    Transform transform() const override;
    inline physx::PxRigidDynamic *actorDynamic() {
        return m_actor; }

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
