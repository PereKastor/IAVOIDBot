#pragma once

#include <vector>

#include "Globals.h"

struct TurnInfo;
struct LevelInfo;

class BotLogicIF
{
public:
	virtual ~BotLogicIF() {};

	virtual void Configure(int argc, char *argv[], const std::string& _logpath) = 0;
	virtual void Load() = 0;
	virtual void OnBotInitialized() = 0;
	virtual void Init(LevelInfo& _levelInfo) = 0;
	virtual void FillActionList(TurnInfo& _turnInfo, std::vector<Action*>& _actionList) = 0;   //calculate actions for a single turn
	virtual void Exit() = 0;
};

