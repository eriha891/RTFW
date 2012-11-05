#include "MonteCarloRenderer.h"


inline vec3 reinhardTonemap(const vec3 &x)
{
    return x/(x + 1.0f);
}

vec3 uncharted2Tonemap(const vec3 &x)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;

    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 MonteCarloRenderer::radiance(const Ray &ray)
{
    vec3 rad;

    Hit hit = rayTraceNode(ray,0);
    if(hit < MAXFLOAT)
    {
		f32 limit = 0.9;
		f32 r = glm::compRand1(0.0f,1.0f);
        rad = matLib[materials[hit.index]].getEmission() + matLib[materials[hit.index]].getDiffuseColor()*0.05f;
        if(r < limit)
        {
			r = r / limit;
        	// calculate plane vectors
        	vec3 n = faces[hit.index].normal;
        	vec3 origin = ray.origin + ray.direction*hit.distance + n*.01f;
        	vec3 a = faces[hit.index].nOrt;
        	vec3 b = glm::cross(n,a);

			f32 phi = glm::compRand1(0.0f,2.0f*PI);

            /*vec3 dir = glm::vecRand3(1.0f,1.0f);
            while(glm::dot(dir,n)<0.0f)
                dir = glm::vecRand3(1.0f,1.0f);*/

            //Ray newRay(origin, glm::normalize(n + a*glm::compRand1(-1.0f,1.0f) + b*glm::compRand1(-1.0f,1.0f)));

			Ray newRay(origin, a*r*glm::cos(phi) + b*r*glm::sin(phi) + n*glm::sqrt(1.0f-r*r));
            rad = matLib[materials[hit.index]].getEmission() + matLib[materials[hit.index]].getDiffuseColor()*radiance(newRay);
        }
    }

    return rad;
}

inline vec3 MonteCarloRenderer::rayTraceBVH(const Ray &ray)
{
    return glm::sqrt(radiance(ray));
}
