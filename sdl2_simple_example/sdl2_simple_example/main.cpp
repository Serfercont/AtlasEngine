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
#include "ModuleInterface.h"
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

static const ivec2 WINDOW_SIZE(1280, 720);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

const char* file = "../../FBX/BakerHouse.fbx";
const char* textureFile = "../../FBX/Baker_house.png";

double frameRate = 0;
ModuleImporter importer;
ModuleScene scene;
ModuleInterface moduleInterface;

GLuint textureID;
GLuint texturaCuadros= importer.createCheckerTexture();



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
    const float baseSpeed = 5.0f;
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


    
    glDisable(GL_TEXTURE_2D);  
    glColor3f(0.8f, 0.8f, 0.8f);  
    importer.drawGrid(1.0f);  
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, importer.getTextureID());

    scene.renderMeshes();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glPopMatrix();

    glFlush();
}


static void init_openGL() {
    glewInit(); 
	moduleInterface.SaveMessage("[INFO] [Glew] Carga e inicializacion completadas correctamente");

    if (!GLEW_VERSION_3_0)
    {
        throw exception("OpenGL 3.0 API no esta disponible.");
        moduleInterface.SaveMessage("[ERROR] [OpenGL] Error al cargar o inicializar OpenGL 3.0. Verifique los controladores o la configuracion del sistema.");
    }
    else
    {
        moduleInterface.SaveMessage("[INFO] [OpenGL] Carga e inicializacion completadas correctamente.");
    }  
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);

    ilInit();
    moduleInterface.SaveMessage("[INFO][Devil] Carga e inicialización completadas correctamente.");
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
                if (SDL_GetModState)
                {
                    isRightClicking = true;
                    SDL_GetMouseState(&lastMouseX, &lastMouseY);
                }
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
            else if (event.key.keysym.sym == SDLK_t)
            {

            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                shiftPressed = false;
            }
            break;
        case SDL_DROPFILE: {
            char* droppedFile = event.drop.file;
            printf("Archivo arrastrado: %s\n", droppedFile);

            std::string filePath = droppedFile;
            std::string extension = filePath.substr(filePath.find_last_of(".") + 1);

            if (extension == "png" || extension == "jpg" || extension == "jpeg") {
                
                GLuint newTextureID = importer.loadTexture(droppedFile);
                if (newTextureID != 0) {
					moduleInterface.SaveMessage("Textura cargada correctamente con ID ");
                    

                    
                    GameObject* selectedGameObject = scene.getSelectedGameObject();
                    if (selectedGameObject != nullptr) {
                        Texture* newTexture = new Texture(newTextureID, "default");
                        selectedGameObject->setTexture(newTexture);
						moduleInterface.SaveMessage("Textura aplicada al GameObject seleccionado.");
                        
                    }
                    else {
						moduleInterface.SaveMessage("No hay un GameObject seleccionado.");
                    }
                }
                else {
					moduleInterface.SaveMessage("Error al cargar la textura: ");
                    
                }
            }
            else if (extension == "fbx" || extension == "obj") {
                if (!importer.loadFBX(droppedFile, &scene, nullptr)) {
					moduleInterface.SaveMessage("Error al cargar el modelo");
                }
            }
            else {
				moduleInterface.SaveMessage("Tipo de archivo no soportado ");
            }

            SDL_free(droppedFile);
            break;
        }
		}
	}
	return true;
}

int main(int argc, char** argv) {
    try {
        MyWindow window("SDL2 Engine Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
        importer.setWindow(&window);
        init_openGL();

		int frames = 0;
		auto startTime = hrclock::now();

        if (!importer.loadFBX(file, &scene, textureFile)) {
            std::cerr << "Error loading FBX file: " << file << std::endl;
            return -1;
        }
        moduleInterface.setScene(&scene);
        scene.drawScene();

        while (true) {
            if (!processEvents()) {
                break;
            }
            float deltaTime = chrono::duration<float>(FRAME_DT).count();

            frames++;

            const auto currentTime = hrclock::now();
            const auto dt = currentTime - startTime;
            std::chrono::duration<double> elapsed = currentTime - startTime;
           if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
            if (elapsed.count() >= 1.0) { 
                frameRate = frames / elapsed.count();
                frames = 0;
            
                startTime = currentTime;
            }
            if (isRightClicking) {
                moveCameraWASD(deltaTime);
            }
            importer.drawGrid(10.0f);
            render();
            window.swapBuffers();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}