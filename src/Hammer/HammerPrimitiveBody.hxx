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

#ifndef HOWARD11_HAMMERPRIMITIVEBODY_HXX
#define HOWARD11_HAMMERPRIMITIVEBODY_HXX

#include "../Common.hxx"
#include "HammerPreDef.hxx"
#include "HammerTransform.hxx"
#include "Hammer.hxx"
#include "HammerActorNode.hxx"

#include <vector>
#include <memory>
#include <Dwight/Foundation.hxx>

namespace Howard {
namespace Hammer {

enum PrimitiveType {
    PNone,
    PSphere,
    PBox,
    PCapsule,
    PEnd
};

struct PrimitiveBase {
    Transform transform;

    PrimitiveBase(const Transform& t)
            : transform(t) { }

    inline virtual PrimitiveType type() const {
        return PNone; }
};

struct PrimitiveSphere : public PrimitiveBase {
    float radius;

    PrimitiveSphere(const Transform& t, float r)
            : PrimitiveBase(t), radius(r) { }

    inline PrimitiveType type() const override {
        return PSphere; }
};

struct PrimitiveBox : public PrimitiveBase {
    HAnyCoord halfExtends;

    PrimitiveBox(const Transform& t, const HAnyCoord& e)
            : PrimitiveBase(t), halfExtends(e) { }

    inline PrimitiveType type() const override {
        return PBox; }
};

struct PrimitiveCapsule : public PrimitiveBase {
    float radius;
    float halfLength;

    PrimitiveCapsule(const Transform& t, float r, float l)
            : PrimitiveBase(t), radius(r), halfLength(l) { }

    inline PrimitiveType type() const override {
        return PCapsule; }
};

class Material {
public:

    Material(HammerFoundation& foundation,
            float staticF, float dynamicF, float restitution);
    Material(float staticF, float dynamicF, float restitution);
    ~Material();

    const physx::PxMaterial *material() const {
        return m_material; }

private:
    physx::PxMaterial *m_material = nullptr;
};

class PrimitiveBody {
public:

    PrimitiveBody();

    PrimitiveBody(Material *material)
            : m_material(material) { }

    void addSphere(const Transform& transform, float radius) {
        m_primitives.emplace_back(new PrimitiveSphere(transform, radius)); }

    void addBox(const Transform& transform, const HAnyCoord& halfExtends) {
        m_primitives.emplace_back(new PrimitiveBox(transform, halfExtends)); }

    void addCapsule(const Transform& transform, float radius, float halfLength) {
        m_primitives.emplace_back(new PrimitiveCapsule(transform, radius, halfLength)); }

    inline const PrimitiveBase *shape(size_t idx) const {
        return m_primitives[idx].get(); }

    inline size_t numberOfShapes() const {
        return m_primitives.size(); }

    inline const Material *material() const {
        return m_material; }

private:

    std::vector<std::unique_ptr<PrimitiveBase>> m_primitives;
    Material *m_material;
};

class PrimitiveHelper {
public:
    static void attachPrimitivesToActor(const PrimitiveBody& body, HammerActorNodeBase *node);
    static void attachHeightfieldToActor(HammerHeightfield& heightfield,
            HammerActorNodeBase *node, const HAnyCoord& scale);
};

}
}

#endif // HOWARD11_HAMMERPRIMITIVEBODY_HXX
