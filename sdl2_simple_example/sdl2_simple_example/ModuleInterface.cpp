#include "ModuleInterface.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "ModuleImporter.h"
#include <string>
#include <vector>
#include <iostream>
//#include "main.cpp"



bool showConsole = true;
bool showConfiguration = false;
bool showHierarchy = false;

std::vector<std::string> logMessages;
int LogSize = 0;

extern ModuleImporter importer;
extern ModuleScene scene;
extern double frameRate;

const char* CubePath = "../../FBX/Primitive/Cube.fbx";
const char* SpherePath = "../../FBX/Primitive/LaserGun_P1.fbx";

ModuleInterface::ModuleInterface()
{
}
void ModuleInterface::drawMainMenuBar(bool& showAbout)
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    Docking();


    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl + N", false, false)) {

            }

            if (ImGui::MenuItem("Open Scene", "Ctrl + O", false, false)) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Save", "Ctrl + S", false, false)) {}

            if (ImGui::MenuItem("Save As", "Ctrl + Shift + S", false, false)) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Build Settings", "Ctrl + Shift + B", false, false)) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Exit")) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, false)) {}

            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Select All", "Ctrl+A", false, false)) {}

            if (ImGui::MenuItem("Deselect All", "Shift+D", false, false)) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Duplicate", "", false, false)) {}

            if (ImGui::MenuItem("Rename", "", false, false)) {}

            if (ImGui::MenuItem("Delete", "", false, false)) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Find", "Ctrl + F", false, false)) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Preferences...", "", false, false)) {}

            if (ImGui::MenuItem("Project Settings...", "", false, false)) {}

            if (ImGui::MenuItem("Shortcuts...", "", false, false)) {}

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets")) {

            if (ImGui::MenuItem("Select All", "Ctrl+A", false, false)) {}

            if (ImGui::MenuItem("Create", "", false, false)) {}

            if (ImGui::MenuItem("Import Asset", "", false, false)) {}

            if (ImGui::MenuItem("Import Package", "", false, false)) {}

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("GameObjects")) {

            if (ImGui::MenuItem("Create empty", "Ctrl + Shift + N", false, false))
            {
                //Create empty
            }

            if (ImGui::BeginMenu("2D object"))
            {
                if (ImGui::BeginMenu("Sprites"))
                {
                    if (ImGui::MenuItem("Circle", "", false, false))
                    {
                        //Create circle
                    }

                    if (ImGui::MenuItem("Square", "", false, false))
                    {
                        //Create square
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("3D object"))
            {
                if (ImGui::MenuItem("Cube", "", false, true))
                {
                    if (!importer.loadFBX(CubePath, scene, nullptr)) {
                        std::cerr << "Error al cargar el archivo FBX: " << std::endl;
                    }
                    
                }

                if (ImGui::MenuItem("Sphere", "", false, true))
                {
                    if (!importer.loadFBX(SpherePath, scene, nullptr)) {
                        std::cerr << "Error al cargar el archivo FBX: " << std::endl;
                    }
                }

                if (ImGui::MenuItem("Capsule", "", false, false))
                {
                    //Create capsule
                }

                if (ImGui::MenuItem("Cylinder", "", false, false))
                {
                    //Create cylinder
                }

                if (ImGui::MenuItem("Plane", "", false, false))
                {
                    //Create plane
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Effects"))
            {
                if (ImGui::MenuItem("Particle System", "", false, false))
                {
                    //Create Particle System
                }
                if (ImGui::MenuItem("Trail", "", false, false))
                {
                    //Create Trail
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Light"))
            {
                if (ImGui::MenuItem("Directional Light", "", false, false))
                {
                    //Create Directional light
                }
                if (ImGui::MenuItem("Point Light", "", false, false))
                {
                    //Create Point Light
                }
                if (ImGui::MenuItem("Spot Light", "", false, false))
                {
                    //Create Spot Light
                }
                if (ImGui::MenuItem("Area Light", "", false, false))
                {
                    //Create Area light
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Audio"))
            {
                if (ImGui::MenuItem("Audio Source", "", false, false))
                {
                    //Create Audio Source
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("UI"))
            {
                if (ImGui::MenuItem("Image", "", false, false))
                {
                    //Create Audio Source
                }

                if (ImGui::MenuItem("Text", "", false, false))
                {
                    //Create Audio Source
                }

                if (ImGui::MenuItem("Panel", "", false, false))
                {
                    //Create Audio Source
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Toggle", "", false, false))
                {
                    //Create Audio Source
                }

                if (ImGui::MenuItem("Slider", "", false, false))
                {
                    //Create Audio Source
                }

                if (ImGui::MenuItem("Button", "", false, false))
                {
                    //Create Audio Source
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Canvas", "", false, false))
                {
                    //Create Audio Source
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Component")) {
            if (ImGui::MenuItem("Test"))
            {

            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Console"))
            {
                showConsole = true;
            }
            if (ImGui::MenuItem("Configuration"))
            {
                showConfiguration = true;
            }
            if (ImGui::MenuItem("Hierarchy"))
            {
                showHierarchy = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About"))
            {
                showAbout = true;
            }
            if (ImGui::MenuItem("/Github/XD")) {

            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (showAbout)
    {
        ImGui::Begin("About", &showAbout); //El &showAbout crea el boton de cerrar,cambia la variable a false cuando le das y cierra la ventana.
        ImGui::TextWrapped("Atlas Engine is a 3D rendering engine developed by Sergio Fernandez, Pau Blanco, and Carlos Gonzalez as part of the Game Engines course at CITM - UPC Terrassa in 2024."
            "The engine is built using technologies such as SDL, OpenGL, ImGUI, DevIL, Assimp, MathGeoLib, Parson, and PhysFS, enabling a smooth and efficient graphical experience for game development.");

        ImGui::Separator();
        ImGui::TextWrapped("Components:");
        ImGui::TextWrapped("Vendor: %s", glGetString(GL_VENDOR));
        ImGui::TextWrapped("Renderer: %s", glGetString(GL_RENDERER));
        ImGui::TextWrapped("OpenGL version supported %s", glGetString(GL_VERSION));
        ImGui::TextWrapped("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        ImGui::End();
    }

    if (showConsole)
    {
        drawConsole();
    }

    if (showConfiguration)
    {
		drawConfig();
        
    }

    if (showHierarchy)
    {
		drawHierarchy();
    }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ModuleInterface::drawConfig() {
    ImGui::Begin("Configuration", &showConfiguration);
    static float frameRateValues[90] = {};
    static int offset = 0;
    frameRateValues[offset] = frameRate;
    offset = (offset + 1) % IM_ARRAYSIZE(frameRateValues);
    char overlay[32];
    sprintf_s(overlay, "avg FPS: %.1f", frameRate);
    ImGui::PlotLines("FPS", frameRateValues, IM_ARRAYSIZE(frameRateValues), offset, overlay, -0.0f, 70.0f, ImVec2(0, 80.0f));
    ImGui::End();
}
void ModuleInterface::drawConsole() {
    ImGui::Begin("Console", &showConsole);
    if (ImGui::SmallButton("Clear"))
    {
        logMessages.clear();
        LogSize = 0;
    }
    ImGui::Separator();
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
    {
        LogInConsole(LogSize);
    }
    ImGui::EndChild();
    ImGui::End();
}

void ModuleInterface::LogInConsole(int ListSize)
{
    for (size_t i = 0; i < ListSize; i++)
    {
        ImGui::Text("%s", logMessages[i].c_str());
    }
}

void ModuleInterface::drawHierarchy() {
    ImGui::Begin("Hierarchy", &showHierarchy);

    // Obtener los GameObjects de la escena
    std::vector<GameObject*> gameObjects = scene->getGameObjects();
    
    // Recorrer y mostrar cada GameObject
    for (const GameObject* obj : gameObjects) {
        ImGui::Text("%s", obj->getName().c_str());
    }

    ImGui::End();
}




void ModuleInterface::SaveMessage(const char* message)
{
    logMessages.push_back(message);
    LogSize++;
}


void ModuleInterface::Docking()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Dockspace", NULL, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspaceId = ImGui::GetID("Dockspace");
    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
}


