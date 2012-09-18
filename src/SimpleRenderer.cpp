#include <vector>
#include <iostream>
#include <omp.h>

#include "SimpleRenderer.h"
#include "BVHTools.h"
#include "AABB.h"
#include "Triangle.h"
#include "Ray.h"
#include "GeometricTools.h"

static std::vector<BVHNode> nodes;
static std::vector<Triangle> faces;
static std::vector<u8> materials;
static std::vector<Material> matLib;

bool rayTraceNode(const Ray &ray, u32 nodeIndex)
{
    if(nodes[nodeIndex].isLeaf())
    {
        //return true;
        for(u32 i=nodes[nodeIndex].getIndex(); i<nodes[nodeIndex].getIndex()+nodes[nodeIndex].getSize(); ++i)
        {
            if(rayVsTriangle(ray,faces[i])<MAXFLOAT)
                return true;
        }
    }
    else
    {
        f32 leftHit = rayVsAABB(ray,nodes[nodes[nodeIndex].getLeft()].aabb);
        f32 rightHit = rayVsAABB(ray,nodes[nodes[nodeIndex].getRight()].aabb);

        if(leftHit < rightHit)
        {
            if(leftHit<MAXFLOAT && rayTraceNode(ray,nodes[nodeIndex].getLeft()))
                return true;

            if(rightHit<MAXFLOAT && rayTraceNode(ray,nodes[nodeIndex].getRight()))
                return true;
        }
        else
        {
            if(rightHit<MAXFLOAT && rayTraceNode(ray,nodes[nodeIndex].getRight()))
                return true;

            if(leftHit<MAXFLOAT && rayTraceNode(ray,nodes[nodeIndex].getLeft()))
                return true;
        }
    }
    return false;
}

bool rayTraceBVH(const Ray &ray)
{
    if(rayVsAABB(ray,nodes[0].aabb)<MAXFLOAT)
    {
        return rayTraceNode(ray,0);
    }
    return false;
}

void SimpleRenderer::renderToArray(Scene *scene, f32 *intensityData, i32 resolutionX, i32 resolutionY)
{
    nodes.clear();
    materials.clear();
    faces.clear();

    // create the bvh-structure and reallign faces and materials to fit the bvh-structure.
    {
        std::vector<u32> order;
        std::vector<AABB> aabb;

        for(u32 i=0; i<scene->geometry.size(); ++i)
        {
            for(u32 u=0; u<scene->geometry[i].face.size(); ++u)
            {
                Triangle t;
                for(u8 p=0; p<3; ++p)
                {
                    t.point[p] = vec3(scene->geometry[i].vertex[ scene->geometry[i].face[u].point[p] ].x,
                                      scene->geometry[i].vertex[ scene->geometry[i].face[u].point[p] ].y,
                                      scene->geometry[i].vertex[ scene->geometry[i].face[u].point[p] ].z);
                    t.normal = scene->geometry[i].faceNormal[u];
                }

                faces.push_back(t);
                materials.push_back(i);

                AABB box;
                box.invertedMax();
                box.addFace(t.point[0], t.point[1], t.point[2]);
                aabb.push_back(box);

                order.push_back(order.size());
            }
        }

        createBVH(nodes, order, aabb, 12, 30);

        reorderVector(faces, order);
        reorderVector(materials, order);

        printf("faces size = %i \n", faces.size());
    }

    Camera *cam = &scene->camera[scene->activeCamera];

    // assumes FOV of 45° for simplicity

    vec3 eyePos = cam->position;
    vec3 eyeLeft = glm::normalize(glm::cross(cam->up, cam->direction));
    vec3 eyeUp = glm::normalize(glm::cross(cam->direction, eyeLeft));
    vec3 eyeForward = glm::normalize(cam->direction);

    vec3 topLeft = eyePos + eyeForward + eyeUp + eyeLeft;
    vec3 topRight = eyePos + eyeForward + eyeUp - eyeLeft;
    vec3 bottomLeft = eyePos + eyeForward - eyeUp + eyeLeft;

    vec3 right = topRight-topLeft;
    vec3 up = topLeft-bottomLeft;

    Ray ray;
    ray.origin = eyePos;

    printf("topLeft %f %f %f \n",topLeft.x,topLeft.y,topLeft.z);
    printf("topRight %f %f %f \n",topRight.x,topRight.y,topRight.z);
    printf("bottomLeft %f %f %f \n",bottomLeft.x,bottomLeft.y,bottomLeft.z);

    i32 x, y;
    f32 fResX = (f32)resolutionX;
    f32 fResY = (f32)resolutionY;

//#pragma omp parallel for
    for(y=0; y<resolutionY; ++y)
    {
        for(x=0; x<resolutionX; ++x)
        {
            ray.direction = glm::normalize(bottomLeft + ((f32)x+0.5f)/fResX * right + ((f32)y+0.5f)/fResY * up - ray.origin);

            if(rayTraceBVH(ray))
            {
                int pos = (resolutionX*y + x) * 3;
                intensityData[pos + 0] = 1;
                intensityData[pos + 1] = 1;
                intensityData[pos + 2] = 1;
            }
        }
    }

    std::cout<<"Done!"<<std::endl;
}
