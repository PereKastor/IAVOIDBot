#pragma once

#include "Globals.h"
#include "TurnInfo.h"
#include "Logger.h"
#include <ctime>
#include <chrono>

#ifdef DEBUG_FRAMEWORK
	#define BOT_DEBUG
#endif

#ifdef BOT_DEBUG
	#define BOT_LOG(logger, text, autoEndLine) logger.Log(text, autoEndLine)
#else
	#define BOT_LOG(logger, text, autoEndLine) 0
#endif

class BotLogicIF;

class Bot
{

	enum EBotState
	{
		BotState_None,
		BotState_NotLoaded,
		BotState_Loading,
		BotState_WaitingCommand,
		BotState_Initializing,
		BotState_PlayTurn,
		BotState_Exiting
	};

public:
	Bot(BotLogicIF* _pBotLogic);
	virtual ~Bot();

	void Configure(int argc, char *argv[]);
	void Run();
	

protected:
	BotLogicIF* mpBotLogic = NULL;
	EBotState mState = BotState_None;
	EBotState mNextState = BotState_None;
	bool mbCompleted = false;
	std::string mLogPath;

#ifdef BOT_DEBUG
	Logger mLogger;
#endif

	void SetBotLogic(BotLogicIF* _pBotLogic);

	void Update();
	
	void Load();
	void Init();
	void PlayTurn();
	void WaitCommand();
	void Exit();

	void SetNextState(EBotState _nextState);
	static const std::string& GetNextStateName(EBotState _botState);

	void Sleep(clock_t sec);
};

