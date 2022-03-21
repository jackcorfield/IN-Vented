#pragma once

#include <string>

class EntityName
{
public:
	EntityName(const std::string& name)
		: m_name(name) {}

	std::string m_name;
};