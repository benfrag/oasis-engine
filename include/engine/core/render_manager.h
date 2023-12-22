#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H
#include "engine/math/matrix.h"

class RenderManager
{
public:
    RenderManager();
    ~RenderManager();

    void set_active_view_matrix(Matrix4* view_projection_matrix);
    Matrix4* get_active_view_matrix();

    Vector3 world_to_screen(Vector3* vec, int s_width, int s_height, bool is_combined);



private:
    Matrix4* view_projection_matrix = NULL;
    Matrix4 temporary_matrix;
};

#endif
