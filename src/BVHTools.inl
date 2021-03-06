/**
 * newNodePair:
 * Allocates a pair of BVHNodes in the vector node
 * and returns the first index of the newly allocated nodes.
*/

inline unsigned int BVH::newNodePair(std::vector<BVHNode> &nodes)
{
    nodes.push_back(BVHNode());
    nodes.push_back(BVHNode());
    nodes[nodes.size()-2].setIndex(nodes.size()-2);
    nodes[nodes.size()-1].setIndex(nodes.size()-1);
    return nodes.size()-2;	
}

/**
 * BVH::reorderVector
 * Reorders a vector 'object' with the order specified by the vector 'order'
*/

template <class T>
inline void BVH::reorderVector(	std::vector<T> &object,
								const std::vector<unsigned int> order)
{
    assert(object.size() == order.size());
    std::vector<T> copy(object.begin(), object.end());

    for(unsigned int i=0; i<object.size(); ++i)
    {
        object[i] = copy[order[i]];
    }
}

template <class T>
inline void BVH::createBVH(	std::vector<BVHNode> &node,
							std::vector<T> &object,
							std::vector<AABB> &aabb,
							unsigned int maxObjsPerNode,
							unsigned int maxDepth)
{
    assert(object.size() > 0 && aabb.size() > 0);
    assert(object.size() == aabb.size());

    node.clear();
    node.push_back(BVHNode());

    node[0].setIndex(0);
    node[0].setSize(object.size());
    node[0].aabb.invertedMax();

    for(unsigned int i=0; i<aabb.size(); ++i)
    {
        node[0].aabb.addAABB(aabb[i]);
    }

    //printf("aabb total size is %f %f %f \n", node[0].aabb.size().x, node[0].aabb.size().y, node[0].aabb.size().z);

    subDivide(node, object, aabb, 0, 0, maxDepth, maxObjsPerNode);

    //printf("BVH built, nodes: %d \n",(int)node.size());
}

