// Refactor fo style standards

#include "InputHandler.h"
#include <Core/source.h>
#include <nlohmann/include/nlohmann/json.hpp>
#include <fstream>

double InputHandler::m_mouseX = 0.0f;
double InputHandler::m_mouseY = 0.0f;
glm::vec2 InputHandler::m_gamepadLeftStickAxis = glm::vec2(0.0f, 0.0f);
glm::vec2 InputHandler::m_gamepadRightStickAxis = glm::vec2(0.0f, 0.0f);;
float InputHandler::m_gamepadLeftTriggerAxis = 1.0f;
float InputHandler::m_gamepadRightTriggerAxis = 1.0f;
bool InputHandler::m_initialized = false;
EventBus* InputHandler::m_mouseClickBus = new EventBus();
std::vector<InputMapping*> InputHandler::m_mappings = std::vector<InputMapping*>();

void InputHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	// JIT initialization
	if (!m_initialized)
		ReadMappings();

	for (InputMapping* mapping : m_mappings)
	{
		for (int _key : mapping->m_keys)
		{
			if (_key == key)
			{
				InputEvent* e = new InputEvent();
				e->m_keyInput = key;
				e->m_scancode = scancode;
				e->m_action = action;
				e->m_mods = mods;

				mapping->m_bus->publish(e);
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
	InputEvent* e = new InputEvent();
	e->m_mouseInput = button;
	e->m_action = action;
	e->m_mods = mods;

	m_mouseClickBus->publish(e);
}

void InputHandler::GamepadButtonPressed(int button)
{
	// JIT initialization
	if (!m_initialized)
		ReadMappings();

	for (InputMapping* mapping : m_mappings)
	{
		for (int _button : mapping->m_buttons)
		{
			if (_button == button)
			{
				InputEvent* e = new InputEvent();
				e->m_gamepadInput = button;

				mapping->m_bus->publish(e);
			}
		}
	}
}

void InputHandler::GamepadAxisChanged(int axis, float value)
{
	if (axis == GLFW_GAMEPAD_AXIS_LEFT_X)
		m_gamepadLeftStickAxis.x = value;

	if (axis == GLFW_GAMEPAD_AXIS_LEFT_Y)
		m_gamepadLeftStickAxis.y = value;

	if (axis == GLFW_GAMEPAD_AXIS_RIGHT_X)
		m_gamepadRightStickAxis.x = value;

	if (axis == GLFW_GAMEPAD_AXIS_RIGHT_Y)
		m_gamepadRightStickAxis.y = value;

	if (axis == GLFW_GAMEPAD_AXIS_LEFT_TRIGGER)
		m_gamepadLeftTriggerAxis = value;

	if (axis == GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER)
		m_gamepadRightTriggerAxis = value;
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
	m_mappings.reserve(mappings.size());
	for (int i = 0; i < mappings.size(); i++)
	{
		std::string mappingName = mappings[i]["name"];

		// Keys
		std::vector<int> keys;
		nlohmann::json keyData = mappings[i]["keys"];
		keys.reserve(keyData.size());

		for (int j = 0; j < keyData.size(); j++)
		{
			keys.emplace_back(keyData[j]);
		}

		// Gamepad Buttons
		std::vector<int> buttons;
		nlohmann::json buttonData = mappings[i]["buttons"];
		buttons.reserve(buttonData.size());

		for (int j = 0; j < buttonData.size(); j++)
		{
			buttons.emplace_back(buttonData[j]);
		}

		m_mappings.emplace_back(new InputMapping(mappingName, keys, buttons));
	}

	m_initialized = true;
}

void InputHandler::Cleanup()
{
	WriteMappings();

	for (InputMapping* mapping : m_mappings)
		delete mapping;

	delete m_mouseClickBus;
}

void InputHandler::WriteMappings()
{
	nlohmann::json outJson;
	outJson["Mappings"] = nlohmann::json::array();
	for (InputMapping* mapping : m_mappings)
	{
		auto obj = nlohmann::json::object();
		obj["name"] = mapping->m_name;

		obj["keys"] = nlohmann::json::array();
		for (int e : mapping->m_keys)
			obj["keys"].push_back(e);

		obj["buttons"] = nlohmann::json::array();
		for (int e : mapping->m_buttons)
			obj["buttons"].push_back(e);

		outJson["Mappings"].push_back(obj);
	}
}

InputMapping* InputHandler::GetMapping(std::string name)
{
	// JIT initialization
	if (!m_initialized)
		ReadMappings();

	for (InputMapping* mapping : m_mappings)
	{
		if (mapping->m_name == name)
			return mapping;
	}

	return nullptr;
}

void InputHandler::PollGameControllers()
{
	int controllerPresent = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (controllerPresent == 1)
	{
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		static std::vector<float> axesStates;
		glm::vec2 leftStickAxes = glm::vec2(axes[0], axes[1]);
		glm::vec2 rightStickAxes = glm::vec2(axes[2], axes[3]);
		if (axesStates.size() == 0)
		{
			for (int i = 0; i < axesCount; i++)
				axesStates.push_back(0.0f);

			for (int i = 0; i < axesCount; i++)
				axesStates[i] = axes[i];
		}

		for (int i = 0; i < axesCount; i++)
		{
			if (axes[i] != axesStates[i])
			{
				GamepadAxisChanged(i, axes[i]);
				axesStates[i] = axes[i];
			}
		}


		int buttonCount;
		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		for (int i = 0; i < buttonCount; i++)
		{
			if (GLFW_PRESS == buttons[i])
			{
				GamepadButtonPressed(i);
			}
		}
	}
}
