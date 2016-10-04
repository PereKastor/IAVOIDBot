#include "TurnInfo.h"

#include <string>
#include <sstream>
#include <fstream>

#include "Logger.h"

void TurnInfo::LogDebugInfo(Logger& _logger)
{
	_logger.Log(serializedData, true);
}

std::istream& operator>>(std::istream& _is, TurnInfo& _turnInfo)
{
	std::getline(_is, _turnInfo.serializedData);
	std::stringstream ss(_turnInfo.serializedData);

	std::string varName;
	while (!ss.eof())
	{
		ss >> varName;
		
		if (varName == "TurnNb")
		{
			ss >> _turnInfo.turnNb;
		}

		if (varName == "NPCCount")
		{
			int nbNPCs = 0;
			ss >> nbNPCs;

			for (int i = 0; i < nbNPCs; ++i)
			{
				NPCInfo npcInfo;
				
				ss >> npcInfo.npcID;
				ss >> npcInfo.currentHP;
				ss >> npcInfo.maxHP;
				ss >> npcInfo.tileID;

				int nbTileCnt = 0;
				ss >> nbTileCnt;

				int nbObjectCnt = 0;
				int tileID = 0;
				int nbTileAttributes = 0;

				int val = 0;

				TileInfo tileInfo;

				ss >> varName;
				if (varName == "VisibleTileCount")
				{
					ss >> nbTileCnt;
					for (int j = 0; j < nbTileCnt; ++j)
					{
						ss >> tileID;
						npcInfo.visibleTiles.insert((unsigned int)tileID);

						tileInfo.tileAttributes.clear();
						tileInfo.tileID = tileID;

						ss >> nbTileAttributes;
						for (int k = 0; k < nbTileAttributes; ++k)
						{
							ss >> val;
							tileInfo.tileAttributes.insert((ETileAttribute)val);
						}

						_turnInfo.tiles[tileID] = tileInfo;
					}
				}

				ss >> varName;
				if (varName == "VisibleObjectCount")
				{
					ss >> nbObjectCnt;
					for (int j = 0; j < nbObjectCnt; ++j)
					{
						ss >> val;
						npcInfo.visibleObjects.insert((unsigned int)val);
					}
				}				

				_turnInfo.npcs[npcInfo.npcID] = npcInfo;
			}
			varName = "";
		}

		if (varName == "VisibleObjectCount")
		{
			int nbObjects = 0;
			ss >> nbObjects;

			std::string objectType;

			for (int i = 0; i < nbObjects; ++i)
			{
				ObjectInfo objectInfo;

				ss >> objectInfo.objectID;

				ss >> objectInfo.tileID;

				ss >> objectType;
				objectInfo.objectType = EObjectType::HighWall;

				for (int j = 0; j < 8; ++j)
				{
					ss >> objectInfo.edgesCost[j];
				}

				_turnInfo.objects[objectInfo.objectID] = objectInfo;
			}
		}
	}

	return _is;
}

