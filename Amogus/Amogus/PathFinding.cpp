#include "PathFinding.h"

Path::Path()
{
	pathNodes = std::vector<pathNode*>();
}

bool Path::AddNode(pathNode* node)
{

	if (std::find(pathNodes.begin(), pathNodes.end(), node) != pathNodes.end())
	{
		pathNodes.push_back(node);
		return true;
	}
	return false;
}

pathNode* Path::GetLast()
{
	if (pathNodes.size() > 0)
	{
		return pathNodes[pathNodes.size() - 1];
	}
	return nullptr;
}

std::string* Path::GetNextPoint(int myNode)
{
	if (pathNodes.size() > 1)
	{
		// return pathNodes[myNode]->nodeData;
	}
	return nullptr;
}


void pathNode::OnEnable()
{
	pathFinding::onResetGrid += ResetNode;
}

void pathNode::OnDisable()
{
	pathFinding::onResetGrid -= ResetNode;
}

void pathNode::ResetNode()
{
	m_costToStart = std::numeric_limits<int>::max();
	m_explored = false;
}

std::vector<pathNode*> pathNode::UpdateNeighbours()
{
	std::vector<pathNode*> updatedNeighbours;
	for (auto n : m_Neighbours)
	{
		if (!n->m_explored && n->m_isValidPath)
		{
			if (n->UpdateNode(m_costToStart + 1))
			{
				updatedNeighbours.push_back(n);
			}

		}

	}
	return updatedNeighbours;
}

bool pathNode::UpdateNode(int cost)
{
	if (m_costToStart > cost)
	{
		m_costToStart = cost;
		return true;
	}
	return false;
}

pathNode* pathNode::GetNextInPath()
{
	if (m_Neighbours.size() > 0)
	{
		pathNode* currentNext = m_Neighbours[0];

		for (int i = 1; i < m_Neighbours.size(); ++i)
		{
			if (m_Neighbours[i]->m_costToStart < currentNext->m_costToStart)
			{
				currentNext = m_Neighbours[i];
			}
		}
		return currentNext;
	}
	return nullptr;
}
Path* pathFinding::GetPath(pathNode* start, pathNode* end)
{
	if (onResetGrid != nullptr)
	{
		//std::invoke(onResetGrid());

	}

	//loop through all nodes and set distance from end to start
	end->UpdateNode(0);
	end->m_explored = true;
	bool isFinished = false, isPossible = true;
	std::vector<pathNode*> _Front;
	_Front.AddRange(end->UpdateNeighbours());

	while (!isFinished)
	{
		std::vector<pathNode*> _NextFront;
		for (auto n : _Front)
		{
			if (n == start)
			{
				isFinished = true;
				break;
			}
			n->m_explored = true;
			_NextFront.AddRange(n->UpdateNeighbours());
		}
		if (_NextFront.empty() && !isFinished) // if we haven't finished and there is nothing left to check then its an infinite loop
		{
			isFinished = true;
			isPossible = false;
		}
		else
		{
			_Front = _NextFront;
		}
	}

	Path* p = new Path();
	if (isPossible)
	{
		isFinished = false;
		//trace path from start to end
		p->AddNode(start);
		while (!isFinished)
		{
			pathNode* temp = p->GetLast().GetNextInPath();
			p->AddNode(temp);
			if (temp == end)
			{
				isFinished = true;
			}
		}
	}

	return p;
}
