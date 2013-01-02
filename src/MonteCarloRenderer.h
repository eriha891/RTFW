#pragma once

#ifndef MONTECARLORENDERER_H
#define MONTECARLORENDERER_H

#include "Types.h"
#include "RenderDevice.h"

class MonteCarloRenderer : public RenderDevice
{
private:
    vec3 radiance(const Ray &ray);
    vec3 rayTraceBVH(const Ray &ray);
};

#endif
