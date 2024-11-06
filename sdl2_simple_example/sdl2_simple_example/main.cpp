#include <GL/glew.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_events.h>
#include "MyWindow.h"
#include "ModuleScene.h"
#include "Mesh.h"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "imgui_impl_sdl2.h"
#include <stdio.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL2/SDL_mouse.h>
#include <GL/glu.h>
#include <IL/il.h>
#include <locale>
#include <codecvt>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
std::wstring convertToWstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

using namespace std;
using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(1080, 720);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

const char* file = "../../FBX/BakerHouse.fbx";
const char* textureFile = "../../FBX/Baker_house.png";

GLuint textureID;

ModuleImporter importer;
ModuleScene scene;

bool isRightClicking = false;
bool isAltClicking = false;
bool shiftPressed = false;

bool isDragging = false;
int lastMouseX = 0;
int lastMouseY = 0;

float cameraDistance = 5.0f;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;

vec3 cameraPosition(0.0f, 0.0f, 5.0f);
vec3 cameraTarget(0.0f, 0.0f, 0.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);



void moveCameraWASD(float deltaTime) {
    const float baseSpeed = 2.5f;
    float speed = shiftPressed ? baseSpeed * 2.0f : baseSpeed;

    const Uint8* state = SDL_GetKeyboardState(NULL);
    vec3 direction(0.0f);

    if (state[SDL_SCANCODE_W]) direction.z += speed * deltaTime;
    if (state[SDL_SCANCODE_S]) direction.z -= speed * deltaTime;
    if (state[SDL_SCANCODE_A]) direction.x -= speed * deltaTime;
    if (state[SDL_SCANCODE_D]) direction.x += speed * deltaTime;
    if (state[SDL_SCANCODE_Q]) direction.y += speed * deltaTime;
    if (state[SDL_SCANCODE_E]) direction.y -= speed * deltaTime;

    vec3 forward = glm::normalize(cameraTarget - cameraPosition);
    vec3 right = glm::normalize(glm::cross(forward, cameraUp));
    cameraPosition += forward * direction.z + right * direction.x + cameraUp * direction.y;
    cameraTarget = cameraPosition + forward;
}

void rotateCameraWithMouse(int deltaX, int deltaY) {
    cameraAngleX += deltaY * 0.5f;
    cameraAngleY -= deltaX * 0.5f;

    cameraAngleX = glm::clamp(cameraAngleX, -89.0f, 89.0f);

    cameraPosition.x = cameraDistance * cos(glm::radians(cameraAngleX)) * sin(glm::radians(cameraAngleY));
    cameraPosition.y = cameraDistance * sin(glm::radians(cameraAngleX));
    cameraPosition.z = cameraDistance * cos(glm::radians(cameraAngleX)) * cos(glm::radians(cameraAngleY));

    cameraTarget = vec3(0.0f);
}

void updateCameraPosition() {
    cameraPosition.x = cameraDistance * cos(glm::radians(cameraAngleX)) * sin(glm::radians(cameraAngleY));
    cameraPosition.y = cameraDistance * sin(glm::radians(cameraAngleX));
    cameraPosition.z = cameraDistance * cos(glm::radians(cameraAngleX)) * cos(glm::radians(cameraAngleY));
}


void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)WINDOW_SIZE.x / (double)WINDOW_SIZE.y, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
        cameraTarget.x, cameraTarget.y, cameraTarget.z,
        cameraUp.x, cameraUp.y, cameraUp.z);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, importer.getTextureID());

    // Renderizar todos los GameObjects en la escena
    scene.renderMeshes();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glPopMatrix();

    glFlush();
}


static void init_openGL() {
    glewInit();
    if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API no esta disponible.");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    ilInit();
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
                if (SDL_GetModState() & KMOD_ALT) {
                    isDragging = true;
                    isAltClicking = true;
                    SDL_GetMouseState(&lastMouseX, &lastMouseY);
                }
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                isRightClicking = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                isDragging = false;
                isAltClicking = false;
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                isRightClicking = false;
            }
            break;
        case SDL_MOUSEMOTION:
            if (isDragging) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                int deltaX = mouseX - lastMouseX;
                int deltaY = mouseY - lastMouseY;

                rotateCameraWithMouse(deltaX, deltaY);

                lastMouseX = mouseX;
                lastMouseY = mouseY;
            }
            break;
        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) {
                cameraDistance -= 0.5f;
            }
            else if (event.wheel.y < 0) {
                cameraDistance += 0.5f;
            }
            cameraDistance = glm::clamp(cameraDistance, 1.0f, 50.0f);

            updateCameraPosition();
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_f) {
                cameraTarget = vec3(0.0f);
                cameraPosition = vec3(0.0f, 0.0f, 5.0f);
            }
            else if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                shiftPressed = true;
            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                shiftPressed = false;
            }
            break;
        case SDL_DROPFILE: {
            //    char* droppedFile = event.drop.file;
            //    printf("Archivo arrastrado: %s\n", droppedFile);

            //    // Liberar la textura anterior si ya estaba cargada
            //    if (textureID != 0) {
            //        glDeleteTextures(1, &textureID);
            //        textureID = 0;
            //    }

            //    // Limpiar las mallas cargadas anteriormente
            //    meshes.clear();

            //    // Cargar el nuevo archivo FBX
            //    loadFBX(droppedFile);

            //    // Volver a cargar la textura
            //    textureID = loadTexture(droppedFile); // Supón que el archivo drop contiene la textura también
            //    if (textureID == 0) {
            //        std::cerr << "Error: No se pudo cargar la nueva textura " << droppedFile << std::endl;
            //    }

            //    // Liberar el path de archivo
            //    SDL_free(droppedFile);
            //}


        }

        }
       
    }
    return true;
}
int main(int argc, char** argv) {
    try {
        MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
        importer.setWindow(&window);
        init_openGL();

        // Cargar el modelo y la textura
        if (!importer.loadFBX(file, &scene)) {  // Pasamos `&scene` a `loadFBX`
            std::cerr << "Error al cargar el archivo FBX: " << file << std::endl;
            return -1;
        }

        if ((textureID = importer.loadTexture(textureFile)) == 0) {
            std::cerr << "Error al cargar la textura: " << textureFile << std::endl;
            return -1;
        }

        while (true) {
            if (!processEvents()) {
                break;
            }
            render();
            window.swapBuffers();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Excepción: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Excepción desconocida." << std::endl;
    }
    return 0;
}