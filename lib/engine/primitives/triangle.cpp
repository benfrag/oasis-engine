#include "engine/renderer/renderer.h"
#undef min
#undef max

#include <functional>
#include <cmath>
#include <immintrin.h> // AVX2 Intrinsics
#include <cstdint>
#include <algorithm>
#include <iostream>


#include <immintrin.h> // AVX2 intrinsics

struct TempVec2
{
    int x, y;
};
float edgeFunction(const TempVec2& a, const TempVec2& b, const TempVec2& c)
{
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void Renderer::draw_triangle(const ScreenVertex& v0, const ScreenVertex& v1, const ScreenVertex& v2, uint32_t clr)
{
  int minX = std::min({v0.x, v1.x, v2.x});
  int minY = std::min({v0.y, v1.y, v2.y});
  int maxX = std::max({v0.x, v1.x, v2.x});
  int maxY = std::max({v0.y, v1.y, v2.y});

  minX = std::max(minX, 0);
  minY = std::max(minY, 0);
  maxX = std::min(maxX, width - 1);
  maxY = std::min(maxY, height - 1);

   __m256i ones = _mm256_set1_epi32(1);
    __m256 a_x = _mm256_set1_ps(static_cast<float>(v0.x));
    __m256 a_y = _mm256_set1_ps(static_cast<float>(v0.y));
    __m256 b_x = _mm256_set1_ps(static_cast<float>(v1.x));
    __m256 b_y = _mm256_set1_ps(static_cast<float>(v1.y));
    __m256 c_x = _mm256_set1_ps(static_cast<float>(v2.x));
    __m256 c_y = _mm256_set1_ps(static_cast<float>(v2.y));

     // Compute area of the triangle (for barycentric coordinates)
    float area = edgeFunction({v0.x, v0.y}, {v1.x, v1.y}, {v2.x, v2.y});
    if (std::abs(area) < 1e-4) return; // Avoid division by zero

    __m256 v0_z = _mm256_set1_ps(v0.z);
    __m256 v1_z = _mm256_set1_ps(v1.z);
    __m256 v2_z = _mm256_set1_ps(v2.z);

    for (int y = minY; y <= maxY; y++)
    {
        int x = minX;
        // Handle 8 pixels at a time
        for (; x <= maxX - 8; x += 8)
        {
            __m256 px = _mm256_add_ps(_mm256_set1_ps(static_cast<float>(x)), _mm256_cvtepi32_ps(_mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7)));
            __m256 py = _mm256_set1_ps(static_cast<float>(y));

            // Edge functions
            __m256 abp = _mm256_sub_ps(_mm256_mul_ps(_mm256_sub_ps(b_x, a_x), _mm256_sub_ps(py, a_y)), _mm256_mul_ps(_mm256_sub_ps(b_y, a_y), _mm256_sub_ps(px, a_x)));
            __m256 bcp = _mm256_sub_ps(_mm256_mul_ps(_mm256_sub_ps(c_x, b_x), _mm256_sub_ps(py, b_y)), _mm256_mul_ps(_mm256_sub_ps(c_y, b_y), _mm256_sub_ps(px, b_x)));
            __m256 cap = _mm256_sub_ps(_mm256_mul_ps(_mm256_sub_ps(a_x, c_x), _mm256_sub_ps(py, c_y)), _mm256_mul_ps(_mm256_sub_ps(a_y, c_y), _mm256_sub_ps(px, c_x)));

            // Test edges
            __m256 mask = _mm256_and_ps(_mm256_and_ps(_mm256_cmp_ps(abp, _mm256_setzero_ps(), _CMP_GE_OQ), _mm256_cmp_ps(bcp, _mm256_setzero_ps(), _CMP_GE_OQ)), _mm256_cmp_ps(cap, _mm256_setzero_ps(), _CMP_GE_OQ));

             __m256 barycentric_u = _mm256_div_ps(abp, _mm256_set1_ps(area));
            __m256 barycentric_v = _mm256_div_ps(bcp, _mm256_set1_ps(area));
            __m256 barycentric_w = _mm256_div_ps(cap, _mm256_set1_ps(area));

            __m256 pixel_z = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(barycentric_u, v0_z), _mm256_mul_ps(barycentric_v, v1_z)), _mm256_mul_ps(barycentric_w, v2_z));


            // Write pixels
            int maski = _mm256_movemask_ps(mask);
            while (maski)
            {
                int bit = __builtin_ctz(maski);
                int index = y * width + x + bit;
                if (pixel_z[bit] < z_buffer[index])
                {
                    back_buffer[index] = clr;
                    z_buffer[index] = pixel_z[bit];
                }
                //back_buffer[y * width + x + bit] = clr;
                maski &= maski - 1;
            }
        }

        // Handle remaining pixels
        for (; x <= maxX; x++)
        {
            float ABP = edgeFunction({v0.x, v0.y}, {v1.x, v1.y}, {x, y});
            float BCP = edgeFunction({v1.x, v1.y}, {v2.x, v2.y}, {x, y});
            float CAP = edgeFunction({v2.x, v2.y}, {v0.x, v0.y}, {x, y});

            if (ABP >= 0 && BCP >= 0 && CAP >= 0)
            {
                // Barycentric coordinates for depth interpolation
                float u = ABP / area;
                float v = BCP / area;
                float w = CAP / area;

                // Interpolate z-value
                float pixel_z = u * v0.z + v * v1.z + w * v2.z;

                // Depth test against z-buffer
                int index = y * width + x;
                if (pixel_z < z_buffer[index])
                {
                    back_buffer[index] = clr;
                    z_buffer[index] = pixel_z;
                }
            }
        }
    }

}

