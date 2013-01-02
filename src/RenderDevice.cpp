#include "RenderDevice.h"

Hit RenderDevice::rayTraceNode(const Ray &ray, u32 nodeIndex)
{
    // Handle Leaf
    if(nodes[nodeIndex].isLeaf())
    {
        f32 hit;
        f32 closestHit = MAXFLOAT;
        u32 triangleIndex=0;
        vec3 hitBaryCoords;
        vec3 baryCoords;

        for(u32 i=nodes[nodeIndex].getIndex(); i < nodes[nodeIndex].getIndex()+nodes[nodeIndex].getSize(); ++i)
        {
            if(i != ray.originID) {
                hit = rayVsTriangle(ray,faces[i],hitBaryCoords);
                if(hit > 0.0 && hit < closestHit)
                {
                    closestHit = hit;
                    triangleIndex = i;
                    baryCoords = hitBaryCoords;
                }
            }
        }
        if(closestHit < MAXFLOAT)
        {
            return Hit(closestHit, triangleIndex, baryCoords);
        }
    }
    else
    {
        Hit leaf_left_hit(MAXFLOAT,0);
        Hit leaf_right_hit(MAXFLOAT,0);

		if(rayVsAABB(ray,nodes[nodes[nodeIndex].getLeft()].aabb) < MAXFLOAT) {
            leaf_left_hit = rayTraceNode(ray,nodes[nodeIndex].getLeft());
		}
		if(rayVsAABB(ray,nodes[nodes[nodeIndex].getRight()].aabb) < MAXFLOAT) {
            leaf_right_hit = rayTraceNode(ray,nodes[nodeIndex].getRight());
		}

		if(leaf_left_hit < leaf_right_hit) {
			return leaf_left_hit;
		} else if (leaf_right_hit.distance < MAXFLOAT) {
			return leaf_right_hit;
		}
    }
    return Hit(MAXFLOAT,0);
}

void RenderDevice::buildBVH(Scene *scene)
{
    nodes.clear();
    materials.clear();
    matLib.clear();
    faces.clear();

    // create the bvh-structure
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

                    t.pointNormal[p] = vec3(scene->geometry[i].vertex[ scene->geometry[i].face[u].point[p] ].nx,
                                            scene->geometry[i].vertex[ scene->geometry[i].face[u].point[p] ].ny,
                                            scene->geometry[i].vertex[ scene->geometry[i].face[u].point[p] ].nz);

                }

                t.normal = scene->geometry[i].faceNormal[u];
                t.nOrt = glm::normalize(t.point[0] - t.point[2]);

                faces.push_back(t);
                materials.push_back(i);

                AABB box(&t.point[0][0],9);
                aabb.push_back(box);

                order.push_back(order.size());
            }
            if(i >= scene->material.size())
                matLib.push_back(Material());
            else
                matLib.push_back(scene->material[i]);
        }

        // create a BVH-structure in nodes
        BVH::createBVH(nodes, order, aabb, 10, 30);

        // reallign faces and materials to fit the bvh-structure.
        BVH::reorderVector(faces, order);
        BVH::reorderVector(materials, order);

        printf("faces size = %i \n", (int)faces.size());
    }
}
void RenderDevice::renderToArray(Scene *scene, f32 *intensityData, int resolutionX, int resolutionY, int raysperpixel)
{


    Camera *cam = &scene->camera[scene->activeCamera];

    // assumes FOV of 45° for simplicity

    vec3 eyePos = cam->position;
    vec3 eyeLeft = glm::normalize(glm::cross(cam->up, cam->direction));
    vec3 eyeUp = glm::normalize(glm::cross(cam->direction, eyeLeft));
    vec3 eyeForward = glm::normalize(cam->direction);

	eyeLeft = eyeLeft* 0.5f;
	eyeUp = eyeUp* 0.5f;

    vec3 topLeft = eyePos + eyeForward + eyeUp + eyeLeft;
    vec3 topRight = eyePos + eyeForward + eyeUp - eyeLeft;
    vec3 bottomLeft = eyePos + eyeForward - eyeUp + eyeLeft;

    vec3 right = topRight-topLeft;
    vec3 up = topLeft-bottomLeft;

    f32 fResX = (f32)resolutionX;
    f32 fResY = (f32)resolutionY;

	int x;
	#pragma omp parallel for private(x) schedule (guided)
	for(int y=0; y<resolutionY; ++y)
    {
		for(x=0; x<resolutionX; ++x)
        {
		    Ray ray;
		    ray.origin = eyePos;
			vec3 intensity;
			for(int rayX = 0; rayX < raysperpixel; rayX++) {
				for(int rayY = 0; rayY < raysperpixel; rayY++) {
					f32 stepx = (f32)rayX / (f32)raysperpixel;
					f32 stepy = (f32)rayY / (f32)raysperpixel;
		            ray.direction = glm::normalize(bottomLeft + ((f32)x+stepx)/fResX * right + ((f32)y+stepy)/fResY * up - ray.origin);
					intensity += rayTraceBVH(ray);
				}
			}
			intensity = intensity / (f32) (raysperpixel*raysperpixel);


            int pos = (resolutionX*y + x) * 3;
            intensityData[pos + 0] = intensity.x;
            intensityData[pos + 1] = intensity.y;
            intensityData[pos + 2] = intensity.z;


        }

        // render after each line
        //#pragma omp master
        if (omp_get_thread_num() == 0)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawPixels(resolutionX,resolutionY,GL_RGB,GL_FLOAT,intensityData);
            glfwSwapBuffers();
        }
    }

    //std::cout<<"Done!"<<std::endl;
}
