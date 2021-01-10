#include "displayer.h"

double Displayer::last_x = 0;
double Displayer::last_y = 0;
bool Displayer::first_mouse = true;
Camera Displayer::camera;

bool Displayer::keys[1024];
COMPUTE_MODE Displayer::compute_mode = COMPUTE_MODE::SERIAL;

Displayer::Displayer(const BPO::variables_map &args) : delta_time(0.03) {
    InitGLFW(args);
    InitGLEW();
    InitOpenGL(args);
    InitShader(args);
    InitBuffer(args);
    InitTexture();
}

Displayer::~Displayer() {
    glfwTerminate();
    delete shader;
}

void Displayer::InitGLFW(const BPO::variables_map &args) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    if (args["fullscreen"].as<bool>()) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        window = glfwCreateWindow(
            mode->width, mode->height, "OpenGL",
            glfwGetPrimaryMonitor(), nullptr
        );
    } else {
        vector<int> wsize = args["window"].as<vector<int>>();
        window = glfwCreateWindow(
            wsize[0], wsize[1], "OpenGL", nullptr, nullptr
        );
    }

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, Displayer::KeyHandler);
    glfwSetCursorPosCallback(window, Displayer::MouseHandler);
    glfwSetScrollCallback(window, Displayer::ScrollHandler);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Displayer::InitGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        cout << "Failed to initialize GLEW" << endl;
}

void Displayer::InitOpenGL(const BPO::variables_map &args) {
    vector<int> wsize = args["window"].as<vector<int>>();
    glViewport(0, 0, wsize[0], wsize[1]);
    glClearColor(0, 0, 0, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

bool Displayer::stop() {
    return glfwWindowShouldClose(window);
}

void Displayer::InitShader(const BPO::variables_map &args) {
    shader = new Shader("data/vertex.glsl", "data/fragment.glsl");
    glUseProgram(shader->program);

    vector<double> color_vec = args["color"].as<vector<double>>();
    glm::vec3 color(color_vec[0], color_vec[1], color_vec[2]);
    glUniform3fv(glGetUniformLocation(shader->program, "color"), 1, glm::value_ptr(color));
}

void Displayer::InitBuffer(const BPO::variables_map &args) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
}

void Displayer::InitTexture() {
    int width, height;
    unsigned char* image = SOIL_load_image("data/particle.png", &width, &height, 0, SOIL_LOAD_AUTO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
}

void Displayer::moveCamera() {
    if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(DIRECTION::FORWARD, delta_time);
    if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(DIRECTION::BACKWARD, delta_time);
    if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(DIRECTION::LEFT, delta_time);
    if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(DIRECTION::RIGHT, delta_time);
}

void Displayer::SendParticlesToBuffer(GLuint vbo, const vector<Particle> &p) {
    int N = p.size();
    GLfloat* all_floats = new GLfloat[N * 3];

    for (int i = 0, j = 0; i < N; i++) {
        all_floats[j] = p[i].location.x;
        j++;
        all_floats[j] = p[i].location.y;
        j++;
        all_floats[j] = p[i].location.z;
        j++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), all_floats,
                 GL_STATIC_DRAW);
    delete []all_floats;
}

void Displayer::update(const vector<Particle> &particles) {
    glfwPollEvents();  // glfwWaitEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    moveCamera();
    showFPS();
    
    SendParticlesToBuffer(vbo, particles);
    glm::mat4 model;
    glm::mat4 view = camera.GetViewMatrix();

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.fov), (float)width / (float)height,
        0.01f, 1000.f);

    // compute mvp and send it
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "mvp"), 1,
                       GL_FALSE, glm::value_ptr(mvp));

    // draw
    glDrawArrays(GL_POINTS, 0, particles.size());
    glfwSwapBuffers(window);
}

void Displayer::KeyHandler(GLFWwindow* window, int key, int code, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_F1:
                compute_mode = COMPUTE_MODE::SERIAL;
                break;
            case GLFW_KEY_F2:
                compute_mode = COMPUTE_MODE::SIMD;
                break;
            case GLFW_KEY_F3:
                compute_mode = COMPUTE_MODE::OPENMP;
                break;
            case GLFW_KEY_F4:
                compute_mode = COMPUTE_MODE::SIMD_OPENMP;
                break;
            case GLFW_KEY_F5:
                compute_mode = COMPUTE_MODE::CUDA;
                break;
            case GLFW_KEY_F6:
                compute_mode = COMPUTE_MODE::CUDA_FAST;
                break;
            case GLFW_KEY_SPACE:
                break;
            default:
                break;
        }
    }

    // ALL keys
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void Displayer::MouseHandler(GLFWwindow* window, double x_pos, double y_pos) {
    if (first_mouse) {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    GLfloat xoffset = x_pos - last_x;
    GLfloat yoffset = last_y - y_pos;  

    last_x = x_pos;
    last_y = y_pos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void Displayer::ScrollHandler(GLFWwindow* window, double x_offset, double y_offset) {
    camera.ProcessMouseScroll(y_offset);
}

COMPUTE_MODE Displayer::getComputeMode() {
    return compute_mode;
}

void Displayer::showFPS() {
    double current_time = glfwGetTime();
    frame_count++;

    double time_interval = current_time - previous_time;
    if (time_interval > 1.0f) {
        double fps = frame_count / (time_interval);
        previous_time = current_time;

        char title[256];
        title[255] = '\0';
        snprintf(title, 255, "%.2f", fps);
        glfwSetWindowTitle(window, title);
        frame_count = 0;
        if (current_time > 10.0f) {
            average_fps_sum += fps;
            average_fps_count += 1;
        }
    }
}
