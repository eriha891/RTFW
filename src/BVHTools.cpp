#include "BVHTools.h"
/**

    newNodePair:
    Allocates a pair of BVHNodes in the vector node
    and returns the first index of the newly allocated nodes.

*/

u32 newNodePair(std::vector<BVHNode> &nodes)
{
    nodes.push_back(BVHNode());
    nodes.push_back(BVHNode());
    nodes[nodes.size()-2].setIndex(nodes.size()-2);
    nodes[nodes.size()-1].setIndex(nodes.size()-1);
    return nodes.size()-2;
}
