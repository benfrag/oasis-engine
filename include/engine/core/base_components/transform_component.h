#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "engine/math/vector.h"

struct TransformComponent
{
    Vector3 position;
    Vector3 rotation;

    bool operator!=(const TransformComponent& other) const {
        return position != other.position; // Assuming Vector3 has a != operator defined
    }
};

#endif
