#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "engine/math/matrix.h"
#include "engine/core/base_components/geometry_component.h"
#include "engine/renderer/renderer.h"
#include "engine/window_manager/window_manager.h"
#include "engine/math/vertex.h"

#include <vector>

class RenderManager
{
public:
    Renderer* primitive_renderer;
    WindowConfig* window_config;

    RenderManager();
    ~RenderManager();

    void set_active_view_matrix(Matrix4* view_projection_matrix);
    Matrix4* get_active_view_matrix();

    void clear_queue();
    void queue_geometry(GeometryComponent geometry);
    void render_geometry_queue();

    Vector4 world_to_vec4(const Vector3& vec);
    ScreenVertex vec4_to_screen(const Vector4& vec);
    ScreenVertex world_to_screen(const Vector3& vec);

private:
    Matrix4* view_projection_matrix = NULL;
    Matrix4 temporary_matrix;

    void render_geometry(std::size_t index);

    //should really be a pointer to the component, however, need to figure if rendering should be a seperate thread.
    std::vector<GeometryComponent> geometry_queue;
};

#endif
