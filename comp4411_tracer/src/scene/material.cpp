#include "ray.h"
#include "material.h"
#include "light.h"

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.

vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const {
	Material m = i.getMaterial();
	vec3f Q = r.at(i.t);
	vec3f V = r.getDirection();
	vec3f I = m.ke + prod(m.ka, scene->ambientLight);
	for (auto l = scene->beginLights(); l != scene->endLights(); l++) {
		Light* light = *l;
		vec3f L = light->getDirection(Q);
		vec3f atten = light->distanceAttenuation(Q) * light->shadowAttenuation(Q);
		vec3f diffuse = prod(kd * std::max(0.0, i.N.dot(L)), vec3f(1.0, 1.0, 1.0) - kt);
		vec3f R = ((2 * i.N * i.N.dot(L)) - L).normalize();
		vec3f specular = pow(std::max(R.dot(V), 0.0), m.shininess * 128) * ks;
		I += prod(prod(atten, (diffuse + specular)), light->getColor(Q));
	}
	I.clamp();
	return I;
}
