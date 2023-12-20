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

}

/*struct TempClr
{
    int r, g, b, a;
};

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


    for (int y = minY; y < maxY; y++)
    {
        for (int x = minX; x < maxX; x++)
        {
            float ABP = edgeFunction({x0, y0}, {x1, y1}, {x, y});
            float BCP = edgeFunction({x1, y1}, {x2, y2}, {x, y});
            float CAP = edgeFunction({x2, y2}, {x0, y0}, {x, y});

            float ABC = edgeFunction({x0, y0}, {x1, y1}, {x2, y2});
            if (ABP >= 0 && BCP >= 0 && CAP >= 0)
            {

            float weightA = BCP / ABC;
            float weightB = CAP / ABC;
            float weightC = ABP / ABC;

            TempClr colourA = {255, 0, 0, 255}; // Red
TempClr colourB = {0, 255, 0, 255}; // Green
TempClr colourC = {0, 0, 255, 255}; // Blue

            float r = colourA.r * weightA + colourB.r * weightB + colourC.r * weightC;
            float g = colourA.g * weightA + colourB.g * weightB + colourC.g * weightC;
            float b = colourA.b * weightA + colourB.b * weightB + colourC.b * weightC;

                back_buffer[y * width + x] = PACK(r, g, b, 255);
            }
        }
    }
}
*/
/*
// Represents a 2D point or vector
struct Vec2 {
    int x, y;
    // ... existing code ...
};

// Compute edge functions
__m256i edgeFunction(const Vec2& a, const Vec2& b) {
    return _mm256_setr_epi32(
        b.y - a.y, a.x - b.x, b.y - a.y, a.x - b.x,
        b.y - a.y, a.x - b.x, b.y - a.y, a.x - b.x);
}

// Batch processing for inside triangle check
__m256i isInsideTriangleBatch(const __m256i& w0, const __m256i& w1, const __m256i& w2) {
    __m256i zero = _mm256_setzero_si256();
    __m256i inside0 = _mm256_cmpgt_epi32(w0, zero);
    __m256i inside1 = _mm256_cmpgt_epi32(w1, zero);
    __m256i inside2 = _mm256_cmpgt_epi32(w2, zero);
    return _mm256_and_si256(_mm256_and_si256(inside0, inside1), inside2);
}

// Main rasterization function
void rasterizeTriangleAVX2(const Vec2& v0, const Vec2& v1, const Vec2& v2, uint32_t* framebuffer, int width, int height) {
    // Precompute edge functions
    __m256i e0 = edgeFunction(v0, v1);
    __m256i e1 = edgeFunction(v1, v2);
    __m256i e2 = edgeFunction(v2, v0);

    // Compute bounding box of the triangle
    int minX = std::max(std::min({v0.x, v1.x, v2.x}), 0);
    int maxX = std::min(std::max({v0.x, v1.x, v2.x}), width - 1);
    int minY = std::max(std::min({v0.y, v1.y, v2.y}), 0);
    int maxY = std::min(std::max({v0.y, v1.y, v2.y}), height - 1);

        // Rasterize in batches of 8 pixels
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; x += 8) {
            __m256i px = _mm256_setr_epi32(x, x+1, x+2, x+3, x+4, x+5, x+6, x+7);
            __m256i py = _mm256_set1_epi32(y);

            // Convert __m128i to __m256i
            __m256i e1_0 = _mm256_cvtepi32_epi64(_mm256_extracti128_si256(e1, 0));
            __m256i e1_1 = _mm256_cvtepi32_epi64(_mm256_extracti128_si256(e1, 1));
            __m256i e2_0 = _mm256_cvtepi32_epi64(_mm256_extracti128_si256(e2, 0));
            __m256i e2_1 = _mm256_cvtepi32_epi64(_mm256_extracti128_si256(e2, 1));
            __m256i e0_0 = _mm256_cvtepi32_epi64(_mm256_extracti128_si256(e0, 0));
            __m256i e0_1 = _mm256_cvtepi32_epi64(_mm256_extracti128_si256(e0, 1));

            // Compute barycentric coordinates for batch
            __m256i w0 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_sub_epi32(px, _mm256_set1_epi32(v2.x)), e1_0), 
                                          _mm256_mullo_epi32(_mm256_sub_epi32(py, _mm256_set1_epi32(v2.y)), e1_1));
            __m256i w1 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_sub_epi32(px, _mm256_set1_epi32(v0.x)), e2_0), 
                                          _mm256_mullo_epi32(_mm256_sub_epi32(py, _mm256_set1_epi32(v0.y)), e2_1));
            __m256i w2 = _mm256_add_epi32(_mm256_mullo_epi32(_mm256_sub_epi32(px, _mm256_set1_epi32(v1.x)), e0_0), 
                                          _mm256_mullo_epi32(_mm256_sub_epi32(py, _mm256_set1_epi32(v1.y)), e0_1));

           // Check if pixels are inside the triangle
            __m256i mask = isInsideTriangleBatch(w0, w1, w2);
            int mask_bits = _mm256_movemask_epi8(mask);
            for (int i = 0; i < 8; ++i) {
                if (mask_bits & (1 << i)) {
                    framebuffer[x + i + y * width] = 0xFFFFFFFF; // White color for demonstration
                }
            }
        }
    }
}

void Renderer::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t clr)
{
 // Ensure that (x0, y0) has the lowest y value
 

    Vec2 a = {x0, y0};
    Vec2 b = {x1, y1};
    Vec2 c = {x2, y2};


    rasterizeTriangleAVX2(a, b, c, back_buffer, width, height);

}
*/

   /*if (y1 < y0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    if (y2 < y0) {
        std::swap(x0, x2);
        std::swap(y0, y2);
    }*/






