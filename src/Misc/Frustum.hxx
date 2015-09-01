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

#ifndef HOWARD11_DUNEFRUSTUM_HXX
#define HOWARD11_DUNEFRUSTUM_HXX

#include "Common.hxx"

namespace Howard {
namespace Dolly {

enum FrustumPlaneType {
    PLeft,
    PRight,
    PTop,
    PBottom,
    PNear,
    PFar,
    PEnd
};

struct FrustumAABB {
    HAnyCoord center;
    HAnyCoord halfExtends;
};

struct FrustumPlane {
    // normal
    HAnyCoord n;
    // distance
    float d;

    // signed result
    inline float distanceTo(const HAnyCoord& point) const {
        return glm::dot(n, point) + d; }
};

class FrustumBox {
public:

    static FrustumBox createFromMatrix(const glm::mat4& view_proj);

    bool checkPoint(const HAnyCoord& point) const {
        for (size_t i = 0; i < 6; i++) {
            if (m_planes[i].distanceTo(point) <= 0) {
                return false; }
        }
        return true;
    }

    // TODO: maybe returning how far away instead, and entirely/partial tests
    bool checkSphere(const HAnyCoord& center, float radius) const {
        for (size_t i = 0; i < 6; i++) {
            if (m_planes[i].distanceTo(center) <= -radius) {
                return false; }
        }
        return true;
    }

    inline bool checkAABB(const FrustumAABB& aabb) const {
        return checkAABB(aabb.center, aabb.halfExtends); }

    bool checkAABB(const HAnyCoord& center, const HAnyCoord& half_extends) const;

private:

    FrustumPlane m_planes[FrustumPlaneType::PEnd];
};

}
}

#endif // HOWARD11_DUNEFRUSTUM_HXX
