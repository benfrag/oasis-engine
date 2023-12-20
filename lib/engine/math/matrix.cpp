#include "engine/math/matrix.h"
#include <iostream>

Matrix4 Matrix4::create_perspective(float fov, float aspect_ratio, float near_plane, float far_plane)
{
    Matrix4 result;
    const float rad = fov * 3.14159265358979323846f / 180.0f;
    std::cout << "rad " << rad << std::endl;
    const float tanHalfFov = tan(rad / 2.0f);
    result.m[0] = 1.0f / (aspect_ratio * tanHalfFov);
    result.m[1] = 0.0f;
    result.m[2] = 0.0f;
    result.m[3] = 0.0f;

    result.m[4] = 0.0f;
    result.m[5] = 1.0f / (tanHalfFov);
    result.m[6] = 0.0f;
    result.m[7] = 0.0f;

    result.m[8] = 0.0f;
    result.m[9] = 0.0f;
    result.m[10] = -(far_plane + near_plane) / (far_plane - near_plane);
    result.m[11] = -1.0f;

    result.m[12] = 0.0f;
    result.m[13] = 0.0f;
    result.m[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);
    result.m[15] = 0.0f;

    std::cout << "set matrix" << std::endl;

    return result;
}

Matrix4 Matrix4::create_orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane)
{
    Matrix4 result;

    result.m[0] = 2.0f / (right - left);
    result.m[1] = 0.0f;
    result.m[2] = 0.0f;
    result.m[3] = 0.0f;

    result.m[4] = 0.0f;
    result.m[5] = 2.0f / (top - bottom);
    result.m[6] = 0.0f;
    result.m[7] = 0.0f;

    result.m[8] = 0.0f;
    result.m[9] = 0.0f;
    result.m[10] = -2.0f / (far_plane - near_plane);
    result.m[11] = 0.0f;

    result.m[12] = -(right + left) / (right - left);
    result.m[13] = -(top + bottom) / (top - bottom);
    result.m[14] = -(far_plane + near_plane) / (far_plane - near_plane);
    result.m[15] = 1.0f;

    return result;
}

Matrix4 Matrix4::create_view(Vector3 position, float yaw, float pitch)
{
    Vector3 look_at = {0, 0, 0};
    yaw = yaw * 3.14159265358979323846 / 180.0f;
    pitch = pitch * 3.14159265358979323846 / 180.0f;
    Vector3 front;
    front.x = cos(pitch) * sin(yaw);
    front.y = sin(pitch);
    front.z = cos(pitch) * cos(yaw);
    front = front.normalized();
    look_at = position + front;
        Vector3 f = (look_at - position);
    Vector3 up_direction = {0, 1, 0};

    // Check if the forward vector is a zero vector
    if (f.magnitude() == 0) {
        std::cout << "fuck" << std::endl;
        // Handle the case where position and look_at are the same
        // Perhaps return an identity matrix or handle this case as needed
        return Matrix4::identity();
    }

    f = f.normalized(); // Forward

    // Check if up_direction is parallel to forward vector
    if (up_direction.cross(f).magnitude() == 0) {
        std::cout << "fuck" << std::endl;
        // Handle the case where up_direction is parallel to forward vector
        // Adjust up_direction or handle as needed
        up_direction = Vector3(0.4f, 0.4f, 0.6f);
        //up_direction = Vector3::some_other_vector(); // Replace with a valid vector
                                                     //
    }

    Vector3 r = up_direction.cross(f).normalized(); // Right
    Vector3 u = f.cross(r); // Up

    Matrix4 result;


    result.m[0] = r.x;
    result.m[1] = u.x;
    result.m[2] = -f.x;
    result.m[3] = 0.0f;

    result.m[4] = r.y;
    result.m[5] = u.y;
    result.m[6] = -f.y;
    result.m[7] = 0.0f;

    result.m[8] = r.z;
    result.m[9] = u.z;
    result.m[10] = -f.z;
    result.m[11] = 0.0f;

    result.m[12] = -r.dot(position);
    result.m[13] = -u.dot(position);
    result.m[14] = f.dot(position);
    result.m[15] = 1.0f;

    return result;
}

Vector3 Matrix4::multiply(const Vector3& vec)
{
    Vector3 result;
    result.x = m[0] * vec.x + m[4] * vec.y + m[8] * vec.z + m[12];
    result.y = m[1] * vec.x + m[5] * vec.y + m[9] * vec.z + m[13];
    result.z = m[2] * vec.x + m[6] * vec.y + m[10] * vec.z + m[14];
    float w = m[3] * vec.x + m[7] * vec.y + m[11] * vec.z + m[15];
    return result;
}

Matrix4 Matrix4::identity()
{
    Matrix4 new_matrix;
    float default_m[16] = {
    1.0f, 0.0f, 0.0f, 0.0f, // First row
    0.0f, 1.0f, 0.0f, 0.0f, // Second row
    0.0f, 0.0f, 1.0f, 0.0f, // Third row
    0.0f, 0.0f, 0.0f, 1.0f  // Fourth row
    };

    std::memcpy(new_matrix.m, default_m, 16 * sizeof(float));
    return new_matrix;

}

Matrix4 Matrix4::multiply_mat(Matrix4* _A, Matrix4* _B)
{
    float* A = _A->m;
    float* B = _B->m;

    Matrix4 new_matrix;
    new_matrix.m[0] = A[0] * B[0] + A[4] * B[1] + A[8] * B[2] + A[12] * B[3];
    new_matrix.m[1] = A[1] * B[0] + A[5] * B[1] + A[9] * B[2] + A[13] * B[3];
    new_matrix.m[2] = A[2] * B[0] + A[6] * B[1] + A[10] * B[2] + A[14] * B[3];
    new_matrix.m[3] = A[3] * B[0] + A[7] * B[1] + A[11] * B[2] + A[15] * B[3];

    // Next, compute the second column of the new matrix
    new_matrix.m[4] = A[0] * B[4] + A[4] * B[5] + A[8] * B[6] + A[12] * B[7];
    new_matrix.m[5] = A[1] * B[4] + A[5] * B[5] + A[9] * B[6] + A[13] * B[7];
    new_matrix.m[6] = A[2] * B[4] + A[6] * B[5] + A[10] * B[6] + A[14] * B[7];
    new_matrix.m[7] = A[3] * B[4] + A[7] * B[5] + A[11] * B[6] + A[15] * B[7];

    // Then, compute the third column of the new matrix
    new_matrix.m[8] = A[0] * B[8] + A[4] * B[9] + A[8] * B[10] + A[12] * B[11];
    new_matrix.m[9] = A[1] * B[8] + A[5] * B[9] + A[9] * B[10] + A[13] * B[11];
    new_matrix.m[10] = A[2] * B[8] + A[6] * B[9] + A[10] * B[10] + A[14] * B[11];
    new_matrix.m[11] = A[3] * B[8] + A[7] * B[9] + A[11] * B[10] + A[15] * B[11];

    // Finally, compute the fourth column of the new matrix
    new_matrix.m[12] = A[0] * B[12] + A[4] * B[13] + A[8] * B[14] + A[12] * B[15];
    new_matrix.m[13] = A[1] * B[12] + A[5] * B[13] + A[9] * B[14] + A[13] * B[15];
    new_matrix.m[14] = A[2] * B[12] + A[6] * B[13] + A[10] * B[14] + A[14] * B[15];
    new_matrix.m[15] = A[3] * B[12] + A[7] * B[13] + A[11] * B[14] + A[15] * B[15];
    return new_matrix;
 }
