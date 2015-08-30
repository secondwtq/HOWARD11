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

#ifndef HOWARD11_HAMMERGLUE_HXX
#define HOWARD11_HAMMERGLUE_HXX

#include "Common.hxx"
#include "Hammer.hxx"
#include "HammerTransform.hxx"
#include "HammerPrimitiveBody.hxx"

#include "../thirdpt/physx.hxx"

#include <memory>

namespace Howard {
namespace Hammer {

namespace Glue {

// due to issue of heightfield, we are now using
//  Y as up vector in physics, and all conversion
//  between GLM and PhysX quantities must be conducted
//  with this module
//
inline physx::PxVec3 pxCoord(const HAnyCoord& src) {
    return { src.x, src.z, src.y }; }

inline physx::PxQuat pxQuaternion(const HQuaternion& src) {
    return { src.x, src.z, src.y, src.w }; }

inline physx::PxTransform pxTransform(const Transform& src) {
    return { pxCoord(src.position), pxQuaternion(src.rotation) }; }

inline HAnyCoord anyCoord(const physx::PxVec3& src) {
    return { src.x, src.z, src.y }; }

inline HQuaternion quaternion(const physx::PxQuat& src) {
    return { src.x, src.z, src.y, src.w }; }

inline Transform hammerTransform(const physx::PxTransform& src) {
    return { anyCoord(src.p), quaternion(src.q) }; }

std::unique_ptr<physx::PxGeometry> convertPrimitive(const PrimitiveBase& src);

}

}
}

#endif // HOWARD11_HAMMERGLUE_HXX
