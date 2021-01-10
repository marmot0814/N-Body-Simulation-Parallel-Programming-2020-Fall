#include "camera.h"

Camera::Camera() {
    world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    position = glm::vec3(0.0f, 0.0f, 3.0f);
    yaw = 0.0f;
    pitch = 0.0f;
    fov = 60.0f;
    movement_speed = 3.0f;
    mouse_sensitivity = 0.05f;

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 temp;
    temp.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    temp.y = sin(glm::radians(pitch));
    temp.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(temp);
    // Also re-calculate the right and up vector
    right = glm::normalize(glm::cross(front, world_up));
                            // Normalize the vectors, because their length
                            // gets closer to 0 the more you look up or down
                            // which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}

void Camera::ProcessMouseScroll(GLfloat y_offset) {
    if (fov >= 1.0f && fov <= 120.0f) fov -= y_offset * 5;
    if (fov <= 1.0f) fov = 1.0f;
    if (fov >= 120.0f) fov = 120.0f;
}

void Camera::ProcessMouseMovement(GLfloat x_offset, GLfloat y_offset) {
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // update front, right and up Vectors using the updated Eular angles
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(DIRECTION direction, GLfloat delta_time) {
    GLfloat distance = movement_speed * delta_time;

    switch (direction) {
        case DIRECTION::FORWARD:
            position += front * distance;
            break;
        case DIRECTION::BACKWARD:
            position -= front * distance;
            break;
        case DIRECTION::LEFT:
            position -= right * distance;
            break;
        case DIRECTION::RIGHT:
            position += right * distance;
            break;
    }
}
