#pragma once

#include <iostream>
#include <string>

#ifdef DEBUG_FRAMEWORK
	#define LEVELINFO_DEBUG
#endif

class Logger;

struct LevelInfo
{	
	int rowCount;
	int colCount;
	int loadDelay;
	int initDelay;
	int turnDelay;
	bool bOmnicientMode;
	int visionRange;

	std::string serializedData;

	void LogDebugInfo(Logger& _logger);

};

std::istream& operator>>(std::istream& _is, LevelInfo& _levelInfo);

