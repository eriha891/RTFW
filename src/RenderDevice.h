#pragma once 

#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include "BVHTools.h"
#include "AABB.h"
#include "Triangle.h"
#include "Ray.h"
#include "GeometricTools.h"
#include "Types.h"

#include <vector>
#include "Scene.h"
#include <omp.h>

class RenderDevice
{
public:
    void renderToArray(Scene *scene, f32 *intensityData, i32 resolutionX, i32 resolutionY);
private:
	vec3 rayTraceBVH(const Ray &ray);
	virtual f32 rayTraceNode(const Ray &ray, u32 nodeIndex, vec3 &intensity) = 0;
protected:
	std::vector<BVHNode> nodes;
	std::vector<Triangle> faces;
	std::vector<u8> materials;
	std::vector<Material> matLib;
};

#endif
