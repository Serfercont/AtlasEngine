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
#include <functional>



bool showConsole = true;
bool showConfiguration = false;
bool showHierarchy = false;
bool showInspector = false;
std::vector<std::string> logMessages;
int LogSize = 0;

// Paleta de colores
ImVec4 color1 = ImVec4(2 / 255.0f, 49 / 255.0f, 94 / 255.0f, 1.0f);   // #005067 (Fondo Principal)
ImVec4 color2 = ImVec4(128 / 255.0f, 100 / 255.0f, 145 / 255.0f, 1.0f); // #048399 (Elementos interactivos)
ImVec4 color3 = ImVec4(254 / 255.0f, 185 / 255.0f, 198 / 255.0f, 1.0f); // #FEB9C6 (Hover suave)
ImVec4 color4 = ImVec4(185 / 255.0f, 107 / 255.0f, 133 / 255.0f, 1.0f); // #B96B85 (Elementos activos)
ImVec4 color5 = ImVec4(2 / 255.0f, 30 / 255.0f, 32 / 255.0f, 1.0f);    // #021E20 (Texto/Bordes)
extern ModuleImporter importer;
extern ModuleScene scene;
extern double frameRate;

ModuleInterface::ModuleInterface()
{
}
void ApplyCustomStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text] = color3;              // Texto principal en verde oscuro
    colors[ImGuiCol_TextDisabled] = color3;      // Texto deshabilitado (rosa suave)
    colors[ImGuiCol_WindowBg] = color1;          // Fondo principal azul oscuro
    colors[ImGuiCol_ChildBg] = color1;           // Fondo secundario
    colors[ImGuiCol_Border] = color2;            // Bordes en verde oscuro
    colors[ImGuiCol_FrameBg] = color3;           // Fondo de cuadro rosa pastel
    colors[ImGuiCol_FrameBgHovered] = color2;    // Hover en cuadro azul brillante
    colors[ImGuiCol_FrameBgActive] = color4;     // Cuadro activo rosa oscuro
    colors[ImGuiCol_TitleBg] = color1;           // Fondo t�tulo principal
    colors[ImGuiCol_TitleBgActive] = color2;     // T�tulo activo azul medio
    colors[ImGuiCol_MenuBarBg] = color1;         // Fondo barra men�
    colors[ImGuiCol_Button] = color2;            // Bot�n base azul medio
    colors[ImGuiCol_ButtonHovered] = color5;     // Bot�n hover rosa pastel
    colors[ImGuiCol_ButtonActive] = color4;      // Bot�n activo rosa oscuro
    colors[ImGuiCol_Header] = color2;            // Encabezado azul brillante
    colors[ImGuiCol_HeaderHovered] = color4;     // Hover encabezado
    colors[ImGuiCol_HeaderActive] = color4;      // Encabezado activo
    colors[ImGuiCol_ScrollbarGrab] = color2;     // Scroll bar azul medio
    colors[ImGuiCol_SliderGrab] = color3;        // Slider activo rosa oscuro
}
const char* CubePath = "../../FBX/Primitive/Cube.fbx";
const char* SpherePath = "../../FBX/Primitive/Sphere.fbx";
const char* CylinderPath = "../../FBX/Primitive/Cylinder.fbx";
const char* PlanePath = "../../FBX/Primitive/Plane.fbx";

