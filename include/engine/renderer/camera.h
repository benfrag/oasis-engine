#ifndef CAMERA_H
#define CAMERA_H

#include "engine/math/matrix.h"
#include "engine/math/vector.h"

#include <windows.h>

class Camera
{
public:
    Vector3 position;
    Vector3 look_at;
    Vector3 up_direction;
    Matrix4 view_matrix;
    Matrix4 projection_matrix;
    Matrix4 view_projection_matrix;

    float yaw = 0.0f;
    float pitch = 0.0f;

    Camera(Vector3 position, Vector3 look_at, Vector3 up_direction);
    void set_perspective_projection(float fov, float aspect_ratio, float near_plane, float far_plane);
    void set_orthographic_projection(float left, float right, float bottom, float top, float near_plane, float far_plane);

    void update_view_matrix();
    void move_to(Vector3 new_position);
    void look_to(Vector3 target);
    void move_forward_backward(float delta);
    void move_left_right(float delta);
    void fps_cycle(HWND hwnd);
};

#endif
