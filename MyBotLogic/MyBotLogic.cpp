#include "MyBotLogic.h"
#include <algorithm>
#include "TurnInfo.h"
#include "LevelInfo.h"
#include "build-solution-x64/SearchMap.h"
#include <windows.h>
#include "build-solution-x64/Map.h"

MyBotLogic::MyBotLogic()
{
    //Write Code Here
}

/*virtual*/ MyBotLogic::~MyBotLogic()
{
    //Write Code Here
}

/*virtual*/ void MyBotLogic::Configure(int argc, char *argv[], const std::string& _logpath)
{
#ifdef BOT_LOGIC_DEBUG
    mLogger.Init(_logpath, "MyBotLogic.log");
#endif

    BOT_LOGIC_LOG(mLogger, "Configure", true);


    //Write Code Here
}

/*virtual*/ void MyBotLogic::Load()
{
    //Write Code Here
    //Sleep(9000);
}

/*virtual*/ void MyBotLogic::Init(LevelInfo& _levelInfo)
{
    unsigned int rowCount = _levelInfo.rowCount;
    unsigned int colCount = _levelInfo.colCount;
    MyMap = Map(colCount, rowCount);
    unsigned int countIndex = 0;
    for(int i = 0; i < rowCount; ++i)
    {
        for(int j = 0; j < colCount; ++j)
        {
            // TODO - changer normal en NONE et mettre en place un type PATH
            MyMap.createNode(new Node{j, i, countIndex, Node::Normal});
            countIndex++;
        }
    }
}

/*virtual*/ void MyBotLogic::OnBotInitialized()
{
    //Write Code Here
}

/*virtual*/ void MyBotLogic::OnGameStarted()
{
    //Write Code Here
}

/*virtual*/ void MyBotLogic::FillActionList(TurnInfo& _turnInfo, std::vector<Action*>& _actionList)
{
    for each(auto info in _turnInfo.tiles)
    {
        auto tileInfo = info.second;

        auto ITisForbidden = tileInfo.tileAttributes.find(TileAttribute_Forbidden);
        auto ITisTarget = tileInfo.tileAttributes.find(TileAttribute_Target);
        if(ITisForbidden != tileInfo.tileAttributes.end())
        {
            MyMap.setNodeType(tileInfo.tileID, Node::Forbidden);
        }
        else if(ITisTarget != tileInfo.tileAttributes.end())
        {
            MyMap.setNodeType(tileInfo.tileID, Node::Goal);
            MyMap.addGoalTile(tileInfo.tileID);
        }
    }

    for each(auto info in _turnInfo.npcs)
    {
        // TODO - check between all npcs to find wich one is the best to go on the goal tile
        int goalTile = MyMap.getBestGoalTile(info.second.tileID);
        auto npcSMap = new SearchMap(&MyMap, MyMap.getNode(info.second.tileID), MyMap.getNode(goalTile));
        MyMap.addSearchMap(info.second.npcID, npcSMap);
        while(!npcSMap->isFinished())
        {
            npcSMap->search();
        }
    }

    for each(auto info in _turnInfo.npcs)
    {
        // Get my search map
        SearchMap* npcMap = MyMap.getSearchMap(info.second.npcID);

        // If my npc path is not correct anymore, we try to find another path
        if(!npcMap->checkPathIntegrity())
        {
            while(!npcMap->isFinished())
            {
                npcMap->search();
            }
        }

        // Get next npc tile
        int nextNpcTile = npcMap->getNextPathTile();

        if(nextNpcTile > 0)
        {
            // Check if we can move on expected tile
            bool canMoveOnThisTile = true;
            for(int i = 0; i < m_nextNpcMove.size(); i++)
            {
                if(m_nextNpcMove[i] == nextNpcTile)
                {
                    // Don't move on the same tile
                    canMoveOnThisTile = false;
                }
            }

            if(canMoveOnThisTile)
            {
                // Add the move in provate array to block other npc to go on this case
                m_nextNpcMove.push_back(nextNpcTile);

                // Add the action in the list
                _actionList.push_back(new Move(info.second.npcID, MyMap.getNextDirection(info.second.npcID, info.second.tileID)));

                // Check the next npc move
                if(npcMap->getNextPathTile() < 0)
                {
                    // If it's -1, this NPC finished his path
                    MyMap.setNodeType(nextNpcTile, Node::Forbidden);
                }
            }
        }
    }
    m_nextNpcMove.clear();
}

/*virtual*/ void MyBotLogic::Exit()
{
    //Write Code Here
}