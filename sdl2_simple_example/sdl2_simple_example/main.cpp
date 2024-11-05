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
struct Mesh {
    vector<GLfloat> vertices;
    vector<GLuint> indices;
    //GLuint VAO, VBO, EBO, TBO;
    vector<GLfloat> uvCoords;

};

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
bool modelLoaded = false;
vector<Mesh> meshes;


GLuint loadTexture(const char* path) {
    int width, height, channels;
    unsigned char* imageData = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

    if (imageData == nullptr) {
        std::cerr << "Error: No se pudo cargar la textura " << textureFile << std::endl;
        return 0;
    }
    GLuint textID;
    glGenTextures(1, &textID);
    glBindTexture(GL_TEXTURE_2D, textID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(imageData);
    return textID;
}


void loadFBX(const string& filePath) {
    // Intentar cargar el archivo FBX
    const aiScene* scene = aiImportFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene) {
        std::cerr << "Error en cargar el archivo: " << filePath << "\n" << aiGetErrorString() << std::endl;
        return;
    }

    meshes.clear();  // Limpiar las mallas anteriores

    // Procesar mallas en el archivo FBX
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* aimesh = scene->mMeshes[i];
        printf("\nMalla %u:\n", i);
        printf(" Numero de vértices: %u\n", aimesh->mNumVertices);

        Mesh mesh;

        for (unsigned int v = 0; v < aimesh->mNumVertices; v++) {
            mesh.vertices.push_back(aimesh->mVertices[v].x);
            mesh.vertices.push_back(aimesh->mVertices[v].y);
            mesh.vertices.push_back(aimesh->mVertices[v].z);

            if (aimesh->HasTextureCoords(0)) {
                mesh.uvCoords.push_back(aimesh->mTextureCoords[0][v].x);
                mesh.uvCoords.push_back(aimesh->mTextureCoords[0][v].y);
            }
            else {
                mesh.uvCoords.push_back(0.0f);
                mesh.uvCoords.push_back(0.0f);
            }
        }
        for (unsigned int f = 0; f < aimesh->mNumFaces; f++) {
            const aiFace& face = aimesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.indices.push_back(face.mIndices[j]);
            }
        }
        meshes.push_back(mesh);
    }
    modelLoaded = true;  // Indicar que el nuevo modelo se ha cargado
}


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
    if (!modelLoaded) return;  // Evitar renderizado si no hay modelo cargado

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
    glBindTexture(GL_TEXTURE_2D, textureID);
    for (const auto& mesh : meshes) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, mesh.uvCoords.data());
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopMatrix();
    glFlush();
}





static void init_openGL() {
    glewInit();
    if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API no est� disponible.");
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
                char* droppedFile = event.drop.file;
                std::string filepath(droppedFile);
                std::string extension = filepath.substr(filepath.find_last_of(".") + 1);

                if (extension == "png" || extension == "dds") {
                    if (textureID != 0) {
                        glDeleteTextures(1, &textureID);
                        textureID = 0;
                    }
                    textureID = loadTexture(filepath.c_str());
                    if (textureID == 0) {
                        std::cerr << "Error al cargar la textura " << filepath << std::endl;
                    }
                } else if (extension == "fbx") {
                    loadFBX(filepath);  // Cargar el nuevo modelo FBX
                } else {
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

    init_openGL();
    loadFBX(file);
    textureID = loadTexture(textureFile);
    if (textureID == 0) {
        cerr << "Error: La textura no se pudo cargar correctamente." << endl;
        return -1;
    }


    while (processEvents()) {
        const auto t0 = hrclock::now();
        float deltaTime = chrono::duration<float>(FRAME_DT).count();

        if (isRightClicking) {
            moveCameraWASD(deltaTime);
        }

        render();
        window.swapBuffers();

        const auto t1 = hrclock::now();
        const auto dt = t1 - t0;
        if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
    }

    return 0;
}
