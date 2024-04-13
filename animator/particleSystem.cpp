#pragma warning(disable : 4786)

#include "particleSystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>


#include "modelerdraw.h"

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() {
}

/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() {
	// TODO
	particles.clear();
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t) {
	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;
}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t) {
	// These values are used by the UI
	simulate = false;
	dirty = true;
}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t) {    
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t) {
	bake_fps = t - currentT;
	currentT = t;

	map<double, std::vector<Particle>>::iterator it = bakedParticles.find(t);
	if (isSimulate()) {
		if (it == bakedParticles.end()) {
			for (int i = 0; i < particles.size(); i++) {				
				particles[i].velocity = particles[i].velocity + particles[i].force / particles[i].mass * bake_fps;
				if (particles[i].position[1] < 0 && particles[i].velocity[1] < 0) {
					particles[i].velocity[1] = -particles[i].velocity[1];
					 if (particles[i].velocity[1] * particles[i].velocity[1] / (10 * 9.81) < 2) {
					 	particles[i].velocity[1] = 0.5 * particles[i].velocity[1];
					 } else {
					 	particles[i].velocity[1] = 0.8 * particles[i].velocity[1];
					 }
					 if (particles[i].velocity[1] < 1) {
						particles[i].velocity[0] = 0;
						particles[i].velocity[1] = 0;
						particles[i].velocity[2] = 0;
						particles[i].force = Vec3d(0, 0, 0);
					 }
				}
				particles[i].position = particles[i].position + particles[i].velocity * bake_fps;
				bakedParticles.insert(std::pair<double, std::vector<Particle>>(t, particles));
			}
		} else {
			particles = bakedParticles[t];
		}
	}
}


/** Render particles */
void ParticleSystem::drawParticles(float t) {
	if (isSimulate()) {
		for (int i = 0; i < particles.size(); i++) {
			setDiffuseColor(0, 1, 0);
			glPushMatrix();
				glTranslated(
					particles[i].position[0],
					particles[i].position[1],
					particles[i].position[2]
				);
				drawSphere(0.2);
			glPopMatrix();
		}
	}
}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) {
	bakedParticles.insert(std::pair<double, std::vector<Particle>>(t, particles));
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked() {
	bakedParticles.clear();
}





