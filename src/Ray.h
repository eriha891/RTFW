#pragma once

#ifndef RAY_H
#define RAY_H

#include "Types.h"

/*
direction should be normalized.
*/

class Ray
{
public:
    vec3 origin;
    vec3 direction;
	f32 importance;

	Ray() {
		importance = 1.0;
	};

	Ray(vec3 ori, vec3 dir, f32 imp=1.0f)
	{
	    origin = ori;
	    direction = dir;
	    importance = imp;
	}
};

#endif
