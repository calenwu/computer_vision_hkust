#include <cmath>

#include "light.h"

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation(const vec3f& P) const {
	isect i;
	Scene* pScene = getScene();
	if (!pScene->intersect(ray(P, getDirection(P)), i)) {
		return color;
	}
	return prod(color, i.getMaterial().kt);
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	double d = vec3f(P - position).length();
	double ret = 1.0 / (
		this->constant_attenuation_coeff +
		this->linear_attenuation_coeff * d +
		this->quadratic_attenuation_coeff * d * d
	);
	return 1.0 < ret ? 1.0 : ret;
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	isect i;
	Scene* pScene = getScene();
	if (pScene->intersect(ray(P, getDirection(P)), i)) {
		return prod(color, i.getMaterial().kt);
	}
	return color;
}