#include "Bot.h"

#include <assert.h>
#include <vector>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>

#include <windows.h>

#include "BotLogicIF.h"
#include "LevelInfo.h"
#include "TurnInfo.h"

#define FSM_MAX_NB_ITERATION 50

Bot::Bot(BotLogicIF* _pBotLogic)
	: mpBotLogic(_pBotLogic)
	, mLogPath("./aibot")
{

}

Bot::~Bot()
{
	BOT_LOG(mLogger, "~Bot\n", true);

	if (mpBotLogic != nullptr)
	{
		delete mpBotLogic;
	}
}

void Bot::SetBotLogic(BotLogicIF* _pBotLogic)
{
	BOT_LOG(mLogger, "SetBotLogic", true);

	if (mpBotLogic != nullptr)
	{
		delete mpBotLogic;
	}
	mpBotLogic = _pBotLogic;
}

void Bot::Run()
{
	while (!mbCompleted)
	{
		Update();
	}
}

void Bot::Update()
{
	do 
	{
		switch (mState)
		{
			case BotState_NotLoaded:
			{
				SetNextState(BotState_Loading);
			}
			break;
			
			case BotState_Loading:
			{
				Load();
			}
			break;

			case BotState_WaitingCommand:
			{
				WaitCommand();
			}
			break;

			case BotState_Initializing:
			{
				Init();
			}
			break;

			case BotState_PlayTurn:
			{
				PlayTurn();
			}
			break;

			case BotState_Exiting:
			{
				Exit();
			}
			break;

			default:
				break;
		}

		if (mState != mNextState)
		{
			mState = mNextState;
		}
		else
		{
			break;
		}
	} while (true);	
}

void Bot::Configure(int argc, char *argv[])
{
	if (argc >= 3)
	{
		std::string switchStr(argv[1]);
		if (switchStr == "-AIBotLogPath")
		{
			std::string path(argv[2]);
			mLogPath = path;
		}
	}

	CreateDirectory(mLogPath.c_str(), NULL);

#ifdef BOT_DEBUG
	mLogger.Init(mLogPath, "BotDebug.txt");
#endif
	
	BOT_LOG(mLogger, "Configure", true);

	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");
	
	mpBotLogic->Configure(argc, argv, mLogPath);

	SetNextState(BotState_NotLoaded);
}

void Bot::Load()
{
	BOT_LOG(mLogger, "Load", true);

	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");

	mpBotLogic->Load();

	std::cout << "AIBotLoaded" << std::endl;

	SetNextState(BotState_WaitingCommand);
}

void Bot::Init()
{
	BOT_LOG(mLogger, "Init", true);
	
	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");
	
	LevelInfo levelInfo;
	std::cin >> levelInfo;

#ifdef LEVELINFO_DEBUG
	Logger logger;
	logger.Init(mLogPath, "LevelInfoDebug.txt");
	levelInfo.LogDebugInfo(logger);
#endif
	

#ifdef BOT_DEBUG
	std::stringstream ss;
	ss << "LevelInfo" << std::endl;

	ss << "\tMap Size : (" << levelInfo.rowCount << ", " << levelInfo.colCount << ")" << std::endl;
	ss << "\tLoadDelay : "<< levelInfo.loadDelay << std::endl;
	ss << "\tInitDelay : " << levelInfo.initDelay << std::endl;
	ss << "\tTurnDelay : " << levelInfo.turnDelay << std::endl;
	ss << "\tOmnicientMode : " << levelInfo.bOmnicientMode << std::endl;
	ss << "\tVisionRange : " << levelInfo.visionRange << std::endl;

	BOT_LOG(mLogger, ss.str(), true);
#endif

	mpBotLogic->OnBotInitialized();

	BOT_LOG(mLogger, "Init BotLogic", true);

	mpBotLogic->Init(levelInfo);

	std::cout << "AIBotInitialized" << std::endl;

	SetNextState(BotState_WaitingCommand);
}

