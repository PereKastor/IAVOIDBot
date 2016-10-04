#include "LevelInfo.h"

#include <string>
#include <sstream>
#include <fstream>

#include "Logger.h"

void LevelInfo::LogDebugInfo(Logger& _logger)
{
	_logger.Log(serializedData, true);
}

//input function
std::istream& operator>>(std::istream& _is, LevelInfo& _levelInfo)
{
	std::getline(_is, _levelInfo.serializedData);
	std::stringstream ss(_levelInfo.serializedData);

	std::string varName;
	while (!ss.eof())
	{
		ss >> varName;

		if (varName == "Rows")
		{
			ss >> _levelInfo.rowCount;
		}
		else if (varName == "Cols")
		{
			ss >> _levelInfo.colCount;
		}
		else if (varName == "LoadDelay")
		{
			ss >> _levelInfo.loadDelay;
		}
		else if (varName == "InitDelay")
		{
			ss >> _levelInfo.initDelay;
		}
		else if (varName == "TurnDelay")
		{
			ss >> _levelInfo.turnDelay;
		}
		else if (varName == "OmniscientMode")
		{
			ss >> _levelInfo.bOmnicientMode;
		}
		else if (varName == "VisionRange")
		{
			ss >> _levelInfo.visionRange;
		}
	}

	return _is;
}