#pragma once

#ifndef HIT_H
#define HIT_H

class Hit
{
public:
    f32 distance;
    u32 index;
	vec3 normal;

    Hit() {}

    Hit(f32 d, u32 i)
    {
        distance = d;
        index = i;
    }
};

inline bool operator<(const Hit &lhs, const Hit &rhs)
{
    return lhs.distance < rhs.distance;
}

inline bool operator<(const Hit &lhs, const float &rhs)
{
    return lhs.distance < rhs;
}

inline bool operator<(const float &lhs, const Hit &rhs)
{
    return lhs < rhs.distance;
}

inline bool operator>(const Hit &lhs, const Hit &rhs)
{
    return lhs.distance > rhs.distance;
}

inline bool operator>(const Hit &lhs, const float &rhs)
{
    return lhs.distance > rhs;
}

inline bool operator>(const float &lhs, const Hit &rhs)
{
    return lhs > rhs.distance;
}

#endif