void Bot::PlayTurn()
{
	BOT_LOG(mLogger, "PlayTurn", true);

	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");

	std::vector<Action*> actions;

	TurnInfo turnInfo;
	std::cin >> turnInfo;

#ifdef TURNINFO_DEBUG
	Logger logger;
	logger.Init(mLogPath, "TurnInfoDebug.txt");
	turnInfo.LogDebugInfo(logger);
#endif

#ifdef BOT_DEBUG
	std::stringstream ss;
	ss << "Turn #" << turnInfo.turnNb << std::endl;

	for each (auto& npc in turnInfo.npcs)
	{
		auto& npcInfo = npc.second;
		ss << "\tNPC UID : " << npcInfo.npcID << std::endl;
		ss << "\t   Current HP : " << npcInfo.currentHP << std::endl;
		ss << "\t   Max HP : " << npcInfo.maxHP << std::endl;
		ss << "\t   TileID : " << npcInfo.tileID << std::endl;

		ss << "\t   Visible Tiles (" << npcInfo.visibleTiles.size() << ") : [";
		
		int i = 0;
		for each(auto val in npcInfo.visibleTiles)
		{
			if (i != 0)
			{
				ss << ", ";
			}
			ss << val;
			i++;			
		}
		ss << "]" << std::endl;

		ss << "\t   Visible Objects (" << npcInfo.visibleObjects.size() << ") : [";
		i = 0;
		for each(auto val in npcInfo.visibleObjects)
		{
			if (i != 0)
			{
				ss << ", ";
			}
			ss << val;
			i++;
		}
		ss << "]" << std::endl;
	}

	for each (auto& objectInfoPair in turnInfo.objects)
	{
		const ObjectInfo& objectInfo = objectInfoPair.second;
		
		ss << "\tObject UID : " << objectInfo.objectID << std::endl;

		ss << "\t   Tile ID : " << objectInfo.tileID << std::endl;
		ss << "\t   Object Type : " << objectInfo.objectType << std::endl;

		ss << "\t   Object Edge Costs : " << std::endl;
		ss << "\t      N  : " << objectInfo.edgesCost[0] << std::endl;
		ss << "\t      NE : " << objectInfo.edgesCost[1] << std::endl;
		ss << "\t      E  : " << objectInfo.edgesCost[2] << std::endl;
		ss << "\t      SE : " << objectInfo.edgesCost[3] << std::endl;
		ss << "\t      S  : " << objectInfo.edgesCost[4] << std::endl;
		ss << "\t      SW : " << objectInfo.edgesCost[5] << std::endl;
		ss << "\t      W  : " << objectInfo.edgesCost[6] << std::endl;
		ss << "\t      NW : " << objectInfo.edgesCost[7] << std::endl;
	}

	BOT_LOG(mLogger, ss.str(), false);
#endif
		
	mpBotLogic->FillActionList(turnInfo, actions);

	std::stringstream actionSS;
	actionSS << "TurnInfo " << actions.size();

	for each (auto* pAction in actions)
	{
		if (pAction->actionType == Action_Move)
		{
			Move* pMove = (Move*)pAction;
			actionSS << " " << pMove->unitID << " " << (int)pMove->actionType << " " << (int)pMove->direction;
		}

		delete pAction;
	}

	std::cout << actionSS.str() << std::endl;

#ifdef BOT_DEBUG
	std::stringstream ss2;
	ss2 << "\t Actions : " << " " << actionSS.str();
	BOT_LOG(mLogger, ss2.str(), true);
#endif

	SetNextState(BotState_WaitingCommand);
}

void Bot::Sleep(clock_t sec)
{
	clock_t start_time = clock();
	clock_t end_time = sec * 1000 + start_time;
	while (clock() != end_time);
}

void Bot::WaitCommand()
{
	BOT_LOG(mLogger, "WaitCommand...", false);

	char c = std::cin.peek();
	if (c == EOF)
	{
		return;
	}

	std::string cmd;
	std::getline(std::cin, cmd);

	BOT_LOG(mLogger, cmd, true);

	if (cmd == "[Init]")
	{
		SetNextState(BotState_Initializing);
	}
	else if (cmd == "[PlayTurn]")
	{
		SetNextState(BotState_PlayTurn);
	}
	else if (cmd == "[Exit]")
	{
		SetNextState(BotState_Exiting);
	}
	else
	{
		assert(false);
	}
}

void Bot::Exit()
{
	BOT_LOG(mLogger, "Exit", false);

	assert(mpBotLogic != nullptr && "mpBotLogic cannot be null at this point. Forgot to assign a botlogic in Main.cpp? ");

	//ExitInfo exitInfo;
	//std::cin >> exitInfo;

	mpBotLogic->Exit();

	BOT_LOG(mLogger, "BotLogic Exit", false);

	std::cout << "ExitProcessed" << std::endl;

	mbCompleted = true;
}

void Bot::SetNextState(EBotState _nextState)
{
	mNextState = _nextState;

#ifdef BOT_DEBUG
	std::stringstream ss;
	ss << "SetNextState(" << GetNextStateName(mNextState) << ")" << std::endl;

	BOT_LOG(mLogger, ss.str(), false);
#endif
}

const std::string& Bot::GetNextStateName(EBotState _botState)
{
	static std::string sBotState_None			= "None";
	static std::string sBotState_NotLoaded		= "NotLoaded";
	static std::string sBotState_Loading		= "Loading";
	static std::string sBotState_WaitingCommand = "WaitingCommand";
	static std::string sBotState_Initializing	= "Initializing";
	static std::string sBotState_PlayTurn		= "PlayTurn";
	static std::string sBotState_Exiting		= "Exiting";
	static std::string sError					= "INVALID";

	switch (_botState)
	{
		case BotState_None:
		{
			return sBotState_None;
		}
		break;
		
		case BotState_NotLoaded:
		{
			return sBotState_NotLoaded;
		}
		break;

		case BotState_Loading:
		{
			return sBotState_Loading;
		}
		break;

		case BotState_WaitingCommand:
		{
			return sBotState_WaitingCommand;
		}
		break;

		case BotState_Initializing:
		{
			return sBotState_Initializing;
		}
		break;

		case BotState_PlayTurn:
		{
			return sBotState_PlayTurn;
		}
		break;

		case BotState_Exiting:
		{
			return sBotState_Exiting;
		}
		break;

		default:
			break;
	}
	return sError;
}
