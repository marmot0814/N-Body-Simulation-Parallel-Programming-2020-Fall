#pragma once
#include "GLM/glm.hpp"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum class DIRECTION {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// An abstract camera class that processes input and calculates the
// corresponding Eular Angles, Vectors and Matrices for use in OpenGL
struct Camera {
    // Camera Attributes
    glm::vec3 world_up;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    // Eular Angles
    GLfloat yaw;
    GLfloat pitch;

    // Camera options
    GLfloat movement_speed;
    GLfloat mouse_sensitivity;
    GLfloat fov;

    Camera();

    // Returns the view matrix calculated using Eular Angles and the LookAt
    // Matrix
    glm::mat4 GetViewMatrix();

    // Processes input received from any keyboard-like input system. Accepts
    // input parameter in the form of camera defined ENUM (to abstract it from
    // windowing systems)
    void ProcessKeyboard(DIRECTION, GLfloat);

    // Processes input received from a mouse input system. Expects the offset
    // value in both the x and y direction.
    void ProcessMouseMovement(GLfloat, GLfloat);

    // Calculates the front vector from the Camera's (updated) Eular Angles
    void UpdateCameraVectors();

    // Processes input received from a mouse scroll-wheel event. Only requires
    // input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat);
};
