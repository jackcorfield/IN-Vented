#pragma once
#include <list>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>

class pathNode
{
public:
	std::vector<pathNode*> m_Neighbours;
	int m_costToStart = std::numeric_limits<int>::max();
	bool m_explored = false;
	bool m_isValidPath = true;

private:

	void OnEnable();

	void OnDisable();

public:
	void ResetNode();

	std::vector<pathNode*> UpdateNeighbours();

	bool UpdateNode(int cost);

	pathNode* GetNextInPath();

};

class Path
{
public:
	std::vector<pathNode*> pathNodes;

	Path();

	bool AddNode(pathNode* node);

	pathNode* GetLast();

	std::string* GetNextPoint(int myNode); //replace with correct data type
};

class pathFinding
{

public:
	static Path* GetPath(pathNode* start, pathNode* end);
	static void onResetGrid();

};