void ModuleInterface::drawMainMenuBar(bool& showAbout)
{
    extern ModuleImporter importer;
    extern ModuleScene scene;
    ApplyCustomStyle();
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
                    if (!importer.loadFBX(CubePath, &scene, nullptr)) {
                        std::cerr << "Error al cargar el archivo FBX: " << std::endl;
                    }
                    
                }

                if (ImGui::MenuItem("Sphere", "", false, true))
                {
                    if (!importer.loadFBX(SpherePath, &scene, nullptr)) {
                        std::cerr << "Error al cargar el archivo FBX: " << std::endl;
                    }
                }

                if (ImGui::MenuItem("Capsule", "", false, false))
                {
                    //Create capsule
                }

                if (ImGui::MenuItem("Cylinder", "", false, true))
                {
                    if (!importer.loadFBX(CylinderPath, &scene, nullptr)) {
                        std::cerr << "Error al cargar el archivo FBX: " << std::endl;
                    }
                }

                if (ImGui::MenuItem("Plane", "", false, true))
                {
                    if (!importer.loadFBX(PlanePath, &scene, nullptr)) {
                        std::cerr << "Error al cargar el archivo FBX: " << std::endl;
                    }
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
            if (ImGui::MenuItem("Inspector"))
            {
                showInspector = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About"))
            {
                showAbout = true;
            }
            if (ImGui::MenuItem("Github :)")) {
                SDL_OpenURL("https://github.com/Serfercont/AtlasEngine");
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
    if (showInspector) {
        drawInspector(); 
    }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
}

void ModuleInterface::drawConfig() {
    ImGui::Begin("Configuration", &showConfiguration);
    ImGui::SeparatorText("FPS");
    static float frameRateValues[90] = {};
    static int offset = 0;
    frameRateValues[offset] = frameRate;
    offset = (offset + 1) % IM_ARRAYSIZE(frameRateValues);
    char overlay[32];
    sprintf_s(overlay, "avg FPS: %.1f", frameRate);
    ImGui::PlotLines("", frameRateValues, IM_ARRAYSIZE(frameRateValues), offset, overlay, -0.0f, 100.0f, ImVec2(ImGui::GetWindowWidth() - 15, 100.0f), 0);

    ImGui::SeparatorText("Libraries versions");

    ImGui::TextWrapped("OpenGL version: %s", glGetString(GL_VERSION));
    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    ImGui::TextWrapped("SDL version: %u.%u.%u \n", compiled.major, compiled.minor, compiled.patch);
    ImGui::TextWrapped("ImGui version: %s\n", IMGUI_VERSION);
    ImGui::TextWrapped("DevIL version: %i\n", IL_VERSION);

    ImGui::SeparatorText("VRAM");

    GLfloat memoriaTotal = 0;
    GLfloat memoriaLibre = 0;
    GLfloat memoriaUsada = 0;

    glGetFloatv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &memoriaTotal);
    glGetFloatv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &memoriaLibre);

    memoriaUsada = (memoriaTotal - memoriaLibre) / 1024;

    ImGui::TextWrapped("Total: %.2f MB", memoriaTotal / 1024);
    ImGui::TextWrapped("Free: %.2f MB", memoriaLibre / 1024);
    ImGui::TextWrapped("Used: %.2f MB", memoriaUsada);

    static float memoryValues[90] = {};
    static int memoryOffset = 0;
    memoryValues[memoryOffset] = memoriaUsada;
    memoryOffset = (memoryOffset + 1) % IM_ARRAYSIZE(memoryValues);
    char memoryOverlay[32];
    sprintf_s(memoryOverlay, "Used VRAM: %.2f", memoriaUsada);
    ImGui::PlotLines("", memoryValues, IM_ARRAYSIZE(memoryValues), memoryOffset, memoryOverlay, -0.0f, memoriaTotal / 1024, ImVec2(ImGui::GetWindowWidth() - 15, 100.0f), 0);

    ImGui::SeparatorText("RAM");

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    ImGui::TextWrapped("Total: %i MB", (statex.ullTotalPhys / (1024 * 1024)));


    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        SIZE_T privateMemoryUsage = pmc.PrivateUsage;
        float memoryUsageMB = static_cast<float>(privateMemoryUsage) / (1024.0f * 1024.0f);

        ImGui::TextWrapped("Used: %.2f MB", memoryUsageMB);

        static float ramValues[90] = {};
        static int ramOffset = 0;
        ramValues[ramOffset] = memoryUsageMB;
        ramOffset = (ramOffset + 1) % IM_ARRAYSIZE(ramValues);
        char ramOverlay[32];
        sprintf_s(ramOverlay, "Used VRAM: %.2f", memoryUsageMB);
        ImGui::PlotLines("", ramValues, IM_ARRAYSIZE(ramValues), ramOffset, ramOverlay, -0.0f, statex.ullTotalPhys / (1024 * 1024), ImVec2(ImGui::GetWindowWidth() - 15, 100.0f), 0);
    }

    ImGui::SeparatorText("Machineri");

    ImGui::TextWrapped("Renderer: %s", glGetString(GL_RENDERER));

    std::string cpuName;

    WCHAR buffer[256];
    DWORD bufferSize = sizeof(buffer);
    HKEY key;

    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key) == ERROR_SUCCESS) {
        if (RegQueryValueExW(key, L"ProcessorNameString", NULL, NULL, reinterpret_cast<LPBYTE>(buffer), &bufferSize) == ERROR_SUCCESS) {
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
            cpuName.resize(size_needed);
            WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &cpuName[0], size_needed, nullptr, nullptr);
        }
        RegCloseKey(key);
    }

    ImGui::TextWrapped("CPU: %s", cpuName.c_str());


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

    std::vector<GameObject*> gameObjects = scene->getGameObjects();

    std::function<void(const GameObject*)> drawGameObjectHierarchy = [&](const GameObject* obj) {
        if (ImGui::TreeNode(obj->getName().c_str())) {
            const std::vector<GameObject*>& children = obj->getChildren();
            for (const GameObject* child : children) {
                drawGameObjectHierarchy(child);
            }
            ImGui::TreePop();
        }
        if (ImGui::IsItemClicked()) {
            scene->selectGameObject(const_cast<GameObject*>(obj));  
        }
        };

    for (const GameObject* obj : gameObjects) {
        if (obj->getParent() == nullptr) {
            drawGameObjectHierarchy(obj);
        }
    }

    ImGui::End();
}


void ModuleInterface::drawInspector() {
    ImGui::Begin("Inspector", &showInspector); 

    if (scene->selectedGameObject != nullptr) {
        GameObject* selectedGO = scene->selectedGameObject;

        ImGui::Text("GameObject: %s", selectedGO->getName().c_str());

        ImGui::Text("Position");
        ImGui::InputFloat3("###Position", &selectedGO->getPosition()[0]);

        ImGui::Text("Rotation");
        ImGui::InputFloat3("###Rotation", &selectedGO->getRotation()[0]);

        ImGui::Text("Scale");
        ImGui::InputFloat3("###Scale", &selectedGO->getScale()[0]);

        Texture* texture = selectedGO->getTexture();
        if (texture) {
            ImGui::Separator();
            ImGui::Text("Texture Path: %s", texture->getPath().c_str());
            ImGui::Text("Texture Size: %d x %d", texture->getWidth(), texture->getHeight());
        }
        else {
            ImGui::Text("No Texture Attached");
        }

        const auto& meshes = selectedGO->getMeshes();
        if (!meshes.empty()) {
            for (size_t i = 0; i < meshes.size(); ++i) {
                Mesh* mesh = meshes[i];
                if (mesh) {
                    ImGui::Separator();
                    ImGui::Text("Mesh %zu", i + 1);
                    ImGui::Text("Vertices: %d", mesh->getVertexCount());
                    ImGui::Text("Faces: %d", mesh->getFaceCount());
                    ImGui::Text("Texture Coords: %d", mesh->getTexCoordCount());
                    ImGui::Text("Normals: %s", mesh->hasNormals() ? "Yes" : "No");
                }
            }
        }
        else {
            ImGui::Text("No Meshes Attached");
        }
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

