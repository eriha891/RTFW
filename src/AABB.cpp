

#include "AABB.h"

bool AABB::inside(const glm::vec3 &point) const
{
	if( point[0] < _min[0] || point[0] > _max[0] ) return false;
    if( point[1] < _min[1] || point[1] > _max[1] ) return false;
    if( point[2] < _min[2] || point[2] > _max[2] ) return false;

    return true;
}

AABB::AABB(const AABB &box)
{
	_min = box._min;
	_max = box._max;
}


AABB::AABB(const f32* pointData, u32 size)
{
    assert(pointData && size>0);

    vec3 p(pointData[0],pointData[1],pointData[2]);
    _min = _max = p;

    for(u32 i=3; i<size*3; i+=3)
    {
        p = vec3(pointData[i],pointData[i+1],pointData[i+2]);
        _min = glm::min(_min,p);
        _max = glm::max(_max,p);
    }
}

AABB AABB::operator+(const AABB &box)
{
	AABB b;

	b._min = glm::min(_min,box._min);
	b._max = glm::max(_max,box._max);

	return b;
}

AABB AABB::operator+(const glm::vec3 &p)
{
	AABB box(*this);

	box._min = glm::min(box._min,p);
	box._max = glm::max(box._max,p);

	return box;
}

const AABB & AABB::operator=(const AABB &box)
{
	_min = box._min;
	_max = box._max;

	return *this;
}

glm::vec3 AABB::getVertexP(const glm::vec3 &normal) const
{

    glm::vec3 p = _min;
    if (normal.x >= 0.0f)
        p.x = _max.x;
    if (normal.y >= 0.0f)
        p.y = _max.y;
    if (normal.z >= 0.0f)
        p.z = _max.z;

    return p;
}

glm::vec3 AABB::getVertexN(const glm::vec3 &normal) const
{
    glm::vec3 n = _max;
    if (normal.x >= 0.0f)
        n.x = _min.x;
    if (normal.y >= 0.0f)
        n.y = _min.y;
    if (normal.z >= 0.0f)
        n.z = _min.z;

    return n;
}

glm::vec3 AABB::getVertexP(const glm::vec4 &plane) const
{
    glm::vec3 p = _min;
    if (plane.x >= 0.0f)
        p.x = _max.x;
    if (plane.y >= 0.0f)
        p.y = _max.y;
    if (plane.z >= 0.0f)
        p.z = _max.z;

    return p;
}

glm::vec3 AABB::getVertexN(const glm::vec4 &plane) const
{
    glm::vec3 n = _max;
    if (plane.x >= 0.0f)
        n.x = _min.x;
    if (plane.y >= 0.0f)
        n.y = _min.y;
    if (plane.z >= 0.0f)
        n.z = _min.z;

    return n;
}

void AABB::draw() const
{
    GLfloat vertices[] = { 	_min.x,_min.y,_min.z,
                            _min.x,_max.y,_min.z,
                            _max.x,_max.y,_min.z,
                            _max.x,_min.y,_min.z,

                            _max.x,_min.y,_max.z,
                            _min.x,_min.y,_max.z,
                            _min.x,_max.y,_max.z,
                            _max.x,_max.y,_max.z
                             };     // 8 of vertex coords
    GLubyte indices[] = {0,1,2,3,   // 24 of indices
                         0,3,4,5,
                         0,5,6,1,
                         1,6,7,2,
                         7,4,3,2,
                         4,7,6,5};

    // activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    // draw a cube
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
}
