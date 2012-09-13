#pragma once

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "Types.h"
#include "AABB.h"

typedef struct
{
    f32 x,y,z;
    f32 nx,ny,nz;
    f32 s,t;
}tVertex;

typedef struct
{
    f32 x,y,z;
}tNormal;

typedef struct
{
    f32 s,t;
}tTexCoord;

typedef struct
{
    u32 point[3];
}tFace;

class Geometry
{
public:
    std::vector<tVertex> vertex;
    std::vector<tFace> face;
    AABB aabb;

    void clear() { vertex.clear(); face.clear(); }

    void process()
    {
        i32 sharedFaces[vertex.size()];
        u32 i;
        f32 numFaces;

        glm::vec3 n,a,b;

        std::vector<vec3> tempNormal;
        tempNormal.resize(vertex.size(),vec3(0));

        assert(vertex.size() > 0);

        aabb.invertedMax();

        for (i=0; i<(u32)vertex.size(); ++i)
        {
            sharedFaces[i] = 0;
            aabb.addPoint( vec3(vertex[i].x, vertex[i].y, vertex[i].z) );
        }

        for (i=0; i<(u32)face.size(); ++i)
        {
            a.x = vertex[face[i].point[2]].x - vertex[face[i].point[0]].x;
            a.y = vertex[face[i].point[2]].y - vertex[face[i].point[0]].y;
            a.z = vertex[face[i].point[2]].z - vertex[face[i].point[0]].z;

            b.x = vertex[face[i].point[1]].x - vertex[face[i].point[0]].x;
            b.y = vertex[face[i].point[1]].y - vertex[face[i].point[0]].y;
            b.z = vertex[face[i].point[1]].z - vertex[face[i].point[0]].z;

            n = glm::cross(b,a);//b.cross(a);
            n = glm::normalize(n);

            for(u32 u=0; u<3; ++u)
            {
                tempNormal[face[i].point[u]] += n;
                sharedFaces[face[i].point[u]]++;
            }
        }
        for (i=0; i<(u32)vertex.size(); ++i)
        {
            if(sharedFaces[i]>0)
            {
                numFaces = (f32)sharedFaces[i];
                tempNormal[i] /= numFaces;

                tempNormal[i] = glm::normalize(tempNormal[i]);
            }
            if(vertex[i].nx == 0.0f && vertex[i].ny == 0.0f && vertex[i].nz == 0.0f)
            {
                vertex[i].nx = tempNormal[i].x;
                vertex[i].ny = tempNormal[i].y;
                vertex[i].nz = tempNormal[i].z;
            }
        }
    }

    Geometry& addGeometry(Geometry &g)
    {
        u32 vertexOffset = vertex.size();
        tFace f;
        for(u32 i=0; i<g.vertex.size(); ++i)
        {
            vertex.push_back(g.vertex[i]);
        }
        for(u32 i=0; i<g.face.size(); ++i)
        {
            f = g.face[i];
            f.point[0] += vertexOffset;
            f.point[1] += vertexOffset;
            f.point[2] += vertexOffset;

            face.push_back(f);
        }
        return *this;
    }
};

#endif
