#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"

class Light: public SceneElement {
	public:
		virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
		virtual double distanceAttenuation( const vec3f& P ) const = 0;
		virtual vec3f getColor( const vec3f& P ) const = 0;
		virtual vec3f getDirection( const vec3f& P ) const = 0;

	protected:
		vec3f color;
		Light( Scene *scene, const vec3f& col )
			: SceneElement( scene ), color( col ) {}
};

class DirectionalLight: public Light {
	public:
		DirectionalLight( Scene *scene, const vec3f& orien, const vec3f& color )
			: Light( scene, color ), orientation( orien ) {}
		virtual vec3f shadowAttenuation(const vec3f& P) const;
		virtual double distanceAttenuation( const vec3f& P ) const;
		virtual vec3f getColor( const vec3f& P ) const;
		virtual vec3f getDirection( const vec3f& P ) const;

	protected:
		vec3f orientation;
};

class PointLight: public Light {
	public:
		double constant_attenuation_coeff;
		double linear_attenuation_coeff;
		double quadratic_attenuation_coeff;

		PointLight( Scene *scene, const vec3f& pos, const vec3f& color )
			: Light( scene, color ), position( pos ) {}
		virtual vec3f shadowAttenuation(const vec3f& P) const;
		virtual double distanceAttenuation( const vec3f& P ) const;
		virtual vec3f getColor( const vec3f& P ) const;
		virtual vec3f getDirection( const vec3f& P ) const;
	protected:
		vec3f position;
};

#endif // __LIGHT_H__
