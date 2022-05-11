#include "Debugger.h"
#include <ctime>

Debugger::Debugger() :
	m_LogLevel(LL_ERROR)
{}

void Debugger::Log(std::string msg)
{
	DebugLog temp;

	temp.logLevel = m_LogLevel;
	temp.msg = msg;
	temp.time = GetTime();

	mConsoleMsg.push_back(temp);
}

void Debugger::Log(std::string msg, eLogLevel logLevel)
{
	DebugLog temp;

	temp.logLevel = logLevel;
	temp.msg = msg;
	temp.time = GetTime();

	mConsoleMsg.push_back(temp);
}

std::string Debugger::GetTime()
{
	time_t systemTime = time(nullptr);
	char chTime[100];
	tm tm_info;
	localtime_s(&tm_info, &systemTime);

	strftime(chTime, 100, "[%H:%M:%S]", &tm_info);

	std::string strTime = chTime;

	return strTime;
}