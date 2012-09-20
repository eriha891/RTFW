
#include "SimpleRenderer.h"


f32 SimpleRenderer::rayTraceNode(const Ray &ray, u32 nodeIndex, vec3 &intensity)
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
            intensity = glm::dot(-ray.direction,faces[triangleIndex].normal)*matLib[materials[triangleIndex]].getDiffuseColor();
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