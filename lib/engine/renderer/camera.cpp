#include "engine/renderer/camera.h"
#include <cmath>
#include <iostream>
#include <windows.h>

Camera::Camera(Vector3 position, Vector3 look_at, Vector3 up_direction) : position(position), look_at(look_at), up_direction(up_direction)
{
    set_perspective_projection(90.0f, 1280 / 720, 0.1f, 100.0f);
    update_view_matrix();
}

void Camera::set_perspective_projection(float fov, float aspect_ratio, float near_plane, float far_plane)
{
    projection_matrix = Matrix4::create_perspective(fov, aspect_ratio, near_plane, far_plane);

}
void Camera::set_orthographic_projection(float left, float right, float bottom, float top, float near_plane, float far_plane)
{
    projection_matrix = Matrix4::create_orthographic(left, right, bottom, top, near_plane, far_plane);
}

void Camera::update_view_matrix()
{
    view_matrix = Matrix4::create_view(position, yaw, pitch);
    view_projection_matrix = Matrix4::multiply_mat(&projection_matrix, &view_matrix);
}

void Camera::move_to(Vector3 new_position)
{
    position = new_position;
    update_view_matrix();
}

void Camera::look_to(Vector3 target)
{
    look_at = target;
    update_view_matrix();
}

// Method for forward and backward movement
void Camera::move_forward_backward(float delta)
{
    Vector3 front;
    float yaw_rads = yaw * (3.14159f / 180.0f);
    float pitch_rads = pitch * (3.14159f / 180.0f);
    front.x = sin(yaw_rads) * cos(pitch_rads);
    front.y = 0; // Keep the camera level
    front.z = cos(yaw_rads) * cos(pitch_rads);
    front = front.normalized();

    position = position + (front * delta);
}

// Method for left and right movement
void Camera::move_left_right(float delta)
{
    Vector3 front;
    float yaw_rads = yaw * (3.14159f / 180.0f);
    float pitch_rads = pitch * (3.14159f / 180.0f);
    front.x = sin(yaw_rads) * cos(pitch_rads);
    front.y = 0; // Keep the camera level
    front.z = cos(yaw_rads) * cos(pitch_rads);

    front.new_normalize();
    Vector3 right = Vector3::new_cross(Vector3(0, 1, 0), front);
    right.new_normalize();

    position = position + (right * delta);
}

void Camera::fps_cycle(HWND hwnd)
{
       RECT rect;
    GetClientRect(hwnd, &rect);

    // Calculate the center of the window
    int centerX = rect.left + (rect.right - rect.left) / 2;
    int centerY = rect.top + (rect.bottom - rect.top) / 2;

    // Convert client coordinates to screen coordinates
    POINT centerScreen = { 1280/2, 720/2 };
    ClientToScreen(hwnd, &centerScreen);

    // Get the current cursor position
    POINT currentMousePos;
    GetCursorPos(&currentMousePos);

    // Calculate the delta
    POINT delta = { currentMousePos.x - centerScreen.x + 8, currentMousePos.y - centerScreen.y + 31 };


        float sensitivity = 0.1;

        yaw += delta.x * sensitivity;
        pitch -= delta.y * sensitivity; // Reversed since y-coordinates range from bottom to top

        // Clamp pitch to prevent flipping
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        // Normalize the yaw to be within the range 0 - 360 degrees
if (yaw < -180.0f) {
    yaw += 360.0f;
}
else if (yaw > 180.0f) {
    yaw -= 360.0f;
}


//        std::cout << "yaw : " << yaw << std::endl;
//        std::cout << "pitch : " << pitch << std::endl;

        // Convert yaw and pitch to a direction vector
        Vector3 direction;
        direction.x = cos(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);
        direction.y = sin(pitch * 3.14159f / 180.0f);
        direction.z = sin(yaw * 3.14159f / 180.0f) * -cos(pitch * 3.14159f / 180.0f);
        direction = direction.normalized();

        // Update the look_at vector
        look_at = position + direction;

        // Update view matrix if needed

    if (GetAsyncKeyState(0x57)) //W
    {
        move_forward_backward(0.1);

    }
    if (GetAsyncKeyState(0x53)) //S
    {
       move_forward_backward(-0.1);
//        position.z -= 0.1;
    }
    if (GetAsyncKeyState(0x44)) //D
    {
        move_left_right(0.1);

//        position.x += 0.1;
    }
    if (GetAsyncKeyState(0x41)) //A
    {
        move_left_right(-0.1);
  //      position.x -= 0.1;
    }
    if (GetAsyncKeyState(VK_SPACE))
    {
        position.y += 0.1;
    }
    if (GetAsyncKeyState(VK_CONTROL))
    {
        position.y -= 0.1;
    }
        update_view_matrix();
}
