#include "engine/renderer/renderer3d.h"

Renderer3D::Renderer3D()
{

}

Renderer3D::~Renderer3D()
{

}

void Renderer3D::set_camera(Camera* camera)
{
    main_camera = camera;
}
