// Refactor fo style standards

#include "InputHandler.h"
#include "source.h"
#include "nlohmann/json.hpp"
#include <fstream>

double InputHandler::m_mouseX = 0.0f;
double InputHandler::m_mouseY = 0.0f;
bool InputHandler::m_initialized = false;
EventBus* InputHandler::m_mouseClickBus = new EventBus();
std::vector<InputMapping*> InputHandler::m_keyMappings = std::vector<InputMapping*>();

void InputHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	// JIT initialization
	if (!m_initialized)
		ReadMappings();

	for (InputMapping* mapping : m_keyMappings)
	{
		for (int _key : mapping->m_enums)
		{
			if (_key == key)
			{
				mapping->m_bus->publish(new KeyInputEvent(key, scancode, action, mods));
			}
		}
	}
}

void InputHandler::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	m_mouseX = xpos;
	m_mouseY = ypos;
}

void InputHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	m_mouseClickBus->publish(new MouseClickInputEvent(button, action, mods));
}

void InputHandler::ReadMappings()
{
	// AP.Intellisense absolutely hates this library for some reason
	// Probably because JSON is the spawn of satan himself
	// Still better than XML tho
	// Blame Bill Gates

	std::fstream file("Data/Config/KEYMAP.json");
	nlohmann::json mapData;
	file >> mapData;
	file.close();

	nlohmann::json mappings = mapData["Mappings"];
	m_keyMappings.reserve(mappings.size());
	for (int i = 0; i < mappings.size(); i++)
	{
		std::string mappingName = mappings[i]["name"];
		std::vector<int> enums;
		nlohmann::json enumData = mappings[i]["enums"];
		enums.reserve(enumData.size());

		for (int j = 0; j < enumData.size(); j++)
		{
			enums.emplace_back(enumData[j]);
		}

		m_keyMappings.emplace_back(new InputMapping(mappingName, enums));
	}

	m_initialized = true;
}

void InputHandler::Cleanup()
{
	WriteMappings();

	for (InputMapping* mapping : m_keyMappings)
		delete mapping;

	delete m_mouseClickBus;
}

void InputHandler::WriteMappings()
{
	nlohmann::json outJson;
	outJson["Mappings"] = nlohmann::json::array();
	for (InputMapping* mapping : m_keyMappings)
	{
		auto obj = nlohmann::json::object();
		obj["name"] = mapping->m_name;

		obj["enums"] = nlohmann::json::array();
		for (int e : mapping->m_enums)
			obj["enums"].push_back(e);

		outJson["Mappings"].push_back(obj);
	}
}

InputMapping* InputHandler::GetMapping(std::string name)
{
	// JIT initialization
	if (!m_initialized)
		ReadMappings();

	for (InputMapping* mapping : m_keyMappings)
	{
		if (mapping->m_name == name)
			return mapping;
	}

	return nullptr;
}
