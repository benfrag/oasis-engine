#include "engine/core/render_manager.h"
#include <iostream>
#include <array>

RenderManager::RenderManager()
{
    view_projection_matrix = &temporary_matrix;
}

RenderManager::~RenderManager()
{

}

void RenderManager::set_active_view_matrix(Matrix4* view_projection_matrix)
{
    this->view_projection_matrix = view_projection_matrix;
}

Matrix4* RenderManager::get_active_view_matrix()
{
    return view_projection_matrix;
}

Vector3 RenderManager::world_to_screen(const Vector3& vec, int s_width, int s_height, bool is_combined)
{
    Matrix4* matrix = view_projection_matrix;
    Vector3 result;
    float* m = matrix->m;
    result.x = m[0] * vec.x + m[4] * vec.y + m[8] * vec.z + m[12];
    result.y = m[1] * vec.x + m[5] * vec.y + m[9] * vec.z + m[13];
    result.z = m[2] * vec.x + m[6] * vec.y + m[10] * vec.z + m[14];
    float w  = m[3] * vec.x + m[7] * vec.y + m[11] * vec.z + m[15];

    if (w < 0.01f)
        return {0, 0, 0};

    Vector3 NDC;
    NDC.x = result.x / w;
    NDC.y = result.y / w;
    NDC.z = result.z / w;

    Vector3 screen;
    screen.x = (s_width / 2 * NDC.x) + (NDC.x + s_width / 2);
    screen.y = -(s_height / 2 * NDC.y) + (NDC.y + s_height / 2);
    screen.z = 0;
    return screen;
}

void RenderManager::queue_geometry(GeometryComponent geometry)
{
    geometry_queue.push_back(geometry);
}

void RenderManager::clear_queue()
{
    geometry_queue.clear();
}

void RenderManager::render_geometry(std::size_t index)
{
    GeometryComponent geometry = geometry_queue[index];

    if (geometry.vertices.size() % 3 != 0)
    {
        std::cout << "geometry vertices not multiple of 3." << std::endl;
        return;
    }

    for (std::size_t i = 0; i < geometry.vertices.size(); i += 3)
    {
        std::array<Vector3, 3> list_screen;
        std::transform(geometry.vertices.begin() + i, geometry.vertices.begin() + i + 3, list_screen.begin(),
                       [this](const Vector3& pt) {
                           return world_to_screen(pt, 800, 600, true);
                       });

        primitive_renderer->draw_triangle(list_screen[0].x, list_screen[0].y, list_screen[1].x, list_screen[1].y, list_screen[2].x, list_screen[2].y, geometry.clr);
    }
}

void RenderManager::render_geometry_queue()
{
    //need to sort triangles by z.
    std::size_t n = geometry_queue.size();
    for (std::size_t i = 0; i < n; ++i)
    {
        render_geometry(i);
    }
}
