#include "engine/renderer/renderer.h"

void Renderer::draw_line(int x0, int y0, int x1, int y1, uint32_t clr, int t)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    for (;;) {
        if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) {
            back_buffer[y0 * width + x0] = clr;
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }

}
