#pragma once

#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <vector>

#include "RenderableObject.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include "Geometry.h"
#include "Scene.h"

class RenderDevice
{
public:
    virtual void Render(    const std::vector<RenderableObject*> &renderableObjects,
                            const std::vector<mat4*> &poses,
                            const std::vector<Material*> &materials,
                            const std::vector<Light*> &lights,
                            const Camera &camera) = 0;

    virtual void Render(const Scene &scene) = 0;
    virtual RenderableObject *CreateRenderableObject(Geometry &geometry) = 0;
};

#endif
