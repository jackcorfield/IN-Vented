#pragma once
#include <vector>
#include <string>

enum eLogLevel
{
	LL_ERROR = 0,
	LL_WARNING,
	LL_DEBUG,
};

struct DebugLog
{
	eLogLevel logLevel;
	std::string msg;
	std::string time;
};

class Debugger
{
	

public:
	Debugger();

	eLogLevel m_LogLevel;
	void Log(std::string msg);
	void Log(std::string, eLogLevel logLevel);
	inline std::vector<DebugLog> GetDebug() { return mConsoleMsg; };

private:
	
	std::string GetTime();
	std::vector<DebugLog> mConsoleMsg;
};

