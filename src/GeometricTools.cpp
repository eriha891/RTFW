#include "GeometricTools.h"

f32 rayVsAABB(const Ray &ray, const AABB &aabb)
{

    return 0.0f;
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
