#pragma once

#include "particle.h"
#include "displayer.h"

#include <vector>
using namespace std;

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

class PhysicalEngine {
    void nextSerial(vector<Particle>&, GLfloat);
    void nextSIMD(vector<Particle>&, GLfloat);
    void nextOpenMP(vector<Particle>&, GLfloat);
    void nextSIMDOpenMP(vector<Particle>&, GLfloat);
    void nextCUDA(vector<Particle>&, GLfloat);
    void nextCUDAFast(vector<Particle>&, GLfloat);
    void calculateOneForce(int, vector<Particle>&, GLfloat);
    void calculateOneForceSIMD(int, vector<Particle>&, GLfloat, float*, float*, float*);
    void updateOnePosition(int, vector<Particle>&, GLfloat);
public:
    void next(vector<Particle>&, COMPUTE_MODE, GLfloat);
};
