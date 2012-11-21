#pragma once

#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include "BVHTools.h"
#include "AABB.h"
#include "Triangle.h"
#include "Ray.h"
#include "Hit.h"
#include "GeometricTools.h"
#include "Types.h"

#include <vector>
#include "Scene.h"
#include <omp.h>
#include <stdio.h>

class RenderDevice
{
public:
    void renderToArray(Scene *scene, f32 *intensityData, int resolutionX, int resolutionY, int raysperpixel = 1);
    void buildBVH(Scene *scene);
private:
	virtual vec3 rayTraceBVH(const Ray &ray) = 0;
protected:
    virtual Hit rayTraceNode(const Ray &ray, u32 nodeIndex);
	std::vector<BVHNode> nodes;
	std::vector<Triangle> faces;
	std::vector<Light> lights;
	std::vector<u8> materials;
	std::vector<Material> matLib;
};

#endif
