#include <iostream>
#include <cstdio>
#include <cassert>
#include <fstream>

#include "ObjLoader.h"

#define toFloat(x) atof(x.c_str())
#define toInt(x) atoi(x.c_str())

// Helper function to retrive the indices in a face chunk
void getIndices(const std::string &str, int &v, int &t, int &n)
{
    v = t = n = -1;

    int slash[2];
    int counter=0;

    slash[0] = (int)str.length();

    for(int i=0; i<(int)str.length(); ++i)
    {
        if(str[i]=='/')
        {
            slash[counter]=i;
            ++counter;
        }
    }

    v = atoi(str.substr(0,slash[0]).c_str())-1;

    if(counter==0)
        return;

    if(slash[0]+1 != slash[1])
        t = atoi(str.substr(slash[0]+1,slash[0]+1+slash[1]).c_str())-1;

    n = atoi(str.substr(slash[1]+1,str.length()).c_str())-1;
}

int loadObj(Geometry &geom, const std::string &filename, float scale)
{
    std::vector<Geometry> geomList;
    loadObj(geomList,filename,scale);

    geom.clear();

    // Pack all Geometry into one.
    for(u32 i=0; i<geomList.size(); ++i)
    {
        geom.addGeometry(geomList[i]);
    }

    return 0;
}

int loadObj( std::vector<Geometry> &geomList, const std::string &filename, float scale )
{
    std::ifstream file;
    file.open(filename.c_str(), std::ios::in);

    std::cout<<"loading "<<filename<<std::endl;

    if(file.fail())
    {
        std::cout<<"loadObj failed, could not read "<<std::endl;
        return 1;
    }

    Geometry g;
    std::string line,param;

    std::vector<vec3> tempVertex;
    std::vector<vec3> tempNormal;
    std::vector<vec2> tempTexCoord;

    tempVertex.reserve(100000);
    tempNormal.reserve(100000);
    tempTexCoord.reserve(10000);

    std::vector<std::vector<int> > vertexUsed;
    std::vector<int> texCoordUsed;
    int tempSG = 0;

    std::vector<int> resetVector;
    resetVector.resize(1,-1);

    bool first=true;
    std::string tempName;

    while( !file.eof() && file.good() )
    {
        std::getline(file,line);

        Tokenizer token(line);

        param = token.getNext();
        if(param == "v")
        {
            vec3 vertex;

            vertex.x = scale*toFloat(token.getNext());
            vertex.y = scale*toFloat(token.getNext());
            vertex.z = scale*toFloat(token.getNext());

            tempVertex.push_back(vertex);
            vertexUsed.push_back(resetVector);
        }
        else if(param == "f")
        {
            ivec4 vdata(-1), vtdata(-1), ndata(-1), fdata(-1);

            //printf("tokensize! %i \n",token.size());

            for(u8 i=0; i<token.size()-1; ++i)
            {
                param = token.getNext();
                getIndices(param,vdata[i],vtdata[i],ndata[i]);

                if(tempSG > (int)vertexUsed[vdata[i]].size()-1)
                    vertexUsed[vdata[i]].resize(tempSG+1,-1);

                if(vertexUsed[vdata[i]][tempSG] > -1)
                    fdata[i] = vertexUsed[vdata[i]][tempSG];
                else
                {
                    vertexUsed[vdata[i]][tempSG] = (int)g.vertex.size();

                    fdata[i] = g.vertex.size();

                    tVertex tv;
                    tv.x = tempVertex[vdata[i]].x;
                    tv.y = tempVertex[vdata[i]].y;
                    tv.z = tempVertex[vdata[i]].z;
                    tv.nx = tv.ny = tv.nz = tv.s = tv.t = 0.0f;

                    if(vtdata[i]>-1)
                    {
                        tv.s = tempTexCoord[vtdata[i]].x;
                        tv.t = tempTexCoord[vtdata[i]].y;
                    }
                    if(ndata[i]>-1)
                    {
                        tv.nx = tempNormal[ndata[i]].x;
                        tv.ny = tempNormal[ndata[i]].y;
                        tv.nz = tempNormal[ndata[i]].z;
                    }

                    g.vertex.push_back(tv);
                }
            }
            // if its a triangle, just insert.
            // However if its a quad, then insert the two triangles forming the quad.
            tFace f;
            f.point[0] = fdata[0];
            f.point[1] = fdata[1];
            f.point[2] = fdata[2];

            g.face.push_back(f);

            if(fdata[3] != -1)
            {
                f.point[0] = fdata[3];
                f.point[1] = fdata[0];
                f.point[2] = fdata[2];

                g.face.push_back(f);
            }
        }
        else if(param == "vt")
        {
            vec2 tc;

            tc.x = toFloat(token.getNext());
            tc.y = toFloat(token.getNext());

            tempTexCoord.push_back(tc);
        }
        else if(param == "vn")
        {
            vec3 normal;

            normal.x = toFloat(token.getNext());
            normal.y = toFloat(token.getNext());
            normal.z = toFloat(token.getNext());

            tempNormal.push_back(normal);
        }
        else if(param == "s")
            tempSG = toInt(token.getNext());
        else if(param == "g")
        {
            if(first)
                first=false;
            else
            {
                g.process();
                geomList.push_back(g);
            }

            for(u32 i=0; i<vertexUsed.size(); ++i)
                vertexUsed[i].clear();

            g.clear();
        }

        if(file.eof())
            break;
    }
    file.close();

    g.process();
    geomList.push_back(g);

    std::cout<<"done reading "<<filename<<std::endl;

    return 0;
}
