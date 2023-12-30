#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "engine/math/vector.h"

struct PhysicsComponent
{
    Vector3 velocity;
    Vector3 acceleration;
};

#endif
