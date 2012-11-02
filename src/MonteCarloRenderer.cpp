#include "MonteCarloRenderer.h"
#define NUMRAYS 10
#define MAXDEPTH 3

inline vec3 reinhard(const vec3 &x)
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

vec3 MonteCarloRenderer::radiance(const Ray &ray, u8 depth)
{
    vec3 rad;

    if(depth < MAXDEPTH)
    {
        Hit hit = rayTraceNode(ray,0);
        if(hit < MAXFLOAT)
        {
            // calculate plane vectors
            vec3 n = faces[hit.index].normal;
            vec3 origin = ray.origin + ray.direction*hit.distance + n*.00001f;
            vec3 a = faces[hit.index].nOrt;
            vec3 b = glm::cross(n,a);
            for(int i=0; i<NUMRAYS; ++i)
            {
                f32 phi = glm::compRand1(0.0f,1.0f) * 360.0f;
                f32 r = glm::compRand1(0.0f,1.0f);

                Ray newRay(origin, a*r*glm::cos(phi) + b*r*glm::sin(phi) + n*glm::sqrt(1.0f-r*r));
                //printf("newray: %f %f %f, length: %f\n", newRay.direction.x, newRay.direction.y, newRay.direction.z, glm::length(newRay.direction));
                rad += matLib[materials[hit.index]].getEmission() + matLib[materials[hit.index]].getDiffuseColor()*radiance(newRay, depth+1);
            }
        }
    }
    /*else
    {
        rad = vec3(1.1,1.1,1.1);
    }*/

    return rad / static_cast<f32>(NUMRAYS);
}

inline vec3 MonteCarloRenderer::rayTraceBVH(const Ray &ray)
{
    return reinhard(radiance(ray,0));
}
