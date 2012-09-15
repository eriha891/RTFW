#include <vector>
#include <iostream>
#include <omp.h>

#include "SimpleRenderer.h"
#include "BVHTools.h"
#include "AABB.h"
#include "Triangle.h"
#include "Ray.h"
#include "GeometricTools.h"

void SimpleRenderer::renderToArray(Scene *scene, f32 *intensityData, i32 resolutionX, i32 resolutionY)
{
    std::vector<BVHNode> nodes;
    std::vector<u8> materials;
    std::vector<Triangle> faces;

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
    }

    Camera *cam = &scene->camera[scene->activeCamera];

    // assumes FOV of 45° for simplicity

    vec3 eyePos = cam->position;
    vec3 eyeLeft = glm::normalize(glm::cross(cam->up, cam->direction));
    vec3 eyeUp = glm::normalize(glm::cross(cam->direction, eyeLeft));
    vec3 eyeForward = glm::normalize(cam->direction);

    vec3 topLeft = eyePos + cam->planeDistance * eyeForward + cam->planeDistance * eyeUp + cam->planeDistance * eyeLeft;
    vec3 topRight = eyePos + cam->planeDistance * eyeForward + cam->planeDistance * eyeUp - cam->planeDistance * eyeLeft;
    vec3 bottomLeft = eyePos + cam->planeDistance * eyeForward - cam->planeDistance * eyeUp + cam->planeDistance * eyeLeft;

    vec3 right = topRight-topLeft;
    vec3 up = topLeft-bottomLeft;

    Ray ray;
    ray.origin = eyePos;

    printf("topLeft %f %f %f \n",topLeft.x,topLeft.y,topLeft.z);
    printf("topRight %f %f %f \n",topRight.x,topRight.y,topRight.z);
    printf("bottomLeft %f %f %f \n",bottomLeft.x,bottomLeft.y,bottomLeft.z);

    // Test triangle!
    Triangle t;
    t.point[0] = vec3(-2,2,-10);
    t.point[1] = vec3(-2,-2,-10);
    t.point[2] = vec3(2,-2,-10);
    t.normal = vec3(0,0,1);

    i32 x, y;

//#pragma omp parallel for
    for(y=0; y<resolutionY; ++y)
    {
        for(x=0; x<resolutionX; ++x)
        {
            ray.direction = glm::normalize(bottomLeft + ((f32)x+0.5f)/(f32)(resolutionX-1) * right + ((f32)y+0.5f)/(f32)(resolutionY-1) * up - ray.origin);

            for(u32 i=0; i<10000; ++i)
            {
                if(rayVsTriangle(ray, faces[i]) < MAXFLOAT)
                {
                    int pos = (resolutionX*y + x) * 3;
                    intensityData[pos + 0] = 1;
                    intensityData[pos + 1] = 1;
                    intensityData[pos + 2] = 1;
                }
            }
        }
    }

    std::cout<<"Done!"<<std::endl;
}
