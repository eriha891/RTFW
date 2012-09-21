#include "MonteCarloRenderer.h"

vec3 MonteCarloRenderer::rayTraceBVH(const Ray &ray)
{
    vec3 intensity;

    if(rayVsAABB(ray,nodes[0].aabb)<MAXFLOAT)
    {
        Hit hit = rayTraceNode(ray,0);
        if(hit < MAXFLOAT)
        {
            // We got a valid hit back, Send multiple new rays from this hit.
            intensity = glm::dot(-ray.direction,faces[hit.index].normal)*matLib[materials[hit.index]].getDiffuseColor();
        }
    }

    return intensity;
}
