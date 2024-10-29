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

const char* file = "C:\\Users\\sergiofc6\\Downloads\\BakerHouse.fbx";
const char* textureFile = "C:\\Users\\sergiofc6\\Downloads\\Baker_house.png";

struct Mesh {
    vector<GLfloat> vertices;
    vector<GLuint> indices;
    GLuint VAO, VBO, EBO, TBO;
	vector<GLfloat> uvCoords;
	GLuint textureID;
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

vector<Mesh> meshes;
void setupMesh(Mesh& mesh) {
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);
    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(GLfloat), mesh.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.uvCoords.size() * sizeof(GLfloat), mesh.uvCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}



GLuint loadTexture(const char* path) {
    ILuint imageID;
    GLuint textureID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);
    if (!ilLoadImage(path)) {
        std::cerr << "Error al cargar la textura: " << path << std::endl;
        ilDeleteImages(1, &imageID);
        return 0;  // Retorna 0 si falla la carga
    }
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    ilDeleteImages(1, &imageID);
    std::cout << "Textura cargada correctamente: " << path << std::endl;
    return textureID;  // Retornar el ID de la textura generada
}


void loadFBX() {
    const aiScene* scene = aiImportFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "Error en cargar el archivo: %s\n", aiGetErrorString());
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* aimesh = scene->mMeshes[i];
        printf("\nMalla %u:\n", i);
        printf(" Numero de vértices: %u\n", aimesh->mNumVertices);

        Mesh mesh;

        for (unsigned int v = 0; v < aimesh->mNumVertices; v++) {
            mesh.vertices.push_back(aimesh->mVertices[v].x);
            mesh.vertices.push_back(aimesh->mVertices[v].y);
            mesh.vertices.push_back(aimesh->mVertices[v].z);

            // Coordenadas UV (si están disponibles)
            if (aimesh->mTextureCoords[0]) {
                mesh.uvCoords.push_back(aimesh->mTextureCoords[0][v].x);
                mesh.uvCoords.push_back(aimesh->mTextureCoords[0][v].y);
            }
            else {
                mesh.uvCoords.push_back(0.0f); // UV predeterminado
                mesh.uvCoords.push_back(0.0f);
            }
        }
        for (unsigned int f = 0; f < aimesh->mNumFaces; f++) {
            const aiFace& face = aimesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.indices.push_back(face.mIndices[j]);
            }
        }
        setupMesh(mesh);
		mesh.textureID = loadTexture(textureFile);
        meshes.push_back(mesh);
    }
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)WINDOW_SIZE.x / (double)WINDOW_SIZE.y, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
        cameraTarget.x, cameraTarget.y, cameraTarget.z,
        cameraUp.x, cameraUp.y, cameraUp.z);

    for (const auto& mesh : meshes) {
        glBindVertexArray(mesh.VAO);
        glBindTexture(GL_TEXTURE_2D, mesh.textureID);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);  // Desenlaza la textura
    }

    glFlush();
}




static void init_openGL() {
    glewInit();
    if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API no está disponible.");
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
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
        }
    }
    return true;
}

int main(int argc, char** argv) {
    MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);

    init_openGL();
    loadFBX();

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
