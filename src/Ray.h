#pragma once

#ifndef RAY_H
#define RAY_H

#include "Types.h"

/*
 *	Ray class
 *	
 *	origin and direction should be self exlpained.
 *	refractiveIndex is the refractive index of the medium that
 *	the ray is comming from.
 *	originID is the ID of the triangle that the ray is spawned from
 * 	and thus is immune to collisions against this triangle.
 *	direction should be normalized.
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
