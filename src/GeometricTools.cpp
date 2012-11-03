#include <glm/gtx/extented_min_max.hpp>

#include "GeometricTools.h"

/**
* Found at http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
*/

f32 rayVsAABB(const Ray &ray, const AABB &aabb)
{
    vec3 a = (aabb._min - ray.origin) / ray.direction;
    vec3 b = (aabb._max - ray.origin) / ray.direction;

    f32 tmin = glm::max(glm::min(a.x, b.x), glm::min(a.y, b.y), glm::min(a.z, b.z));
    f32 tmax = glm::min(glm::max(a.x, b.x), glm::max(a.y, b.y), glm::max(a.z, b.z));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    // if tmin > tmax, ray doesn't intersect AABB
    if (tmax < 0.0 || tmin > tmax)
    {
        return MAXFLOAT;
    }

    return tmin;
}

bool barycentricTriangleIntersect(vec3 &p, const Triangle &t)
{
    glm::vec3 v0 = t.point[2] - t.point[0];
    glm::vec3 v1 = t.point[1] - t.point[0];
    glm::vec3 v2 = p - t.point[0];

    f32 dot00 = glm::dot(v0,v0);
    f32 dot01 = glm::dot(v0,v1);
    f32 dot02 = glm::dot(v0,v2);
    f32 dot11 = glm::dot(v1,v1);
    f32 dot12 = glm::dot(v1,v2);

    f32 invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    f32 u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    f32 v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0.0) && (v >= 0.0) && (u + v < 1.0);
}

f32 rayVsTriangle(const Ray &ray, const Triangle &triangle)
{
    f32 t = glm::dot(triangle.point[0]-ray.origin, triangle.normal) / glm::dot(ray.direction, triangle.normal);

    vec3 pointInPlane = ray.origin + ray.direction*t;

    if(barycentricTriangleIntersect(pointInPlane, triangle))
        return t;

    return MAXFLOAT;
}
