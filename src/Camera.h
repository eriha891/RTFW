#pragma once
#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
    vec3 position;
    vec3 direction;
    vec3 up;
    f32 planeDistance;
};

#endif
