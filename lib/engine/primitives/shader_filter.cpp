#include "engine/renderer/renderer.h"
#undef min
#undef max

#include <cmath>
#include <cstdint>
#include <utility>

// Helper functions to mimic GLSL functions
float fract(float x) {
    return x - std::floor(x);
}

float smoothstep(float edge0, float edge1, float x) {
    float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

// Custom rand function to mimic GLSL rand
float rand(const std::pair<float, float>& co) {
    float dot_product = co.first * 12.9898f + co.second * 78.233f;
    return fract(sin(dot_product) * 43758.5453f);
}

// Vertical bar calculation
float verticalBar(float pos, float uvY, float offset, float range) {
    float edge0 = (pos - range);
    float edge1 = (pos + range);

    float x = smoothstep(edge0, pos, uvY) * offset;
    x -= smoothstep(pos, edge1, uvY) * offset;
    return x;
}

// Main effect application function
void applyEffect(uint32_t* frame_buffer, int width, int height, float iTime, 
                 float noiseQuality, float noiseIntensity, float offsetIntensity, float colorOffsetIntensity) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Convert x, y to UV coordinates
            float uvX = static_cast<float>(x) / width;
            float uvY = static_cast<float>(y) / height;

            // Apply vertical bar logic
            for (float i = 0.0f; i < 0.71f; i += 0.1313f) {
                float d = std::fmod(iTime * i, 1.7f);
                float o = sin(1.0f - tan(iTime * 0.24f * i)) * offsetIntensity;
                uvX += verticalBar(d, uvY, o, 0.05f); // range is hardcoded as 0.05
            }

            // Apply noise
            uvY *= noiseQuality;
            uvY = static_cast<float>(static_cast<int>(uvY)) * (1.0f / noiseQuality);
            float noise = rand({iTime * 0.00001f, uvY});
            uvX += noise * noiseIntensity;

            // Get the pixel at the new UV coordinates
            int newX = static_cast<int>(uvX * width) % width;
            int newY = static_cast<int>(uvY * height) % height;
            uint32_t& pixel = frame_buffer[newY * width + newX];
            if (pixel == 0xFF000000)
                continue;

            // Extract RGB components
            uint8_t r = (pixel >> 16) & 0xFF;
            uint8_t g = (pixel >> 8) & 0xFF;
            uint8_t b = pixel & 0xFF;

            // Apply color offset (dummy implementation)
            // This is where you would implement your specific color manipulation logic
            // Based on your shader code, this might involve shifting color channels
            r = std::min(255, static_cast<int>(r * colorOffsetIntensity));
            g = std::min(255, static_cast<int>(g * colorOffsetIntensity));
            b = std::min(255, static_cast<int>(b * colorOffsetIntensity));

            // Repack the RGB components into the pixel
            pixel = (r << 16) | (g << 8) | b;
        }
    }
}

//float g_itime = 0;
void Renderer::shader_filter_hook(float dt)
{
 //   g_itime += dt;
//    applyEffect(back_buffer, width, height, g_itime, 250.0f, 0.0088f, 0.02f, 1.3f);
}
