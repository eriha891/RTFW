#pragma once

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Types.h"

/**
 *	Triangle
 *	A simple triangle with normals for each vertex,
 *	a normal for the plane and a helpvector nOrt that is
 *	orthogonal to the normal and is located in the plane.
 */

class Triangle
{
public:
    vec3 point[3];
    vec3 pointNormal[3];
    vec3 normal;
    vec3 nOrt;
};

#endif
