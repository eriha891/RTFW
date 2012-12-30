#include "MonteCarloRenderer.h"


inline vec3 reinhardTonemap(const vec3 &x)
{
    return x/(x + 1.0f);
}

vec3 uncharted2Tonemap(const vec3 &x)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;

    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 MonteCarloRenderer::radiance(const Ray &ray, int depth)
{
    vec3 rad;

    Hit hit = rayTraceNode(ray,0);
    if(hit < MAXFLOAT)
    {
        // set the kill-ray limit
		f32 limit = 0.9;

        // increase chance for first rays to survive
        //if(depth < 2) limit = 1.0;
        
		f32 r = glm::compRand1(0.0f,1.0f);
		
        Material *material = &matLib[materials[hit.index]];
        rad = material->getEmission();

        // calculate the surface normal
        vec3 n = interpolateNormal(faces[hit.index], hit.baryCoords);

        // check if ray is inside
        bool inside = (glm::dot(ray.direction,n) > 0.0) ? true : false;

        // check if the object is semi-transparent (guessing that no real object are perfect transparent)
        float opacity = material->getOpacity();
        
        if(r < limit)
        {
            if(inside) {
                float snell = material->getRefractiveIndex() / AIR;
                float neg = -1.0;
                n = n*neg;
                vec3 origin;
                vec3 newDirection;

                float angle = acos(glm::dot(n, ray.direction * (float) -1.0));
                float angle_lim = acos(ray.refractiveIndex/material->getRefractiveIndex());
                if(angle > angle_lim) {
					origin = ray.origin + ray.direction*hit.distance;
                    newDirection = glm::normalize(ray.direction - 2.0f * glm::dot(ray.direction,n) * n);
                } else {
					origin = ray.origin + ray.direction*hit.distance;
                    float thedot = glm::dot(n,ray.direction);
                    float factor = (snell * thedot - sqrt(1.0 - snell * snell *(1.0 - thedot*thedot)));
                    newDirection = glm::normalize(ray.direction*-snell + n * factor);
                }
                Ray extRay( origin, newDirection, material->getRefractiveIndex(), hit.index);
                rad += material->getDiffuseColor()*radiance(extRay, depth +1);  
            } else {
                r = r / limit;

                // calculate plane vectors
                vec3 origin = ray.origin + ray.direction*hit.distance;
                vec3 a = faces[hit.index].nOrt;
                vec3 b = glm::cross(n,a);

                // calculate perfect reflection
                vec3 reflection = ray.direction - 2.0f * glm::dot(ray.direction,n) * n;

                // calculate a perfect diffuse ray with random direction
                f32 phi = glm::compRand1(0.0f,2.0f*PI);
                vec3 diffuse = a*r*glm::cos(phi) + b*r*glm::sin(phi) + n*glm::sqrt(1.0f-r*r);

                // interpolationfaction between perfect reflectio nand perfect diffuse rays
                f32 t = material->getSpecularFactor();

                // Do a linear interpolation between a perfect diffuse and a perfect reflection according to specularFactor
                vec3 direction = glm::normalize(diffuse*(1.0f-t) + reflection*t);

                // the new ray that bounces of the surface
                Ray extRay( origin, direction, ray.refractiveIndex, hit.index);

                if(opacity < 1.0) {
                    
                    // the new ray that bounces into the object
                    float snell = ray.refractiveIndex / material->getRefractiveIndex();

                    float thedot = glm::dot(n,ray.direction);
                    float factor = (snell * thedot - sqrt(1.0 - snell * snell *(1.0 - thedot*thedot)));
                    vec3 intDirection = glm::normalize(ray.direction*-snell + n * factor);
                    //printf("hej hej");
                    Ray intRay(ray.origin + ray.direction*hit.distance, intDirection, material->getRefractiveIndex(), hit.index);

                    // calculate the totance emittance of this surface spot depending on tha opacity factor.
                    float invopacity = (1.0 - opacity);
                    if(opacity > 0.0)
                        rad += material->getDiffuseColor()*radiance(extRay, depth +1)*opacity;
                    rad += material->getDiffuseColor()*radiance(intRay, depth +1)*invopacity;
					//rad = vec3(0,0,1);
                    
                } else {
                    rad += material->getDiffuseColor()*radiance(extRay, depth +1);   
                }
            }
        }
    }

    return rad;
}

inline vec3 MonteCarloRenderer::rayTraceBVH(const Ray &ray)
{
    return glm::sqrt(radiance(ray, 0));
}
