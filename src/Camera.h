#pragma once
#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
    vec3 position;
    vec3 direction;
    vec3 up;

    Camera()
    {
        direction = vec3(0,0,-1);
        up = vec3(0,1,0);
    }
};

#endif
