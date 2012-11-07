
#include "SimpleRenderer.h"

vec3 SimpleRenderer::rayTraceBVH(const Ray &ray)
{
    vec3 intensity;
    vec3 normal;

    //if(rayVsAABB(ray,nodes[0].aabb)<MAXFLOAT)
    //{
        Hit hit = rayTraceNode(ray,0);
        // Did we get a valid hit back?
        if(hit < MAXFLOAT)
        {
            normal = interpolateNormal(faces[hit.index], hit.baryCoords);
            // Apply diffuse flat-shading
            intensity = glm::dot(-ray.direction,normal)*matLib[materials[hit.index]].getDiffuseColor();
        }
    //}

    return intensity;
}
