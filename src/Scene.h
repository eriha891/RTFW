#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Types.h"
#include "Geometry.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"

class Scene
{
public:
    std::vector<Geometry> geometry;
    std::vector<Material> material;
    std::vector<Camera> camera;
    std::vector<Light> light;
    u32 activeCamera;

    Scene()
    {
        activeCamera = 0;
    }

    void translate(f32 x, f32 y, f32 z)
    {
        for(u32 i=0; i<geometry.size(); ++i)
        {
            geometry[i].translate(x,y,z);
        }
    }
};

#endif
