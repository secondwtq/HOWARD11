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

#ifndef HOWARD11_HAMMERPREDEF_HXX
#define HOWARD11_HAMMERPREDEF_HXX

namespace physx {

class PxFoundation;
class PxPhysics;
class PxCooking;
class PxScene;
class PxDefaultCpuDispatcher;
class PxProfileZoneManager;
class PxControllerManager;

namespace debugger {
namespace comm {
class PvdConnection;
}
}

class PxMaterial;
class PxRigidActor;
class PxRigidStatic;
class PxRigidDynamic;

class PxHeightField;
}

namespace Howard {
namespace Hammer {
class Material;
class HammerActorNode;
class HammerHeightfield;
}
}

#endif // HOWARD11_HAMMERPREDEF_HXX
