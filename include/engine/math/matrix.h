#ifndef MATRIX_H
#define MATRIX_H

#include "engine/math/vector.h"

class Matrix4
{
public:
    float m[16];
    static Matrix4 create_perspective(float fov, float aspect_ratio, float near_plane, float far_plane);
    static Matrix4 create_orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane);
    static Matrix4 create_view(Vector3 position, float yaw, float pitch);
    Vector3 multiply(const Vector3& vec);

    static Matrix4 multiply_mat(Matrix4* _A, Matrix4* _B);

    static Matrix4 identity();
};

#endif
