#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "engine/math/matrix.h"
#include "engine/core/base_components/geometry_component.h"
#include "engine/renderer/renderer.h"

#include <vector>

class RenderManager
{
public:
    Renderer* primitive_renderer;

    RenderManager();
    ~RenderManager();

    void set_active_view_matrix(Matrix4* view_projection_matrix);
    Matrix4* get_active_view_matrix();

    void clear_queue();
    void queue_geometry(GeometryComponent geometry);
    void render_geometry_queue();

    Vector3 world_to_screen(const Vector3& vec, int s_width, int s_height, bool is_combined);

private:
    Matrix4* view_projection_matrix = NULL;
    Matrix4 temporary_matrix;

    void render_geometry(std::size_t index);

    //should really be a pointer to the component, however, need to figure if rendering should be a seperate thread.
    std::vector<GeometryComponent> geometry_queue;
};

#endif
