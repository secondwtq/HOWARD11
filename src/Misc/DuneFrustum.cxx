//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/01.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "DuneFrustum.hxx"

#include <glm/glm.hpp>

namespace Howard {
namespace Dolly {

FrustumBox FrustumBox::createFromMatrix(const glm::mat4& view_proj) {
    FrustumBox ret;
    FrustumPlane (&planes)[FrustumPlaneType::PEnd] = ret.m_planes;

    planes[PLeft].n.x = view_proj[0][3] + view_proj[0][0];
    planes[PLeft].n.y = view_proj[1][3] + view_proj[1][0];
    planes[PLeft].n.z = view_proj[2][3] + view_proj[2][0];
    planes[PLeft].d = view_proj[3][3] + view_proj[3][0];

    planes[PRight].n.x = view_proj[0][3] - view_proj[0][0];
    planes[PRight].n.y = view_proj[1][3] - view_proj[1][0];
    planes[PRight].n.z = view_proj[2][3] - view_proj[2][0];
    planes[PRight].d = view_proj[3][3] - view_proj[3][0];

    planes[PBottom].n.x = view_proj[0][3] + view_proj[0][1];
    planes[PBottom].n.y = view_proj[1][3] + view_proj[1][1];
    planes[PBottom].n.z = view_proj[2][3] + view_proj[2][1];
    planes[PBottom].d = view_proj[3][3] + view_proj[3][1];

    planes[PTop].n.x = view_proj[0][3] - view_proj[0][1];
    planes[PTop].n.y = view_proj[1][3] - view_proj[1][1];
    planes[PTop].n.z = view_proj[2][3] - view_proj[2][1];
    planes[PTop].d = view_proj[3][3] - view_proj[3][1];

    planes[PNear].n.x = view_proj[0][3] + view_proj[0][2];
    planes[PNear].n.y = view_proj[1][3] + view_proj[1][2];
    planes[PNear].n.z = view_proj[2][3] + view_proj[2][2];
    planes[PNear].d = view_proj[3][3] + view_proj[3][2];

    planes[PFar].n.x = view_proj[0][3] - view_proj[0][2];
    planes[PFar].n.y = view_proj[1][3] - view_proj[1][2];
    planes[PFar].n.z = view_proj[2][3] - view_proj[2][2];
    planes[PFar].d = view_proj[3][3] - view_proj[3][2];

    for (size_t i = 0; i < 6; i++) {
        float l = glm::length(planes[i].n);
        planes[i].n /= l, planes[i].d /= l;
    }

    return ret;
}

bool FrustumBox::checkAABB(const HAnyCoord& center, const HAnyCoord& half_extends) const {
    for (size_t i = 0; i < 6; i++) {
        // ---
        if (m_planes[i].distanceTo({ center.x - half_extends.x,
                center.y - half_extends.y, center.z - half_extends.z }) >= 0) {
            continue; }
        // +--
        if (m_planes[i].distanceTo({ center.x + half_extends.x,
                center.y - half_extends.y, center.z - half_extends.z }) >= 0) {
            continue; }
        // -+-
        if (m_planes[i].distanceTo({ center.x - half_extends.x,
                center.y + half_extends.y, center.z - half_extends.z }) >= 0) {
            continue; }
        // --+
        if (m_planes[i].distanceTo({ center.x - half_extends.x,
                center.y - half_extends.y, center.z + half_extends.z }) >= 0) {
            continue; }
        // ++-
        if (m_planes[i].distanceTo({ center.x + half_extends.x,
                center.y + half_extends.y, center.z - half_extends.z }) >= 0) {
            continue; }
        // +-+
        if (m_planes[i].distanceTo({ center.x + half_extends.x,
                center.y - half_extends.y, center.z + half_extends.z }) >= 0) {
            continue; }
        // -++
        if (m_planes[i].distanceTo({ center.x - half_extends.x,
                center.y + half_extends.y, center.z + half_extends.z }) >= 0) {
            continue; }
        // +++
        if (m_planes[i].distanceTo({ center.x + half_extends.x,
                center.y + half_extends.y, center.z + half_extends.z }) >= 0) {
            continue; }

        return false;
    }

    return true;
}

}
}
