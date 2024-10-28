#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Para las transformaciones de c�mara
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

using namespace std;
using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(1080, 720);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

const char* defaultFile = "../../FBX/BakerHouse.fbx";
const char* cubeFile = "../../FBX/BakerHouse.fbx";

struct Mesh {
    vector<GLfloat> vertices;
    vector<GLuint> indices;
    GLuint VAO, VBO, EBO;
};
// Variables globales para controlar la rotaci�n
float rotationAngleX = 0.0f; // �ngulo de rotaci�n alrededor del eje X
float rotationAngleY = 0.0f; // �ngulo de rotaci�n alrededor del eje Y

bool isDragging = false;      // Indica si el rat�n est� siendo arrastrado
int lastMouseX = 0;          // �ltima posici�n X del rat�n
int lastMouseY = 0;          // �ltima posici�n Y del rat�n

float cameraDistance = 5.0f; // Distancia de la c�mara al objeto
float cameraAngleX = 0.0f;   // �ngulo de rotaci�n alrededor del eje X
float cameraAngleY = 0.0f;   // �ngulo de rotaci�n alrededor del eje Y

vector<Mesh> meshes;

void setupMesh(Mesh& mesh) {
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(GLfloat), mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);

    // Atributo de posici�n del v�rtice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void loadFBX(const char* file) {
    const aiScene* scene = aiImportFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "Error en cargar el archivo: %s\n", aiGetErrorString());
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* aimesh = scene->mMeshes[i];
        printf("\nMalla %u:\n", i);
        printf(" Numero de v�rtices: %u\n", aimesh->mNumVertices);

        Mesh mesh;

        for (unsigned int v = 0; v < aimesh->mNumVertices; v++) {
            mesh.vertices.push_back(aimesh->mVertices[v].x);
            mesh.vertices.push_back(aimesh->mVertices[v].y);
            mesh.vertices.push_back(aimesh->mVertices[v].z);
        }
        for (unsigned int f = 0; f < aimesh->mNumFaces; f++) {
            const aiFace& face = aimesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.indices.push_back(face.mIndices[j]);
            }
        }
        setupMesh(mesh);

        meshes.push_back(mesh);
    }
}


void render()
{
    // Limpiar la pantalla y el buffer de profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Establecer la matriz de proyecci�n (perspectiva)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)WINDOW_SIZE.x / (double)WINDOW_SIZE.y, 0.1, 100.0); // Proyecci�n perspectiva

    // Calcular la posici�n de la c�mara
    float cameraX = cameraDistance * sin(glm::radians(cameraAngleY)) * cos(glm::radians(cameraAngleX));
    float cameraY = cameraDistance * sin(glm::radians(cameraAngleX));
    float cameraZ = cameraDistance * cos(glm::radians(cameraAngleY)) * cos(glm::radians(cameraAngleX));

    // Establecer la matriz de modelo-vista
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ,  // C�mara en (cameraX, cameraY, cameraZ)
        0.0f, 0.0f, 0.0f,         // Mira hacia el origen
        0.0f, 1.0f, 0.0f);        // Eje Y hacia arriba

    // Dibujar todas las mallas cargadas de Assimp
    for (const auto& mesh : meshes)
    {
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    glFlush(); // Asegurarse de que todos los comandos se han ejecutado
}




static void init_openGL() {
    glewInit();
    if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API no est� disponible.");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}



static bool processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Procesa los eventos de ImGui primero
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                isDragging = true;  // Inicia el arrastre cuando se presiona el bot�n izquierdo
                SDL_GetMouseState(&lastMouseX, &lastMouseY); // Guarda la posici�n inicial del rat�n
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                isDragging = false; // Termina el arrastre cuando se suelta el bot�n
            }
            break;
        case SDL_MOUSEMOTION:
            if (isDragging) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY); // Obtiene la posici�n actual del rat�n

                // Calcula el cambio en la posici�n del rat�n
                int deltaX = mouseX - lastMouseX;
                int deltaY = mouseY - lastMouseY;

                // Actualiza los �ngulos de la c�mara basados en el movimiento del rat�n
                cameraAngleX += deltaY * 0.5f; // Ajusta la sensibilidad seg�n sea necesario
                cameraAngleY -= deltaX * 0.5f; // Ajusta la sensibilidad seg�n sea necesario

                // Guarda la nueva posici�n del rat�n
                lastMouseX = mouseX;
                lastMouseY = mouseY;
            }
            break;
        case SDL_MOUSEWHEEL:
            // Aumentar o disminuir la distancia de la c�mara
            if (event.wheel.y > 0) {
                cameraDistance -= 0.5f; // Alejar la c�mara
            }
            else if (event.wheel.y < 0) {
                cameraDistance += 0.5f; // Acercar la c�mara
            }
            break;
        }
    }
    return true;
}


int main(int argc, char** argv) {
    MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);

    init_openGL();
    loadFBX(defaultFile); // Cargar la malla desde el archivo FBX

    while (processEvents()) {
        const auto t0 = hrclock::now();

        render(); // Llamar a la funci�n render en lugar de display_func
        window.swapBuffers();
        const auto t1 = hrclock::now();
        const auto dt = t1 - t0;
        if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
    }

    return 0;
}
