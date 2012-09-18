#ifndef AABB_H
#define AABB_H

#include "Types.h"

class AABB
{
public:
	AABB(){}
	AABB(const AABB &box);
	AABB(const f32* pointData, u32 size);

	const vec3 & min() { return _min; }
	const vec3 & max() { return _max; }

	vec3 getVertexP(const vec3 &normal) const;
	vec3 getVertexN(const vec3 &normal) const;

    vec3 getVertexP(const vec4 &plane) const;
	vec3 getVertexN(const vec4 &plane) const;

	inline void addFace(const vec3 &p0, const vec3 &p1, const vec3 &p2)
	{
        _min = glm::min(_min, glm::min(p0, glm::min(p1,p2) ) );
        _max = glm::max(_max, glm::max(p0, glm::max(p1,p2) ) );
	}

    inline void addPoint(const vec3 &p)
    {
        _min = glm::min(_min,p);
        _max = glm::max(_max,p);
    }

    inline void addAABB(const AABB &aabb)
    {
        _min = glm::min(_min, aabb._min);
        _max = glm::max(_max, aabb._max);
    }

	AABB operator+(const AABB &box);
	AABB operator+(const vec3 &p);
	const AABB & operator=(const AABB &box);

	bool inside(const vec3 &p) const;

	inline vec3 size() const { return _max-_min; }

	inline vec3 center() const { return (_min+_max)/2.0f; }

	void draw() const;

	AABB &invertedMax()
	{
		_min[0] = MAXFLOAT;
		_min[1] = MAXFLOAT;
		_min[2] = MAXFLOAT;
		_max[0] = -MAXFLOAT;
		_max[1] = -MAXFLOAT;
		_max[2] = -MAXFLOAT;

		return *this;
	}

	inline float volume() const { vec3 _size=size(); return _size.x*_size.y*_size.z; }
	inline float surfaceArea() const { vec3 _size=size(); return 2.0f*(_size.x*_size.y + _size.x*_size.z + _size.y*_size.z); }

	vec3 _min, _max;
};

#endif
