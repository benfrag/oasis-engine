#include "engine/renderer/renderer.h"

void Renderer::draw_rect(int x, int y, int w, int h, uint32_t clr)
{
    int x0 = max(x, 0);
    int x1 = min(x + w, width);
    int y0 = max(y, 0);
    int y1 = min(y + h, height);

    for (int iy = y0; iy < y1; ++iy) {
        for (int ix = x0; ix < x1; ++ix) {
            back_buffer[iy * width + ix] = clr;
        }
    }
}
