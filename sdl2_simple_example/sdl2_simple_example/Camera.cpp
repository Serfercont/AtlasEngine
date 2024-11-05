#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

Camera::Camera()
    : distance(5.0f), angleX(0.0f), angleY(0.0f), position(0.0f, 0.0f, 5.0f), target(0.0f), up(0.0f, 1.0f, 0.0f), isDragging(false) {}

void Camera::startDragging(int mouseX, int mouseY, bool isAltPressed) {
    if (isAltPressed) {
        isDragging = true;
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
}

void Camera::stopDragging() {
    isDragging = false;
}

void Camera::handleMouseMotion(int mouseX, int mouseY) {
    if (!isDragging) return;

    int deltaX = mouseX - lastMouseX;
    int deltaY = mouseY - lastMouseY;

    angleX += deltaY * 0.5f;
    angleY -= deltaX * 0.5f;

    angleX = glm::clamp(angleX, -89.0f, 89.0f);

    position.x = distance * cos(glm::radians(angleX)) * sin(glm::radians(angleY));
    position.y = distance * sin(glm::radians(angleX));
    position.z = distance * cos(glm::radians(angleX)) * cos(glm::radians(angleY));

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    target = glm::vec3(0.0f);
}

void Camera::zoom(int yScroll) {
    distance += (yScroll > 0) ? -0.5f : 0.5f;
    distance = glm::clamp(distance, 1.0f, 50.0f);

    position.x = distance * cos(glm::radians(angleX)) * sin(glm::radians(angleY));
    position.y = distance * sin(glm::radians(angleX));
    position.z = distance * cos(glm::radians(angleX)) * cos(glm::radians(angleY));
}

void Camera::reset() {
    target = glm::vec3(0.0f);
    position = glm::vec3(0.0f, 0.0f, 5.0f);
    angleX = 0.0f;
    angleY = 0.0f;
    distance = 5.0f;
}

void Camera::moveWASD(float deltaTime, bool shiftPressed) {
    const float baseSpeed = 2.5f;
    float speed = shiftPressed ? baseSpeed * 2.0f : baseSpeed;

    const Uint8* state = SDL_GetKeyboardState(NULL);
    glm::vec3 direction(0.0f);

    if (state[SDL_SCANCODE_W]) direction.z += speed * deltaTime;
    if (state[SDL_SCANCODE_S]) direction.z -= speed * deltaTime;
    if (state[SDL_SCANCODE_A]) direction.x -= speed * deltaTime;
    if (state[SDL_SCANCODE_D]) direction.x += speed * deltaTime;
    if (state[SDL_SCANCODE_Q]) direction.y += speed * deltaTime;
    if (state[SDL_SCANCODE_E]) direction.y -= speed * deltaTime;

    glm::vec3 forward = glm::normalize(target - position);
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    position += forward * direction.z + right * direction.x + up * direction.y;
    target = position + forward;
}

