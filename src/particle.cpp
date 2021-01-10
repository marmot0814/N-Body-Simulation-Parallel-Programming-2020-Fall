#include "particle.h"

Particle::Particle(float mass = 0)
    : mass(mass), location(glm::vec3(glm::sphericalRand(1.5f))) {}
