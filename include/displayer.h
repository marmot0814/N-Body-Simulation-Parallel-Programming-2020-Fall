#pragma once

#include <boost/program_options.hpp>
namespace BPO = boost::program_options;

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "SOIL/SOIL.h"
#include <vector>
#include <iostream>
using namespace std;

#include "particle.h"
#include "camera.h"
#include "shader.h"

enum class COMPUTE_MODE {
    SERIAL,
    SIMD,
    OPENMP,
    SIMD_OPENMP,
    CUDA,
    CUDA_FAST
};

class Displayer {
    void InitOpenGL(const BPO::variables_map&);
    void InitGLEW();
    void InitGLFW(const BPO::variables_map&);
    void InitShader(const BPO::variables_map&);
    void InitBuffer(const BPO::variables_map&);
    void InitTexture();
    void moveCamera();
    void SendParticlesToBuffer(GLuint, const vector<Particle> &);
    void showFPS();

    GLFWwindow* window;
    Shader* shader;
    GLuint vbo, vao;
    GLfloat delta_time;
    int frame_count = 0;
    double previous_time = 0.0f;
    double average_fps_sum = 0.0f;
    double average_fps_count = 0;

    static double last_x, last_y;
    static bool first_mouse;
    static Camera camera;
    static bool restart;

    static bool keys[1024];
    static COMPUTE_MODE compute_mode;
    static void KeyHandler(GLFWwindow*, int, int, int, int);
    static void MouseHandler(GLFWwindow*, double, double);
    static void ScrollHandler(GLFWwindow*, double, double);
public:
    Displayer(const BPO::variables_map &args);
    ~Displayer();
    bool stop();
    void update(const vector<Particle> &);
    COMPUTE_MODE getComputeMode();
    GLfloat getDeltaTime();
    bool getRestart();
    void setRestart(bool);
};
