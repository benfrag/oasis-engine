#include "engine/math/matrix.h"

struct CameraComponent
{
    Matrix4 view_matrix;
    Matrix4 projection_matrix;
    Matrix4 view_projection_matrix;

    float yaw = 0, pitch = 0; //replace with quaternions
    float edited_variable = 0;

    Vector3 last_position = {0, 0, 0};

    void update_view(Vector3 position)
    {
        view_matrix = Matrix4::create_view(position, yaw, pitch);
        view_projection_matrix = Matrix4::multiply_mat(&projection_matrix, &view_matrix);
    }

    Vector3 get_front()
    {
        Vector3 front;
        float yaw_rads = yaw * (3.14159f / 180.0f);
        float pitch_rads = pitch * (3.14159f / 180.0f);
        front.x = sin(yaw_rads) * cos(pitch_rads);
        front.y = 0; // Keep the camera level
        front.z = cos(yaw_rads) * cos(pitch_rads);
        front = front.normalized();
        return front;
    }

    void clamp_rotation()
    {
        if (pitch > 89.0f)
        {
            pitch = 89.0f;
        }
        else if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }

        if (yaw < -180.0f)
        {
            yaw += 360.0f;
        }
        else if (yaw > 180.0f)
        {
            yaw -= 360.0f;
        }

    }

    CameraComponent(float fov, float screen_width, float screen_height, float near_plane, float far_plane)
    {
        //update_view at start? with position that is not related to transform? need a better solution.
        projection_matrix = Matrix4::create_perspective(fov, screen_width / screen_height, near_plane, far_plane);
    }

    CameraComponent()
    {
        projection_matrix = Matrix4::create_perspective(45.0f, 800.f / 600.f, 0.1f, 100.f);
        //guessing that constructor is called when assigning in array, then data from the old instance is moved into the new instance.
    }
};

