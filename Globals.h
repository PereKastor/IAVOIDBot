#pragma once

enum EDirection { N, NE, E, SE, S, SW, W, NW };
enum EAction { Action_None, Action_Move, Action_Interact};

enum EObjectType
{
	HighWall
};

enum ETileAttribute
{
	TileAttribute_Forbidden,
	TileAttribute_Target,
	TileAttribute_Zone,
	TileAttribute_Obstacle,
	TileAttribute_Descriptor
};

struct Action
{
public:
	
	Action(unsigned int _unitID, EAction _actionType)
		: unitID(_unitID)
		, actionType(_actionType)
	{

	}

	virtual Action* Clone() = 0;

	unsigned int unitID;
	EAction actionType;

private:
};

struct Move : Action
{
	Move(unsigned int _unitID, EDirection _direction)
		: Action (_unitID, EAction::Action_Move)
		, direction(_direction)
	{

	}

	Move(const Move& _other)
		: Action (_other.unitID, _other.actionType)
		, direction(_other.direction)
	{

	}

	virtual Action* Clone()
	{
		return new Move(*this);
	}
	
	EDirection direction;
};

struct Interact : Action
{
	Interact(unsigned int _unitID)
		: Action(_unitID, EAction::Action_Interact)
	{

	}

	Interact(const Interact& _other)
		: Action(_other.unitID, _other.actionType)
	{

	}

	virtual Action* Clone()
	{
		return new Interact(*this);
	}
};
