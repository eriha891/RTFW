#pragma once

#ifndef BVHNODE_H
#define BVHNODE_H

#include "Types.h"
#include "AABB.h"

/**

    The Node-structure for the BoundingVolumeHierarchy.

    if the leaf flag in sizeData is 0 then:
    the Node itself is not a leaf and locData is the location of the left node, thus locData+1 is the location of the right node.

    if the leaf flag in sizeData is 1 then:
    the Node is a leaf and locData refers to the startingIndex of some sort, (Typically a index to some triangle)

    sizeData is the number of elements in the node.
*/

class BVHNode
{
private:
	u32 locData;
	u32 sizeData;

	// first bit in sizeData is a leaf flag.
	// second bit in sizeData is a visibility flag.
public:
	AABB aabb;

	BVHNode() : locData(0), sizeData(0) {}

    inline void setLeft(u32 nodeIndex) { locData = nodeIndex; }
	inline u32 getLeft() const { return locData; }
	inline u32 getRight() const { return (locData+1); }

	inline void setSize(u32 size) { sizeData = size + (sizeData&0xc0000000); }
	inline u32 getSize() const { return sizeData&0x3fffffff; }

    inline void setIndex(u32 faceIndex) { locData = faceIndex; }
	inline u32 getIndex() const { return locData; }

	inline void setLeaf(bool leaf) { sizeData = (leaf)?(sizeData|0x80000000):(sizeData&0x7fffffff); }
	inline bool isLeaf() const { return ((sizeData&0x80000000) > 0); }

	inline void setVisible(bool vis) { sizeData = (vis)?(sizeData|0x40000000):(sizeData&0xbfffffff); }
	inline bool isVisible() const { return ((sizeData&0x40000000) > 0); }
};

#endif
