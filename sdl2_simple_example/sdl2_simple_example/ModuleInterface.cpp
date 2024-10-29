#include "ModuleInterface.h"
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"


void RenderImGuiMenus(bool& showAbout)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();


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
                if (ImGui::MenuItem("Cube", "", false, false))
                {
                    //Create cube
                }

                if (ImGui::MenuItem("Sphere", "", false, false))
                {
                    //Create sphere
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

        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Test"))
            {

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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

