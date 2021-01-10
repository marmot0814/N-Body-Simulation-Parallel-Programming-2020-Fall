#include "particle.h"

Particle::Particle(float mass)
    : mass(mass), location(glm::vec3(glm::sphericalRand(1.5f))) {}
