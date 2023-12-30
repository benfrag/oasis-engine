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

ScreenVertex RenderManager::world_to_screen(const Vector3& vec)
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

    ScreenVertex screen;
    int width = window_config->width;
    int height = window_config->height;

    screen.x = static_cast<int>((1 + NDC.x) * width * 0.5);
    screen.y = static_cast<int>((1 - NDC.y) * height * 0.5);
    screen.z = w;
    return screen;
}

Vector4 RenderManager::world_to_vec4(const Vector3& vec)
{
    Matrix4* matrix = view_projection_matrix;
    Vector4 result;
    float* m = matrix->m;
    result.x = m[0] * vec.x + m[4] * vec.y + m[8] * vec.z + m[12];
    result.y = m[1] * vec.x + m[5] * vec.y + m[9] * vec.z + m[13];
    result.z = m[2] * vec.x + m[6] * vec.y + m[10] * vec.z + m[14];
    result.w  = m[3] * vec.x + m[7] * vec.y + m[11] * vec.z + m[15];
    return result;
}

ScreenVertex RenderManager::vec4_to_screen(const Vector4& vec)
{
    Vector3 NDC;
    NDC.x = vec.x / vec.w;
    NDC.y = vec.y / vec.w;
    NDC.z = vec.z / vec.w;

    ScreenVertex screen;
    int width = window_config->width;
    int height = window_config->height;

    screen.x = static_cast<int>((1 + NDC.x) * width * 0.5);
    screen.y = static_cast<int>((1 - NDC.y) * height * 0.5);
    screen.z = vec.w;
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

#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<uint32_t> dis(0, 0xFFFF);

uint32_t temp_random_uint32()
{
    uint32_t randomPart = dis(gen);
    uint32_t result = (0x64 << 16) | randomPart;
    return result;
}

/*bool clip_triangle(const std::array<Vector4, 3>& vertices, std::vector<Vector4>& clipped_vertices, float near_plane)
{
    auto interpolate = [](const Vector4& v1, const Vector4& v2, float t)
    {
        return Vector4(
            v1.x + (v2.x - v1.x) * t,
            v1.y + (v2.y - v1.y) * t,
            v1.z + (v2.z - v1.z) * t,
            v1.w + (v2.w - v1.w) * t
        );
    };

    std::array<bool, 3> in_front;
    for (int i = 0; i < 3; ++i)
        in_front[i] = !(vertices[i].z + vertices[i].w < 0);
        //in_front[i] = vertices[i].z >= near_plane;

    // Check if all vertices are in front or behind the near plane
    if (in_front[0] == in_front[1] && in_front[1] == in_front[2])
    {
        if (in_front[0]) // All vertices are in front of the near plane
        {
            clipped_vertices.insert(clipped_vertices.end(), vertices.begin(), vertices.end());
            return true;
        }
        return false; // Triangle is completely behind the near plane
    }

    // Process each edge
    for (int i = 0; i < 3; ++i)
    {
        const Vector4& v1 = vertices[i];
        const Vector4& v2 = vertices[(i + 1) % 3];

        if (in_front[i] == in_front[(i + 1) % 3])
        {
            if (in_front[i])
                clipped_vertices.push_back(v1);
        }
        else
        {
            float t = (near_plane - v1.z) / (v2.z - v1.z);
            Vector4 interpolated = interpolate(v1, v2, t);
            if (in_front[i])
            {
                clipped_vertices.push_back(v1);
                clipped_vertices.push_back(interpolated);
            }
            else
                clipped_vertices.push_back(interpolated);
        }
    }

    return true;
}*/

/*bool clip_triangle(const std::array<Vector4, 3>& vertices, std::vector<Vector4>& clipped_vertices, float near_plane)
{
    auto interpolate = [](const Vector4& start, const Vector4& end, float t) {
        // Perspective-correct interpolation factor
        float w = 1.0f / (t / end.w + (1.0f - t) / start.w);
        float x = (start.x + t * (end.x - start.x)) * w;
        float y = (start.y + t * (end.y - start.y)) * w;
        float z = (start.z + t * (end.z - start.z)) * w;
        return Vector4(x, y, z, w);
    };

    // Calculate the distances of each vertex from the near plane
    std::array<float, 3> distances;
    for (int i = 0; i < 3; ++i) {
        distances[i] = vertices[i].w + vertices[i].z;  // Distance from the near plane
    }

    // Check if all vertices are on the same side of the near plane
    bool allInFront = true;
    bool allBehind = true;
    for (int i = 0; i < 3; ++i) {
        if (distances[i] < 0) allInFront = false;
        else allBehind = false;
    }

    // If all vertices are behind the near plane, discard the triangle
    if (allBehind) {
        return false;
    }

    // If all vertices are in front of the near plane, keep the triangle as it is
    if (allInFront) {
        clipped_vertices.insert(clipped_vertices.end(), vertices.begin(), vertices.end());
        return true;
    }

    // Clip each edge
    for (int i = 0; i < 3; ++i) {
        const Vector4& current = vertices[i];
        const Vector4& next = vertices[(i + 1) % 3];

        float currentDistance = distances[i];
        float nextDistance = distances[(i + 1) % 3];

        bool currentInFront = currentDistance >= 0;
        bool nextInFront = nextDistance >= 0;

        if (currentInFront) {
            clipped_vertices.push_back(current);
        }

        // If the vertices are on opposite sides of the plane, clip the edge
        if (currentInFront != nextInFront) {
            float t = currentDistance / (currentDistance - nextDistance);
            clipped_vertices.push_back(interpolate(current, next, t));
        }
    }

    return !clipped_vertices.empty();
}*/


bool clip_triangle(const std::array<Vector4, 3>& vertices, std::vector<Vector4>& clipped_vertices, float near_plane)
{
    // Lambda for perspective-correct interpolation
    auto interpolate = [](const Vector4& start, const Vector4& end, float t) -> Vector4 {
        return Vector4(
            start.x + t * (end.x - start.x),
            start.y + t * (end.y - start.y),
            start.z + t * (end.z - start.z),
            start.w + t * (end.w - start.w)
        );
    };

    // This will hold whether each vertex is in front of the near plane
    std::array<bool, 3> in_front;
    for (int i = 0; i < 3; ++i) {
        in_front[i] = (vertices[i].w + vertices[i].z >= 0);
    }

    // Check all three vertices against the near plane
    for (int i = 0; i < 3; ++i) {
        int next = (i + 1) % 3;
        const Vector4& current_vertex = vertices[i];
        const Vector4& next_vertex = vertices[next];
        
        if (in_front[i]) {
            // Current vertex is in front; we can keep it
            clipped_vertices.push_back(current_vertex);
        }

        // Check if we have an edge that crosses the plane
        bool next_in_front = (next_vertex.w + next_vertex.z >= 0);
        if (in_front[i] != next_in_front) {
            // Compute intersection t value (how far along the edge the intersection is)
            float t = (near_plane - current_vertex.w) / (next_vertex.w - current_vertex.w);
            Vector4 intersection = interpolate(current_vertex, next_vertex, t);
            clipped_vertices.push_back(intersection);
        }
    }

    // If after clipping there are no vertices left, the triangle is completely behind the near plane
    if (clipped_vertices.empty()) {
        return false;
    }

    // At this point, clipped_vertices could contain between 3 to 6 vertices, and if it's more than 3,
    // we need to form a triangle fan from these vertices for proper rendering.
    // Note: This assumes clipped_vertices form a convex set, which should be true for clipping against a single plane.
    
    // Clipping can generate a maximum of 4 vertices if the original triangle straddles the plane
    if (clipped_vertices.size() > 4) {
        std::vector<Vector4> temp_vertices(clipped_vertices);
        clipped_vertices.clear();

        // Re-triangulate the polygon into triangles
        for (size_t i = 1; i < temp_vertices.size() - 1; ++i) {
            clipped_vertices.push_back(temp_vertices[0]);
            clipped_vertices.push_back(temp_vertices[i]);
            clipped_vertices.push_back(temp_vertices[i + 1]);
        }
    }

    return true;
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
        /*std::array<ScreenVertex, 3> list_screen;
        std::transform(geometry.vertices.begin() + i, geometry.vertices.begin() + i + 3, list_screen.begin(),
                       [this](const Vector3& pt) {
                           return world_to_screen(pt);
                       });

        primitive_renderer->draw_triangle(list_screen[0], list_screen[1], list_screen[2], geometry.clr);*/

        std::array<ScreenVertex, 3> list_screen;
        int num_clipped = 0;
 /*       std::transform(geometry.vertices.begin() + i, geometry.vertices.begin() + i + 3, list_screen.begin(), [this, &num_clipped](const Vector3& pt) {
            Vector4 mvec = world_to_vec4(pt);
            if (mvec.z + mvec.w < 0)
            {
                num_clipped++;

            }
            return vec4_to_screen(mvec);
});*/
        //std::array<ScreenVertex, 3> list_screen;
        std::array<Vector4, 3> manipulated_v;
        std::vector<int> clipped_indices;
        std::vector<Vector4> clipped_vertices;


        std::size_t n = 3;
        for (std::size_t j = 0; j < 3; ++j)
        {
            Vector3 pt = geometry.vertices[i + j];
            manipulated_v[j] = world_to_vec4(pt);
            if (manipulated_v[j].z + manipulated_v[j].w < 0)
                num_clipped++;
            list_screen[j] = vec4_to_screen(manipulated_v[j]);
        }

//        bool clipped = clip_triangle(manipulated_v, clipped_vertices, 0.1f);
//        if (!clipped)
//           continue;
//        for (std::size_t j = 0; j < clipped_vertices.size(); ++j)
//        {
//            if (j > 2)
//                break;
//            list_screen[j] = vec4_to_screen(clipped_vertices[j]);
//        }

        uint32_t temp_clr = geometry.clr;
/*        if (num_clipped == 0)
            temp_clr = 0xFFeb34e8;
        if (num_clipped == 1)
            temp_clr = 0xFF344feb;
        if (num_clipped == 2)
            temp_clr = 0xFFd9eb34;
            */

        if (num_clipped > 0)
            continue;
            
        primitive_renderer->draw_triangle(list_screen[0], list_screen[1], list_screen[2], temp_clr);


//        uint32_t random_clr = temp_random_uint32();
//        primitive_renderer->draw_triangle(list_screen[0], list_screen[1], list_screen[2], random_clr);

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
