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
	f32 refractiveIndex;
	int originID;

	Ray() {
		refractiveIndex = AIR;
		originID = -1;
	};

	Ray(vec3 ori, vec3 dir, f32 refr=AIR, int oID = -1)
	{
	    origin = ori;
	    direction = dir;
		refractiveIndex = refr;
		originID = oID;
	}
};

#endif
