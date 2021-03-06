#pragma once
#ifndef GEOMETRICTOOLS_H
#define GEOMETRICTOOLS_H

#include "Types.h"
#include "AABB.h"
#include "Ray.h"
#include "Triangle.h"

f32 rayVsAABB(const Ray &ray, const AABB &aabb);
f32 rayVsTriangle(const Ray &ray, const Triangle &triangle, vec3 &baryCoords);

vec3 interpolateNormal(const Triangle &triangle, const vec3 &baryCoords);

#endif