/*
void orderVerticesCCW(Vec2& a, Vec2& b, Vec2& c) {
    int area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (area < 0) {
        // Swap vertices to ensure counter-clockwise order
        std::swap(b, c);
        std::cout << "Swapped vertices for CCW order" << std::endl;
    }
}*/



/*bool isOffScreen(int x0, int y0, int x1, int y1, int x2, int y2, int width, int height) {
    int minX = std::min({x0, x1, x2});
    int maxX = std::max({x0, x1, x2});
    int minY = std::min({y0, y1, y2});
    int maxY = std::max({y0, y1, y2});

    return minX >= width || maxX < 0 || minY >= height || maxY < 0;
}
void Renderer::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t clr) {
        if (isOffScreen(x0, y0, x1, y1, x2, y2, width, height)) return;
    auto edge = [&](int x0, int y0, int x1, int y1, auto&& plot) {
        bool steep = abs(y1 - y0) > abs(x1 - x0);
        if (steep) {
            std::swap(x0, y0);
            std::swap(x1, y1);
        }
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx = x1 - x0;
        int dy = abs(y1 - y0);
        int error = dx / 2;
        int ystep = (y0 < y1) ? 1 : -1;
        int y = y0;
        for (int x = x0; x <= x1; x++) {
            if (steep) {
                plot(y, x);
            } else {
                plot(x, y);
            }
            error -= dy;
            if (error < 0) {
                y += ystep;
                error += dx;
            }
        }
    };

    auto drawLine = [&](int x0, int y0, int x1, int y1) {
        edge(x0, y0, x1, y1, [&](int x, int y) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                back_buffer[x + y * width] = clr;
            }
        });
    };

    // Sort vertices by y-coordinate
    if (y0 > y1) { std::swap(x0, x1); std::swap(y0, y1); }
    if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }
    if (y0 > y1) { std::swap(x0, x1); std::swap(y0, y1); }

    // Compute the inverse slopes
    float invSlope1 = (x1 - x0) / float(y1 - y0);
    float invSlope2 = (x2 - x0) / float(y2 - y0);
    float invSlope3 = (x2 - x1) / float(y2 - y1);

    // Draw the first half of the triangle
    float curx1 = x0;
    float curx2 = x0;
    for (int scanlineY = y0; scanlineY <= y1; scanlineY++) {
        drawLine((int)curx1, scanlineY, (int)curx2, scanlineY);
        curx1 += invSlope1;
        curx2 += invSlope2;
    }

    // Draw the second half of the triangle
    curx1 = x1;
    for (int scanlineY = y1; scanlineY <= y2; scanlineY++) {
        drawLine((int)curx1, scanlineY, (int)curx2, scanlineY);
        curx1 += invSlope3;
        curx2 += invSlope2;
    }
}
*/


/*
// Edge function
inline float edgeFunction(int ax, int ay, int bx, int by, int x, int y) {
    return (x - ax) * (by - ay) - (y - ay) * (bx - ax);
}

void Renderer::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t clr)
{
    // Calculate the signed area of the triangle
    float signedArea = edgeFunction(x0, y0, x1, y1, x2, y2);
    bool isCCW = signedArea >= 0;

    // Calculate bounding box of the triangle
    int minX = std::min({x0, x1, x2});
    int maxX = std::max({x0, x1, x2});
    int minY = std::min({y0, y1, y2});
    int maxY = std::max({y0, y1, y2});

    // Iterate only within the bounding box
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            // Use edge function for each vertex combination
            float w0 = edgeFunction(x1, y1, x2, y2, x, y);
            float w1 = edgeFunction(x2, y2, x0, y0, x, y);
            float w2 = edgeFunction(x0, y0, x1, y1, x, y);

            // Adjust sign for triangle orientation
            if (!isCCW) {
                w0 = -w0;
                w1 = -w1;
                w2 = -w2;
            }

            // Check if the point is inside the triangle
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                if (x >= 0 && x < width && y >= 0 && y < height) {
                    back_buffer[x + y * width] = clr;
                }
            }
        }
    }
}*/

/*
bool is_inside_triangle(int x, int y, int x0, int y0, int x1, int y1, int x2, int y2)
{
    // Compute vectors with floating-point precision
    float v0x = x2 - x0;
    float v0y = y2 - y0;
    float v1x = x1 - x0;
    float v1y = y1 - y0;
    float v2x = x - x0;
    float v2y = y - y0;

    // Compute dot products
    float dot00 = v0x * v0x + v0y * v0y;
    float dot01 = v0x * v1x + v0y * v1y;
    float dot02 = v0x * v2x + v0y * v2y;
    float dot11 = v1x * v1x + v1y * v1y;
    float dot12 = v1x * v2x + v1y * v2y;

    // Compute barycentric coordinates
    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

void Renderer::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t clr)
{
    // Ensure that (x0, y0) has the lowest y value
    if (y1 < y0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    if (y2 < y0) {
        std::swap(x0, x2);
        std::swap(y0, y2);
    }

    for (int y = y0; y <= y2; y++) {
        for (int x = std::min({x0, x1, x2}); x <= std::max({x0, x1, x2}); x++) {
            if (x >= 0 && x < width && y >= 0 && y < height && is_inside_triangle(x, y, x0, y0, x1, y1, x2, y2)) {
                back_buffer[x + y * width] = clr;
            }
        }
    }


}*/
