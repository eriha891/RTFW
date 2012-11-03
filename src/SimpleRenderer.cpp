
#include "SimpleRenderer.h"

vec3 SimpleRenderer::rayTraceBVH(const Ray &ray)
{
    vec3 intensity;

    //if(rayVsAABB(ray,nodes[0].aabb)<MAXFLOAT)
    //{
        Hit hit = rayTraceNode(ray,0);
        // Did we get a valid hit back?
        if(hit < MAXFLOAT)
        {
            // Apply diffuse flat-shading
            intensity = glm::dot(-ray.direction,faces[hit.index].normal)*matLib[materials[hit.index]].getDiffuseColor();
        }
    //}

    return intensity;
}
