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
}

#endif
