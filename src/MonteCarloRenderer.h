#pragma once

#ifndef MONTECARLORENDERER_H
#define MONTECARLORENDERER_H

#include "Types.h"
#include "RenderDevice.h"

class MonteCarloRenderer : public RenderDevice
{
private:
    vec3 radiance(const Ray &ray, int depth = 0);
    vec3 localLighting(const Hit &hit, const Ray &ray);
    vec3 rayTraceBVH(const Ray &ray);
};

#endif
