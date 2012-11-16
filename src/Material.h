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
    vec3 emission;

    Texture *texture[8];

    f32 opacity;
    f32 specularFactor; // 0 is perfectly diffuse, 1 is perfectly specular
    f32 refractiveIndex;

public:

    Material()
    {
        //Defaultvalues
        opacity = 1.0f;             // Perfectly opaque
        specularFactor = 0.1f;      // Very diffuse
        refractiveIndex = 1.0f;     // Som refractive index? Air ≈ 1.0?
        diffuseColor = vec3(0.5f);  // Grey color

        for(int i=0; i<8; ++i)
            texture[i] = NULL;
    }

    inline const vec3 &getDiffuseColor() { return diffuseColor; }
    inline const vec3 &getEmission() { return emission; }
    inline const f32 &getSpecularFactor() { return specularFactor; }

    void setAmbientColor(f32 r, f32 g, f32 b) { ambientColor = glm::clamp(vec3(r,g,b),0.0f,1.0f); }
    void setDiffuseColor(f32 r, f32 g, f32 b) { diffuseColor = glm::clamp(vec3(r,g,b),0.0f,1.0f); }
    void setSpecularColor(f32 r, f32 g, f32 b) { specularColor = glm::clamp(vec3(r,g,b),0.0f,1.0f); }
    void setEmission(f32 r, f32 g, f32 b) { emission = glm::clamp(vec3(r,g,b),0.0f,MAXFLOAT); }

    void setOpacity(f32 val) { opacity = glm::clamp(val,0.0f,1.0f); }
    void setSpecularFactor(f32 val) { specularFactor = glm::clamp(val,0.0f,1.0f); }
    void setTexture(u8 slot, Texture *tex) { if(slot<8) texture[slot] = tex; }
};

#endif
