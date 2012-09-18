#pragma once

#ifndef MONTECARLORENDERER_H
#define MONTECARLORENDERER_H

#include "Types.h"
#include "RenderDevice.h"

class MonteCarloRenderer : public RenderDevice
{
public:
    void renderToArray(Scene *scene, f32 *intensityData, u32 resolutionX, u32 resolutionY);
};

#endif
