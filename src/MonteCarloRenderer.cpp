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
        // set the kill-ray limit
		f32 limit = 0.90;
		f32 r = glm::compRand1(0.0f,1.0f);
		
        Material *material = &matLib[materials[hit.index]];
        rad = material->getEmission();

        // calculate the surface normal
        vec3 n = interpolateNormal(faces[hit.index], hit.baryCoords);
		float normalDisplacement = 0.00001;

        // check if ray is inside
        bool inside = (glm::dot(ray.direction,n) > 0.0) ? true : false;

        // check if the object is semi-transparent (guessing that no real object are perfect transparent)
        float opacity = material->getOpacity();
        
        if(r < limit)
        {
            if(inside) {
                float snell = material->getRefractiveIndex() / AIR;
                float neg = -1.0;
                n = n*neg;
                vec3 origin;
                vec3 newDirection;

                float angle = acos(glm::dot(n, ray.direction * (float) -1.0));
                float angle_lim = acos(ray.refractiveIndex/material->getRefractiveIndex());
                if(angle > angle_lim) {
					origin = ray.origin + ray.direction*hit.distance + n*normalDisplacement;
                    newDirection = glm::normalize(ray.direction - 2.0f * glm::dot(ray.direction,n) * n);
                } else {
					origin = ray.origin + ray.direction*hit.distance - n*normalDisplacement;
                    float thedot = glm::dot(n,ray.direction);
                    float factor = (snell * thedot - sqrt(1.0 - snell * snell *(1.0 - thedot*thedot)));
                    newDirection = glm::normalize(ray.direction*-snell + n * factor);
                }
                Ray extRay( origin, newDirection);
				/*
				if(angle > angle_lim) {
					rad += vec3(0,0,1);
				} else {
					rad += vec3(1,0,0);
				}
				*/
				//rad += radiance(extRay);
                rad += material->getDiffuseColor()*radiance(extRay);  
            } else {
                r = r / limit;

                // calculate plane vectors
                vec3 origin = ray.origin + ray.direction*hit.distance + n*normalDisplacement;
                vec3 a = faces[hit.index].nOrt;
                vec3 b = glm::cross(n,a);

                // calculate perfect reflection
                vec3 reflection = ray.direction - 2.0f * glm::dot(ray.direction,n) * n;

                // calculate a perfect diffuse ray with random direction
                f32 phi = glm::compRand1(0.0f,2.0f*PI);
                vec3 diffuse = a*r*glm::cos(phi) + b*r*glm::sin(phi) + n*glm::sqrt(1.0f-r*r);

                // interpolationfaction between perfect reflectio nand perfect diffuse rays
                f32 t = material->getSpecularFactor();

                // Do a linear interpolation between a perfect diffuse and a perfect reflection according to specularFactor
                vec3 direction = glm::normalize(diffuse*(1.0f-t) + reflection*t);

                // the new ray that bounces of the surface
                Ray extRay( origin, direction);

                if(opacity < 1.0) {
                    
                    // the new ray that bounces into the object
                    float snell = ray.refractiveIndex / material->getRefractiveIndex();

                    float thedot = glm::dot(n,ray.direction);
                    float factor = (snell * thedot - sqrt(1.0 - snell * snell *(1.0 - thedot*thedot)));
                    vec3 intDirection = glm::normalize(ray.direction*-snell + n * factor);
                    //printf("hej hej");
                    Ray intRay(ray.origin + ray.direction*hit.distance - n*normalDisplacement, intDirection);

                    // calculate the totance emittance of this surface spot depending on tha opacity factor.
                    float invopacity = (1.0 - opacity);
                    if(opacity > 0.0)
                        rad += material->getDiffuseColor()*radiance(extRay)*opacity;
                    rad += material->getDiffuseColor()*radiance(intRay)*invopacity;
					//rad = vec3(0,0,1);
                    
                } else {
                    rad += material->getDiffuseColor()*radiance(extRay);   
                }
            }
        } else {
            rad += localLighting(hit, ray);
        }

        /*
        if(opacity < 1.0)
            limit = 0.45;


        // why, robsk? why? 
        //f32 limit = matLib[materials[hit.index]].getSpecularFactor();
        if(r < limit)
        {
			r = r / limit;

        	// calculate plane vectors
        	vec3 origin = ray.origin + ray.direction*hit.distance + n*.001f;
        	vec3 a = faces[hit.index].nOrt;
        	vec3 b = glm::cross(n,a);

            // calculate perfect reflection
            vec3 reflection = ray.direction - 2.0f * glm::dot(ray.direction,n) * n;

            // calculate a perfect diffuse ray with random direction
            f32 phi = glm::compRand1(0.0f,2.0f*PI);
            vec3 diffuse = a*r*glm::cos(phi) + b*r*glm::sin(phi) + n*glm::sqrt(1.0f-r*r);

            // interpolationfaction between perfect reflectio nand perfect diffuse rays
            f32 t = matLib[materials[hit.index]].getSpecularFactor();

            // Do a linear interpolation between a perfect diffuse and a perfect reflection according to specularFactor
            vec3 direction = glm::normalize(diffuse*(1.0f-t) + reflection*t);

            // the new ray that bounces of the surface
			Ray extRay( origin, direction);

            if(opacity < 1.0) {
                
                // the new ray that bounces into the object
                float snell = AIR / matLib[materials[hit.index]].getRefractiveIndex();
                if(inside) snell = 1.0/snell;

                float thedot = glm::dot(n,ray.direction);
                float factor = (snell * thedot - sqrt(1.0 - snell * snell *(1.0 - thedot*thedot)));
                vec3 intDirection = ray.direction*-snell + n * factor;
                //printf("hej hej");
                Ray intRay( ray.origin + ray.direction*hit.distance - n*.001f, intDirection);

                // calculate the totance emittance of this surface spot depending on tha opacity factor.
                float invopacity = (1.0 - opacity);
                if(inside) {
                    invopacity = 1.0;
                } else {
                    rad += matLib[materials[hit.index]].getDiffuseColor()*radiance(extRay)*opacity;
                }
                rad += matLib[materials[hit.index]].getDiffuseColor()*radiance(intRay)*invopacity;
                
            } else {
                rad += matLib[materials[hit.index]].getDiffuseColor()*radiance(extRay);   
            }
            
        }
        else
            rad += localLighting(hit, ray);

            */
    }

    return rad;
}

vec3 MonteCarloRenderer::localLighting(const Hit &hit, const Ray &ray)
{
    vec3 rad;
    vec3 normal = interpolateNormal(faces[hit.index], hit.baryCoords);
    vec3 position = ray.origin + ray.direction*hit.distance + normal*.001f;

    for(int i=0; i<lights.size(); i++)
    {
        vec3 pixelToLight = lights[i].position - position;
        Ray shadowRay(position, glm::normalize(pixelToLight));
        f32 distSquared = glm::dot(pixelToLight,pixelToLight);
        Hit shadowHit = rayTraceNode(shadowRay, 0);

		if(shadowHit.distance * shadowHit.distance < distSquared) {
			rad += matLib[materials[hit.index]].getEmission();
		} else {
            rad += matLib[materials[hit.index]].getDiffuseColor() * lights[i].color * lights[i].intensity / distSquared	;
		}
    }
    
    return rad;
}

inline vec3 MonteCarloRenderer::rayTraceBVH(const Ray &ray)
{
    return glm::sqrt(radiance(ray));
}