/**
 * subDivide:
 * Recursive function that subdivides a node into two subnodes.
 * The recursion is halted when MAXDEPTH is reached or the number of objects per node is less than MAXOBJS.
 * Basics of the algorithm:
 * places the AABBs of the objects into bins that are spaced evenly across an axis of the node[_nodeIndex]'s AABB.
 * then it finds the "cheapest" split using Surface Area Heuristics along the axis.
 * when the cheapest split along the cheapest axis is found, it subdivides the node into two new AABBs containing
 * the objects.
*/
template <class T>
inline void BVH::subDivide(	std::vector<BVHNode> &node,
							std::vector<T> &object,
							std::vector<AABB> &aabb,
							unsigned int _nodeIndex,
							unsigned int _depth,
							const unsigned int maxDepth,
							const unsigned int maxObjsPerNode)
{
    // Are we too deep? terminate the node!
    if(_depth >= maxDepth)
    {
        node[_nodeIndex].setLeaf(true);
        return;
    }

    vec3 aabbSize = node[_nodeIndex].aabb.size();

    AABB aabbBin[NUMBINS], accLeft[NUMBINS], accRight[NUMBINS];
    unsigned int numBin[NUMBINS], numLeft[NUMBINS], numRight[NUMBINS];

    AABB aabbLeft, aabbRight;
    unsigned int nL=0, nR=0;

    const float K = 8.0f;
    const float SAV = 0.5f / (node[_nodeIndex].aabb.surfaceArea());
    
    float lowCost = MAXFLOAT;
    float SA1, SA2, cost;
    float center;

    unsigned int bestBin = 4;
    unsigned int bestAxis = 0;

    unsigned int begin = node[_nodeIndex].getIndex();
    unsigned int end = begin+node[_nodeIndex].getSize();

    unsigned int u, index;
    int i;

    for(unsigned int axis=0; axis<3; ++axis)
    {
        for(i=0; i<NUMBINS; ++i)
        {
            numBin[i] = 0;
            aabbBin[i].invertedMax();
            accLeft[i].invertedMax();
            accRight[i].invertedMax();
            numLeft[i] = numRight[i] = 0;
        }

        // Biggest time-slice goes into this for-loop
        for(i=(int)begin; i<(int)end; ++i)
        {
            center = aabb[i].center()[axis];
            index = (unsigned int)(K*(1.0f-EPSILON)*(center - node[_nodeIndex].aabb.min()[axis]) / aabbSize[axis]);

            assert(index < 8);

            ++numBin[index];
            aabbBin[index].addAABB(aabb[i]);
        }

        accLeft[1] = aabbBin[0];
        numLeft[1] = numBin[0];

        for(i=2; i<NUMBINS; ++i)
        {
            accLeft[i] = accLeft[i-1];
            accLeft[i].addAABB(aabbBin[i-1]);
            numLeft[i] = numLeft[i-1] + numBin[i-1];
        }

        accRight[7] = aabbBin[7];
        numRight[7] = numBin[7];

        for(i=NUMBINS-2; i>0; --i)
        {
            accRight[i] = accRight[i+1];
            accRight[i].addAABB(aabbBin[i]);
            numRight[i] = numRight[i+1] + numBin[i];
        }

        // Calculate the costs of the accumulated volumes
        for(i=1; i<NUMBINS; ++i)
        {
            SA1 = accLeft[i].surfaceArea();
            SA2 = accRight[i].surfaceArea();
            cost = 0.3f + 1.0f * SAV * (SA1 * numLeft[i] + SA2 * numRight[i]);

            if(cost < lowCost)
            {
                lowCost = cost;
                bestBin = i;
                bestAxis = axis;
                aabbLeft = accLeft[i];
                aabbRight = accRight[i];
                nL = numLeft[i];
                nR = numRight[i];
            }
        }
    }

    if((float)node[_nodeIndex].getSize() < lowCost)
    {
        node[_nodeIndex].setLeaf(true);
        return;
    }

    // Array of bool to check if order[i] is left of the splitBox
    bool *leftSide = new bool[node[_nodeIndex].getSize()];

    for(i=0, u=begin; u<end; ++u, ++i)
    {
        center = aabb[u].center()[bestAxis];
        index = (unsigned int)(K*(1.0f-EPSILON)*(center - node[_nodeIndex].aabb.min()[bestAxis]) / aabbSize[bestAxis]);

        assert(index < 8);

        leftSide[i] = (index < bestBin) ? true : false;
    }

    int max = node[_nodeIndex].getSize()-1;
    for(int i=-1, j=node[_nodeIndex].getSize(); ; )
    {
        while((i < max) && leftSide[++i]);
        while((j > 0) && !leftSide[--j]);
        if(i<j)//swap
        {
            T ti = object[node[_nodeIndex].getIndex()+i];
            AABB taabb = aabb[node[_nodeIndex].getIndex()+i];

            object[node[_nodeIndex].getIndex()+i] = object[node[_nodeIndex].getIndex()+j];
            aabb[node[_nodeIndex].getIndex()+i] = aabb[node[_nodeIndex].getIndex()+j];

            object[node[_nodeIndex].getIndex()+j] = ti;
            aabb[node[_nodeIndex].getIndex()+j] = taabb;
        }
        else
            break;
    }

    if(leftSide)
        delete leftSide;

    // All is set, subdivide this bastard
    unsigned int leftIndex = newNodePair(node);

    // Left
    node[leftIndex].setIndex(node[_nodeIndex].getIndex());
    node[leftIndex].setSize(nL);
    node[leftIndex].aabb = aabbLeft;

    // Right
    node[leftIndex+1].setIndex(node[_nodeIndex].getIndex() + nL);
    node[leftIndex+1].setSize(nR);
    node[leftIndex+1].aabb = aabbRight;

    if(nL > maxObjsPerNode)
        subDivide(node, object, aabb, leftIndex, _depth+1, maxDepth, maxObjsPerNode);
    else
        node[leftIndex].setLeaf(true);

    if(nR > maxObjsPerNode)
        subDivide(node, object, aabb, leftIndex+1, _depth+1, maxDepth, maxObjsPerNode);
    else
        node[leftIndex+1].setLeaf(true);

    node[_nodeIndex].setLeaf(false);
    node[_nodeIndex].setLeft(leftIndex);
}
