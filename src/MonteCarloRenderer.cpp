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

            // We got a valid hit back, Send multiple new rays from this hit.
            /*for(i8 = 0; i<NUMRAYS; ++i)
            {

            }*/
            vec3 hitPosition = ray.origin + ray.direction*hit.distance + faces[hit.index].normal*0.01f;

            vec3 pixelLightVec = light - hitPosition;

            f32 distanceToLightSquared = glm::dot(pixelLightVec,pixelLightVec);

            Ray shadowRay;
            shadowRay.origin = hitPosition;// + faces[hit.index].normal;
            shadowRay.direction = glm::normalize( light - hitPosition );
            Hit shadowHit = rayTraceNode(shadowRay,0);

            vec3 pixelToHit = shadowRay.direction*shadowHit.distance;

            f32 distanceToHitSquared = glm::dot(pixelToHit,pixelToHit);

            if(distanceToHitSquared >= distanceToLightSquared)
                intensity = glm::dot(-ray.direction,faces[hit.index].normal)*matLib[materials[hit.index]].getDiffuseColor();
        }
    }

    return intensity;
}
