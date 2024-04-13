#include <cmath>
#include <assert.h>
#include "../scene/Ray.h"
#include "Box.h"

bool Box::intersectLocal(const ray& r, isect& i) const {
	// YOUR CODE HERE:
	// Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	BoundingBox box = BoundingBox();
	box.min = vec3f(-0.5, -0.5, -0.5);
	box.max = vec3f(0.5, 0.5, 0.5);

	double tmin, tmax;
	if (!box.intersect(r, tmin, tmax)) {
		return false;
	}
	if (tmax < RAY_EPSILON || abs(tmin - tmax) < RAY_EPSILON) {
		return false;
	}
	i.t = tmin;
	vec3f interPoint = r.at(i.t);
	vec3f surfaceNormal = vec3f(0, 0, 0);
	i.obj = this;
	for (int j = 0; j < 3; j++) {
		if (abs(box.max[j] - interPoint[j]) < RAY_EPSILON) {
			surfaceNormal[j] = 1;
			i.setN(surfaceNormal);
			return true;
		}
		else if (abs(box.min[j] - interPoint[j]) < RAY_EPSILON) {
			surfaceNormal[j] = -1;
			i.setN(surfaceNormal);
			return true;
		}
	}
	return true;
}
