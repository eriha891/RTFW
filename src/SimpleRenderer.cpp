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

f32 rayTraceNode(const Ray &ray, u32 nodeIndex, vec3 &intensity)
{
    //intensity = vec3(1.0,0.0,0.0);
    //return true;
    if(nodes[nodeIndex].isLeaf())
    {
        //return true;
        f32 hit;
        f32 closestHit = MAXFLOAT;
        u32 triangleIndex;

        for(u32 i=nodes[nodeIndex].getIndex(); i < nodes[nodeIndex].getIndex()+nodes[nodeIndex].getSize(); ++i)
        {
            hit = rayVsTriangle(ray,faces[i]);
            if(hit < closestHit)
            {
                closestHit = hit;
                triangleIndex = i;
            }
        }
        if(closestHit < MAXFLOAT)
        {
            intensity = glm::abs(glm::dot(-ray.direction,faces[triangleIndex].normal))*matLib[materials[triangleIndex]].getDiffuseColor();
            return closestHit;
        }
        //return false;
    }
    else
    {
        f32 distance_box_left = rayVsAABB(ray,nodes[nodes[nodeIndex].getLeft()].aabb);
        f32 distance_box_right = rayVsAABB(ray,nodes[nodes[nodeIndex].getRight()].aabb);

        f32 distance_leaf_left = MAXFLOAT;
        f32 distance_leaf_right = MAXFLOAT;

        vec3 intensity_leaf_left(1.0f);
        vec3 intensity_leaf_right(1.0f);
		
		if(distance_box_left < MAXFLOAT) {
            distance_leaf_left = rayTraceNode(ray,nodes[nodeIndex].getLeft(), intensity_leaf_left);
		}
		if(distance_box_right < MAXFLOAT) {
            distance_leaf_right = rayTraceNode(ray,nodes[nodeIndex].getRight(), intensity_leaf_right);
		}
		
		if(distance_leaf_left < distance_leaf_right) {
			intensity = intensity_leaf_left;
			return distance_leaf_left;
		} else if (distance_leaf_right < MAXFLOAT) {
			intensity = intensity_leaf_right;
			return distance_leaf_right;
		}
    }
    return MAXFLOAT;
}

vec3 rayTraceBVH(const Ray &ray)
{
    vec3 intensity;

    if(rayVsAABB(ray,nodes[0].aabb)<MAXFLOAT)
    {
        rayTraceNode(ray,0,intensity);
    }

    return intensity;
}

void SimpleRenderer::renderToArray(Scene *scene, f32 *intensityData, i32 resolutionX, i32 resolutionY)
{
    nodes.clear();
    materials.clear();
    matLib.clear();
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
            if(i >= scene->material.size())
                matLib.push_back(Material());
            else
                matLib.push_back(scene->material[i]);
        }

        createBVH(nodes, order, aabb, 12, 20);

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

            //if(rayTraceBVH(ray))
            //{
            vec3 intensity = rayTraceBVH(ray);
                int pos = (resolutionX*y + x) * 3;
                intensityData[pos + 0] = intensity.x;
                intensityData[pos + 1] = intensity.y;
                intensityData[pos + 2] = intensity.z;
            //}
        }
    }

    std::cout<<"Done!"<<std::endl;
}
