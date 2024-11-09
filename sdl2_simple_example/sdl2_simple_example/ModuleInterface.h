#pragma once
#include <string>
#include <vector>

	extern std::vector<std::string> logMessages;

	extern double frameRate;

	void InitImGuiMenus();

	void RenderImGuiMenus(bool& showAbout);

	void LogInConsole(int Listsize);

	void SaveMessage(const char* message);

	void Docking();


