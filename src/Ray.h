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
};

#endif
