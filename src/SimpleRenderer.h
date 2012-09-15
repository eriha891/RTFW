#pragma once

#ifndef SIMPLERENDERER_H
#define SIMPLERENDERER_H

#include "Types.h"
#include "RenderDevice.h"

class SimpleRenderer : public RenderDevice
{
public:
    void renderToArray(Scene *scene, f32 *intensityData, i32 resolutionX, i32 resolutionY);
};

#endif
