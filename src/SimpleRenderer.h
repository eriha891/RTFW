#pragma once

#ifndef SIMPLERENDERER_H
#define SIMPLERENDERER_H

#include "Types.h"
#include "RenderDevice.h"

class SimpleRenderer : public RenderDevice
{
public:
    f32 rayTraceNode(const Ray &ray, u32 nodeIndex, vec3 &intensity);
};

#endif