/*struct TempVec2
{
    int x, y;
};
float edgeFunction(TempVec2 a, TempVec2 b, TempVec2 c)
{
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void Renderer::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t clr)
{
  int minX = std::min({x0, x1, x2});
  int minY = std::min({y0, y1, y2});
  int maxX = std::max({x0, x1, x2});
  int maxY = std::max({y0, y1, y2});

  minX = std::max(minX, 0);
  minY = std::max(minY, 0);
  maxX = std::min(maxX, width - 1);
  maxY = std::min(maxY, height - 1);

   __m256i ones = _mm256_set1_epi32(1);
    __m256 a_x = _mm256_set1_ps(static_cast<float>(x0));
    __m256 a_y = _mm256_set1_ps(static_cast<float>(y0));
    __m256 b_x = _mm256_set1_ps(static_cast<float>(x1));
    __m256 b_y = _mm256_set1_ps(static_cast<float>(y1));
    __m256 c_x = _mm256_set1_ps(static_cast<float>(x2));
    __m256 c_y = _mm256_set1_ps(static_cast<float>(y2));

    for (int y = minY; y <= maxY; y++)
    {
        int x = minX;
        // Handle 8 pixels at a time
        for (; x <= maxX - 8; x += 8)
        {
            __m256 px = _mm256_add_ps(_mm256_set1_ps(static_cast<float>(x)), _mm256_cvtepi32_ps(_mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7)));
            __m256 py = _mm256_set1_ps(static_cast<float>(y));

            // Edge functions
            __m256 abp = _mm256_sub_ps(_mm256_mul_ps(_mm256_sub_ps(b_x, a_x), _mm256_sub_ps(py, a_y)), _mm256_mul_ps(_mm256_sub_ps(b_y, a_y), _mm256_sub_ps(px, a_x)));
            __m256 bcp = _mm256_sub_ps(_mm256_mul_ps(_mm256_sub_ps(c_x, b_x), _mm256_sub_ps(py, b_y)), _mm256_mul_ps(_mm256_sub_ps(c_y, b_y), _mm256_sub_ps(px, b_x)));
            __m256 cap = _mm256_sub_ps(_mm256_mul_ps(_mm256_sub_ps(a_x, c_x), _mm256_sub_ps(py, c_y)), _mm256_mul_ps(_mm256_sub_ps(a_y, c_y), _mm256_sub_ps(px, c_x)));

            // Test edges
            __m256 mask = _mm256_and_ps(_mm256_and_ps(_mm256_cmp_ps(abp, _mm256_setzero_ps(), _CMP_GE_OQ), _mm256_cmp_ps(bcp, _mm256_setzero_ps(), _CMP_GE_OQ)), _mm256_cmp_ps(cap, _mm256_setzero_ps(), _CMP_GE_OQ));

            // Write pixels
            int maski = _mm256_movemask_ps(mask);
            while (maski)
            {
                int bit = __builtin_ctz(maski);
                back_buffer[y * width + x + bit] = clr;
                maski &= maski - 1;
            }
        }

        // Handle remaining pixels
        for (; x <= maxX; x++)
        {
            float ABP = edgeFunction({x0, y0}, {x1, y1}, {x, y});
            float BCP = edgeFunction({x1, y1}, {x2, y2}, {x, y});
            float CAP = edgeFunction({x2, y2}, {x0, y0}, {x, y});

            if (ABP >= 0 && BCP >= 0 && CAP >= 0)
            {
                back_buffer[y * width + x] = clr;
            }
        }
    }

}*/
