//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-30
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "HammerHeightfield.hxx"
#include "HammerActorNode.hxx"
#include "HammerPrimitiveBody.hxx"

#include "Dwight/Foundation.hxx"

#include "../thirdpt/physx.hxx"

using namespace physx;

namespace Howard {
namespace Hammer {

HammerHeightfield::HammerHeightfield() :
        HammerHeightfield(Foundation.hammerFoundation().defaultMaterial()) { }

void PrimitiveHelper::attachHeightfieldToActor(HammerHeightfield& heightfield,
        HammerActorNodeBase *node, const HAnyCoord& scale) {
    ASSERT(heightfield.heightfield());

    PxHeightFieldGeometry geometry(heightfield.heightfield(),
            PxMeshGeometryFlags(), scale.z, scale.x, scale.y);
    PxShape *shape = node->actor()->createShape(geometry, *heightfield.material()->material());
}

void HammerHeightfield::generateHeightfield() {
    ASSERT(m_data_img && m_data_img->ok());

    if (m_heightfield) {
        m_heightfield->release();
        m_heightfield = nullptr;
    }

    PxHeightFieldSample *samples = (PxHeightFieldSample *) malloc(sizeof(PxHeightFieldSample) *
            (m_data_img->size.x * m_data_img->size.y));

    PxHeightFieldDesc desc;
    desc.format = PxHeightFieldFormat::eS16_TM;
    // TODO: order correct?
    desc.nbColumns = m_data_img->size.y;
    desc.nbRows = m_data_img->size.x;
    desc.samples.stride = sizeof(PxHeightFieldSample);
    desc.samples.data = samples;

    PxHeightFieldSample *cursample = samples;
//    const RawDataT *curheight = m_data_img->ptr() + m_data_img->size.x * m_data_img->size.y;
    for (size_t j = 0; j < m_data_img->size.y; j++) {
        for (size_t i = 0; i < m_data_img->size.x; i++) {
            cursample->height = m_data_img->ptr()[i * m_data_img->size.y + j];

            cursample->materialIndex0 =
                    cursample->materialIndex1 = 0;
            cursample->clearTessFlag();

            cursample += 1;
        }
    }

    m_heightfield = Foundation.hammerFoundation().cooking().createHeightField(desc,
        Foundation.hammerFoundation().physics().getPhysicsInsertionCallback());

    free(samples);
}

void HammerHeightfield::setDataImage(std::shared_ptr<Verdandi::Image> image) {
    m_data_img = image;
}

HammerHeightfield::~HammerHeightfield() {
    if (m_heightfield) {
        m_heightfield->release(); }
}

}
}
