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

#ifndef HOWARD11_HAMMERHEIGHTFIELD_HXX
#define HOWARD11_HAMMERHEIGHTFIELD_HXX

#include "Common.hxx"
#include "HammerPreDef.hxx"
#include "HammerPreDefExternal.hxx"
#include <memory>

namespace Howard {
namespace Verdandi {
class Image;
}
namespace Hammer {

class HammerHeightfield {
public:

    HammerHeightfield();
    HammerHeightfield(Material *material)
        : m_material(material) { }
    ~HammerHeightfield();

    inline physx::PxHeightField *heightfield() {
        return m_heightfield; }

    inline const Material *material() const {
        return m_material; }

    void generateHeightfield();

    void setDataImage(SHARED(Verdandi::Image) image);

private:

    SHARED(Verdandi::Image) m_data_img;
    physx::PxHeightField *m_heightfield = nullptr;
    Material *m_material;
};

}
}

#endif // HOWARD11_HAMMERHEIGHTFIELD_HXX
