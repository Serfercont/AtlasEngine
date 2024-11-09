#pragma once
#include <string>
#include <vector>

class ModuleInterface {

public:
	std::vector<std::string> logMessages;

	double frameRate;

	void InitImGuiMenus();

	void RenderImGuiMenus(bool& showAbout);

	void LogInConsole(int Listsize);

	void SaveMessage(const char* message);

	void Docking();




};


