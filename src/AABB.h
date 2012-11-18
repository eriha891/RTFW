#ifndef AABB_H
#define AABB_H

#include "Types.h"

#ifndef MAXFLOAT
#define MAXFLOAT 1.0e+37f;
#endif

class AABB
{
public:
	AABB(){}
	AABB(const AABB &box);
	AABB(const f32* pointData, u32 size);

	inline const vec3 & min() { return _min; }
	inline const vec3 & max() { return _max; }

	vec3 getVertexP(const vec3 &normal) const;
	vec3 getVertexN(const vec3 &normal) const;

    vec3 getVertexP(const vec4 &plane) const;
	vec3 getVertexN(const vec4 &plane) const;

    void addPoint(const vec3 &p)
    {
        _min = glm::min(_min,p);
        _max = glm::max(_max,p);
    }

    void addAABB(const AABB &aabb)
    {
        _min = glm::min(_min, aabb._min);
        _max = glm::max(_max, aabb._max);
    }

	AABB operator+(const AABB &box);
	AABB operator+(const vec3 &p);
	const AABB & operator=(const AABB &box);

	bool inside(const vec3 &p) const;

	inline vec3 size() const { return _max-_min; }
	inline vec3 center() const { return (_min+_max)*0.5f; }

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

	void draw() const;

	vec3 _min, _max;
};

#endif
