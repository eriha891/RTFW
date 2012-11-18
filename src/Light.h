#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Types.h"

class Light
{
public:
	vec3 position;
	vec3 color;
	f32 intensity;
};

#endif
