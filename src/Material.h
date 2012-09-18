#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Types.h"
#include "Texture.h"

class Material
{
private:
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;

    Texture *texture[8];

    f32 opacity;
    f32 specularIntensity;
    f32 emission;
    f32 refractiveIndex;

public:

    Material()
    {
        opacity = 1.0f;
        specularIntensity = 10.0f;
        emission = 0.1f;
        refractiveIndex = 1.0f;

        for(int i=0; i<8; ++i)
            texture[i] = NULL;
    }

    void setAmbientColor(f32 r, f32 g, f32 b) { ambientColor = glm::clamp(vec3(r,g,b),0.0f,1.0f); }
    void setDiffuseColor(f32 r, f32 g, f32 b) { diffuseColor = glm::clamp(vec3(r,g,b),0.0f,1.0f); }
    void setSpecularColor(f32 r, f32 g, f32 b) { specularColor = glm::clamp(vec3(r,g,b),0.0f,1.0f); }

    void setOpacity(f32 val) { opacity = glm::clamp(val,0.0f,1.0f); }
    void setSpecularIntensity(f32 val) { specularIntensity = val; }
    void setEmission(f32 val) { emission = val; }
    void setTexture(u8 slot, Texture *tex) { if(slot<8) texture[slot] = tex; }
};

#endif
