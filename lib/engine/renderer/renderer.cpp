#include "engine/renderer/renderer.h"
#include <iostream>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
//    delete[] front_buffer;
//    delete[] back_buffer;
//
    _aligned_free(front_buffer);
    _aligned_free(back_buffer);
}

void Renderer::update_config(int width, int height)
{
    this->width = width;
    this->height = height;

    //front_buffer = new uint32_t[width * height];
//    alignas(32) uint32_t* front_buffer = new uint32_t[width * height];
    //back_buffer = new uint32_t[width * height];
  front_buffer = static_cast<uint32_t*>(_aligned_malloc(width * height * sizeof(uint32_t), 32)); 
   back_buffer = static_cast<uint32_t*>(_aligned_malloc(width * height * sizeof(uint32_t), 32)); 

//    alignas(32) uint32_t* back_buffer = new uint32_t[width * height];

    std::fill_n(front_buffer, width * height, 0xFF00FFFF);
    std::fill_n(back_buffer, width * height, 0xFF00FFFF);
}

uint32_t* Renderer::get_front_buffer()
{
    return front_buffer;
}

int Renderer::get_width()
{
    return width;
}

int Renderer::get_height()
{
    return height;
}

void Renderer::cycle_start()
{
    std::fill_n(back_buffer, width * height, 0xFF000000);
}

void Renderer::cycle_end()
{
    uint32_t* temp = front_buffer;
    front_buffer = back_buffer;
    back_buffer = temp;
}
