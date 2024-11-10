#include "Logger.h"
#include "imgui.h"

Logger logger;

void Logger::LogInConsole()
{
    for (size_t i = 0; i < LogSize; i++)
    {
        ImGui::Text("%s", logMessages[i].c_str());
    }
}

void Logger::SaveMessage(const char* message)
{
    logMessages.push_back(message);
    LogSize++;
}

void Logger::ClearLog()
{
    logMessages.clear();
    LogSize = 0;
}

