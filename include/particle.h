#pragma once
#include "GLM/glm.hpp"
#include "GLM/gtc/random.hpp"

struct Particle {
    float mass;
    glm::vec3 location;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 force;
    Particle(float mass = 0);
};
