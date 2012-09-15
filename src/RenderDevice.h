#pragma once

#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <vector>

#include "Scene.h"

class RenderDevice
{
public:
    virtual void renderToArray(Scene *scene, f32 *intensityData, i32 resolutionX, i32 resolutionY) = 0;
};

#endif
