#pragma once

#include <string>
#include <vector>
#include <imgui.h>
#include "ModuleScene.h"

class ModuleInterface {
public:
    ModuleInterface();
    void SaveMessage(const char* message);
    void drawMainMenuBar(bool& showAbout);
    void setScene(ModuleScene* scene) {
        this->scene = scene;
    }

private:
   
    void drawConsole();
    void drawConfig();
    void drawHierarchy();
    void drawInspector();
    void LogInConsole(int ListSize);
    void Docking();

    bool showConsole = true;
    bool showConfiguration = false;
    bool showHierarchy = true;
    bool showInspector = true;  
    bool useCheckerTexture = false;

    std::vector<std::string> logMessages;
    ModuleScene* scene; 
};
