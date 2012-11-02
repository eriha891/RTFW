#include <glm/gtx/extented_min_max.hpp>

#include "GeometricTools.h"

/**
* Found at http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
*/

f32 rayVsAABB(const Ray &ray, const AABB &aabb)
{
    // r.dir is unit direction vector of ray
    //vec3 dirfrac = 1.0f / ray.direction;

    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray

    vec3 a = (aabb._min - ray.origin) / ray.direction;
    vec3 b = (aabb._max - ray.origin) / ray.direction;

    /*float t1 = (aabb._min.x - ray.origin.x)*dirfrac.x;
    float t2 = (aabb._max.x - ray.origin.x)*dirfrac.x;
    float t3 = (aabb._min.y - ray.origin.y)*dirfrac.y;
    float t4 = (aabb._max.y - ray.origin.y)*dirfrac.y;
    float t5 = (aabb._min.z - ray.origin.z)*dirfrac.z;
    float t6 = (aabb._max.z - ray.origin.z)*dirfrac.z;*/

    f32 tmin = glm::max(glm::min(a.x, b.x), glm::min(a.y, b.y), glm::min(a.z, b.z));
    f32 tmax = glm::min(glm::max(a.x, b.x), glm::max(a.y, b.y), glm::max(a.z, b.z));

    //float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
    //float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

//    f32 tmin = glm::max(glm::min(a,b));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    // if tmin > tmax, ray doesn't intersect AABB
    if (tmax < 0.0f || tmin > tmax)
    {
        return MAXFLOAT;
    }

    return tmin;
}

bool barycentricTriangleIntersect(vec3 p, const Triangle &t)
{
    glm::vec3 v0 = t.point[2] - t.point[0];
    glm::vec3 v1 = t.point[1] - t.point[0];
    glm::vec3 v2 = p - t.point[0];

    f32 dot00 = glm::dot(v0,v0);
    f32 dot01 = glm::dot(v0,v1);
    f32 dot02 = glm::dot(v0,v2);
    f32 dot11 = glm::dot(v1,v1);
    f32 dot12 = glm::dot(v1,v2);

    f32 invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    f32 u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    f32 v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0.0f) && (v >= 0.0f) && (u + v < 1.0f);
}

f32 rayVsTriangle(const Ray &ray, const Triangle &triangle)
{
    f32 t = glm::dot(triangle.point[0]-ray.origin, triangle.normal) / glm::dot(ray.direction, triangle.normal);

    if(barycentricTriangleIntersect(ray.origin + ray.direction*t, triangle))
        return t;

    return MAXFLOAT;
}
