#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class Camera {
public:
    Camera();

    void startDragging(int mouseX, int mouseY, bool isAltPressed);
    void stopDragging();
    void handleMouseMotion(int mouseX, int mouseY);
    void zoom(int yScroll);
    void reset();
    void moveWASD(float deltaTime, bool shiftPressed);

    const glm::vec3& getPosition() const;
    const glm::vec3& getTarget() const;
    const glm::vec3& getUp() const;

private:
    float distance;
    float angleX, angleY;
    glm::vec3 position, target, up;
    bool isDragging;
    int lastMouseX, lastMouseY;
};

#endif
