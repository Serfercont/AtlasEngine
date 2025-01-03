#include <GL/glew.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "MyWindow.h"
#include "ModuleInterface.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "ModuleInterface.h"
using namespace std;
extern ModuleInterface moduleInterface;

MyWindow::MyWindow(const std::string& title, int w, int h) : _width(w), _height(h) {

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
    if (!_window) throw exception(SDL_GetError());

    _ctx = SDL_GL_CreateContext(_window);
    if (!_ctx) throw exception(SDL_GetError());
    if (SDL_GL_MakeCurrent(_window, _ctx) != 0) throw exception(SDL_GetError());
    if (SDL_GL_SetSwapInterval(1) != 0) throw exception(SDL_GetError());

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplSDL2_InitForOpenGL(_window, _ctx);
    ImGui_ImplOpenGL3_Init("#version 130");
	moduleInterface.SaveMessage("[INFO][ImGui] Carga e inicializacion completadas correctamente.");
}

MyWindow::~MyWindow() {
    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(_ctx);
    SDL_DestroyWindow(static_cast<SDL_Window*>(_window));
}

void MyWindow::swapBuffers(){

	moduleInterface.drawMainMenuBar(showAbout);

    SDL_GL_SwapWindow(static_cast<SDL_Window*>(_window));
}


