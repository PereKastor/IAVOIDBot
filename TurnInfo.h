#pragma once

#include <map>
#include <set>
#include <vector>

#include <iostream>
#include <string>

#include "Globals.h"

#ifdef DEBUG_FRAMEWORK
   #define TURNINFO_DEBUG
#endif

class Logger;

struct ObjectInfo
{
	unsigned int tileID;
	unsigned int objectID;
	EObjectType objectType;
	unsigned int edgesCost[8] = { 0 }; // Si c'est a 0, on ne peut pas traverser
};

struct TileInfo
{
	unsigned int tileID;
	std::set<ETileAttribute> tileAttributes;
};

struct NPCInfo
{
	unsigned int npcID = 0;
	float currentHP = 0.0f;
	float maxHP = 0.0f;
	unsigned int tileID = 0;
	std::set<unsigned int> visibleTiles;
	std::set<unsigned int> visibleObjects;
};

struct TurnInfo
{
	unsigned int turnNb;
	std::map<unsigned int, ObjectInfo> objects;
	std::map<unsigned int, NPCInfo> npcs;
	std::map<unsigned int, TileInfo> tiles;

	std::string serializedData;

	void LogDebugInfo(Logger& _logger);
};

std::istream& operator>>(std::istream& _is, TurnInfo& _turnInfo);
