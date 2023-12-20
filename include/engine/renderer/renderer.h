#ifndef RENDERER_H
#define RENDERER_H

#include <windows.h>
#include <algorithm>
#include "engine/interfaces/irenderable.h"

inline uint32_t PACK(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return (static_cast<uint32_t>(a) << 24) |
            (static_cast<uint32_t>(r) << 16) |
            (static_cast<uint32_t>(g) << 8) |
            static_cast<uint32_t>(b);
    }


class Renderer : public IRenderable
{
public:
    Renderer();
    ~Renderer();

    void update_config(int width, int height) override;
    uint32_t* get_front_buffer() override;
    int get_width() override;
    int get_height() override;

    void cycle_start();
    void cycle_end();

    void draw_rect(int x, int y, int w, int h, uint32_t clr);
    void draw_line(int x0, int y0, int x1, int y1, uint32_t clr, int t);
    void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t clr);

private:
    int width;
    int height;

    uint32_t* front_buffer;
    uint32_t* back_buffer;

};

#endif
