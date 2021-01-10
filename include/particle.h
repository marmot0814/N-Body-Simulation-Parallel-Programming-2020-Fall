#pragma once
#include "GLM/glm.hpp"
#include "GLM/gtc/random.hpp"

class Particle {
    float mass;
    glm::vec3 location;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 force;

public:
    Particle(float mass);
};
