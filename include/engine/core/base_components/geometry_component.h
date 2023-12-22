#ifndef GEOMETRY_COMPONENT_H
#define GEOMETRY_COMPONENT_H

#include "engine/math/vector.h"
#include <vector>

struct GeometryComponent
{
    std::vector<Vector3> vertices;
    uint32_t clr;
};

#endif
