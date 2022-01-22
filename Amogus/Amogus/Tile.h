#pragma once

enum class TileObject
{
	NONE = 0,
	HOUSE = 1,
	TREE = 2,
};

class Tile
{
public:

	Tile()
		: Tile(TileObject::NONE)
	{

	}

	Tile(TileObject object)
		: m_object(object)
	{

	}

	bool IsPassable()
	{
		return (m_object == TileObject::NONE);
	}

	TileObject m_object;
private:
};