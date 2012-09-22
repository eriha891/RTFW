#include "MonteCarloRenderer.h"
#define NUMRAYS 10
#define MAXDEPTH 2

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
            vec3 a = glm::normalize(faces[hit.index].point[2]-faces[hit.index].point[0]);
            vec3 b = glm::cross(n,a);
            for(int i=0; i<NUMRAYS; ++i)
            {
                f32 phi = glm::compRand1(0.0f,1.0f) * 2.0f*PI;
                f32 height = glm::compRand1(0.0f,1.0f);
                f32 sqrtHeight = glm::sqrt(height);

                Ray newRay(ray.origin + ray.direction*hit.distance, glm::normalize(a*sqrtHeight*glm::cos(phi) + b*sqrtHeight*glm::sin(phi) + n*glm::sqrt(1.0f-height)));

                Material *mat = &matLib[materials[hit.index]];

                rad = mat->getEmission() + mat->getDiffuseColor()*radiance(newRay, depth+1);
            }
        }
    }

    return rad;
}

vec3 MonteCarloRenderer::rayTraceBVH(const Ray &ray)
{
    vec3 intensity;

/*    if(rayVsAABB(ray,nodes[0].aabb)<MAXFLOAT)
    {
        Hit hit = rayTraceNode(ray,0);
        if(hit < MAXFLOAT)
        {
            // calculate plane vectors
            vec3 n = faces[hit.index].normal;
            vec3 a = glm::normalize(faces[hit.index].point[2]-faces[hit.index].point[0]);
            vec3 b = glm::cross(n,a);
            for(int i=0; i<10; ++i)
            {
                f32 phi = glm::compRand1(0.0f,1.0f) * 2.0f*PI;
                f32 height = glm::compRand1(0.0f,1.0f);
                f32 sqrtHeight = glm::sqrt(height);

                Ray newRay(ray.origin, glm::normalize(a*sqrtHeight*glm::cos(phi) + b*sqrtHeight*glm::sin(phi) + n*glm::sqrt(1.0f-height)));

            }
			intensity = glm::dot(-ray.direction,faces[hit.index].normal)*matLib[materials[hit.index]].getDiffuseColor();
        }
    }
    */
    intensity = radiance(ray,0);

    return glm::sqrt(intensity);
}
