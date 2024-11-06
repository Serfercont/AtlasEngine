#pragma once
#include <vector>
#include <string>

class Logger
{
public:
	void LogInConsole();

	void SaveMessage(const char* message);

	void ClearLog();

public:

	std::vector<std::string> logMessages;
	int LogSize = 0;

};

extern Logger logger;

