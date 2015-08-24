//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/23.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "HammerGlue.hxx"

#include "HammerPrimitiveBody.hxx"

#include "../thirdpt/physx.hxx"
#include <memory>

namespace Howard {
namespace Hammer {

namespace Glue {

std::unique_ptr<physx::PxGeometry> convertPrimitive(const PrimitiveBase& src) {
    switch (src.type()) {
        case PSphere:
            return std::unique_ptr<physx::PxGeometry>(new physx::PxSphereGeometry(reinterpret_cast
                    <const PrimitiveSphere *>(&src)->radius));
        case PBox:
            return std::unique_ptr<physx::PxGeometry>(new physx::PxBoxGeometry(pxCoord(reinterpret_cast
                    <const PrimitiveBox *>(&src)->halfExtends)));
        case PCapsule: {
            const PrimitiveCapsule *t = reinterpret_cast<const PrimitiveCapsule *>(&src);
            return std::unique_ptr<physx::PxGeometry>(new physx::PxCapsuleGeometry(t->radius, t->halfLength));
        }
        default:
            return nullptr;
    }
}

}

}
}
