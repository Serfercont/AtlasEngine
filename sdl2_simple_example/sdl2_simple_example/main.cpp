

#include <GL/glew.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <SDL2/SDL_events.h>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_mouse.h>
#include "imgui_impl_sdl2.h"
#include "MyWindow.h"
#include <GL/glu.h>
#include <IL/il.h>
#include "Importer.h"
#include "Renderer.h"
#include "Camera.h"

using namespace std;
using hrclock = chrono::high_resolution_clock;

static const glm::ivec2 WINDOW_SIZE(1080, 720);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

const char* file = "../../FBX/BakerHouse.fbx";
const char* textureFile = "../../FBX/Baker_house.png";

bool isRightClicking = false;
bool shiftPressed = false;

Importer importer;
Renderer renderer;
Camera camera;

static void initOpenGL() {
    glewInit();
    if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API no está disponible.");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

static bool processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                camera.startDragging(event.button.x, event.button.y, SDL_GetModState() & KMOD_ALT);
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                isRightClicking = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                camera.stopDragging();
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                isRightClicking = false;
            }
            break;
        case SDL_MOUSEMOTION:
            camera.handleMouseMotion(event.motion.x, event.motion.y);
            break;
        case SDL_MOUSEWHEEL:
            camera.zoom(event.wheel.y);
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_f) {
                camera.reset();
            }
            if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                shiftPressed = true;
            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                shiftPressed = false;
            }
            break;
        case SDL_DROPFILE: {
            char* droppedFile = event.drop.file;
            std::string filepath(droppedFile);
            std::string extension = filepath.substr(filepath.find_last_of(".") + 1);

            if (extension == "png" || extension == "dds") {
                GLuint textureID = importer.loadTexture(filepath.c_str());
                if (textureID == 0) {
                    std::cerr << "Error al cargar la textura " << filepath << std::endl;
                }
            }
            else if (extension == "fbx") {
                importer.loadFBX(filepath);  // Cargar el nuevo modelo FBX
            }
            else {
                std::cerr << "Tipo de archivo no compatible: " << filepath << std::endl;
            }
            SDL_free(droppedFile);
            break;
        }
        }
    }
    return true;
}

int main(int argc, char** argv) {
    MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
    initOpenGL();

    importer.loadFBX(file);
    GLuint textureID = importer.loadTexture(textureFile);

    while (processEvents()) {
        const auto t0 = hrclock::now();
        float deltaTime = chrono::duration<float>(FRAME_DT).count();

        if (isRightClicking) {
            camera.moveWASD(deltaTime, shiftPressed);
        }

        renderer.render(importer.getMeshes(), textureID, camera);
        window.swapBuffers();

        const auto t1 = hrclock::now();
        const auto dt = t1 - t0;
        if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
    }

    return 0;
}
