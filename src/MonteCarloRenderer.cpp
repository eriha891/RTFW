#include "MonteCarloRenderer.h"
#define NUMRAYS 4

vec3 MonteCarloRenderer::rayTraceBVH(const Ray &ray)
{
    vec3 intensity;

    vec3 light(10,60,50);

    if(rayVsAABB(ray,nodes[0].aabb)<MAXFLOAT)
    {
        Hit hit = rayTraceNode(ray,0);
        if(hit < MAXFLOAT)
        {
			intensity = glm::dot(-ray.direction,faces[hit.index].normal)*matLib[materials[hit.index]].getDiffuseColor();
        }
    }

    return intensity;
}
