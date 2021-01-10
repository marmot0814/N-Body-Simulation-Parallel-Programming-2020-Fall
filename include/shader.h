#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

class Shader {
public:
    GLuint program;
    Shader(const GLchar*, const GLchar*);
};
