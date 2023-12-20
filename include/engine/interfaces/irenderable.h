#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include <windows.h>
#include <cstdint>

class IRenderable
{
public:
    virtual uint32_t* get_front_buffer() = 0;
    virtual void update_config(int width, int height) = 0;
    virtual int get_width() = 0;
    virtual int get_height() = 0;
};

#endif
