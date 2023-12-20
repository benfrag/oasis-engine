#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "engine/renderer/camera.h"

class Renderer3D
{
public:
    Renderer3D();
    ~Renderer3D();

    void set_camera(Camera* camera);

private:
    Camera* main_camera;

};


#endif
