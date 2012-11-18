#pragma once

#ifndef BVHTOOLS_H
#define BVHTOOLS_H

#include <vector>
#include "BVHNode.h"
#include "AABB.h"

#define NUMBINS 8
#define EPSILON 1.0e-5f

class BVH
{
public:
    template <class T>
    static void createBVH(  std::vector<BVHNode> &node,
                            std::vector<T> &object,
                            std::vector<AABB> &aabb,
                            unsigned int maxObjsPerNode,
                            unsigned int maxDepth);

    template <class T>
    static void reorderVector(  std::vector<T> &object,
                                const std::vector<unsigned int> order);

private:
    template <class T>
    static void subDivide(  std::vector<BVHNode> &node,
                            std::vector<T> &object,
                            std::vector<AABB> &aabb,
                            unsigned int _nodeIndex,
                            unsigned int _depth,
                            const unsigned int maxDepth,
                            const unsigned int maxObjsPerNode);

    static unsigned int newNodePair(std::vector<BVHNode> &nodes);
};

#include "BVHTools.inl"

#endif