#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

#include "EventHandler.h"

// AP. Mapped out by hand and trial and error
enum XBOX_CONTROLS
{
	XB1_A = 0,
	XB1_B = 1,
	XB1_X = 2,
	XB1_Y = 3,
	XB1_LEFT_BUMPER = 4,
	XB1_RIGHT_BUMPER = 5,
	XB1_SELECT = 6,
	XB1_START = 7,
	XB1_LEFT_STICK_PRESS = 8,
	XB1_RIGHT_STICK_PRESS = 9,
	XB1_DPAD_UP = 10,
	XB1_DPAD_RIGHT = 11,
	XB1_DPAD_LEFT = 13,
	XB1_DPAD_DOWN = 12
};

class InputEvent : public Event
{
public:
	int m_mouseInput = -1;
	int m_keyInput = -1;
	int m_gamepadInput = -1;

	int m_action = -1;
	int m_mods = -1;
	int m_scancode = -1;

	glm::vec2 m_leftStickAxis = glm::vec2(-1, -1);
	glm::vec2 m_rightStickAxis = glm::vec2(-1, -1);
	float m_leftTriggerAxis = -1;
	float m_rightTriggerAxis = -1;
};

class InputMapping
{
public:
	InputMapping(std::string name, std::vector<int> keys, std::vector<int> buttons) : m_name(name), m_keys(keys), m_buttons(buttons)
	{
		m_bus = new EventBus();
	}

	~InputMapping()
	{
		delete m_bus;
	}

	std::string m_name;
	std::vector<int> m_keys;
	std::vector<int> m_buttons;
	EventBus* m_bus;
};

#include <string>

// Refactor fo style standards

struct GLFWindow;
class InputMapping;

class InputHandler
{
public:
	// The GLFW callback for key events.  Sends events to all KeyInput instances
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int key, int action, int mods);

	static void GamepadButtonPressed(int button);
	static void GamepadAxisChanged(int axis, float value);

	static void ReadMappings();
	static void Cleanup();
	static void WriteMappings();
	static InputMapping* GetMapping(std::string name);
	static void PollGameControllers();

	static double GetMousePosition(double& x, double& y)
	{
		x = m_mouseX;
		y = m_mouseY;
	}

	static EventBus* m_mouseClickBus;

private:
	static std::vector<InputMapping*> m_mappings;

	static double m_mouseX;
	static double m_mouseY;
	static glm::vec2 m_gamepadLeftStickAxis;
	static glm::vec2 m_gamepadRightStickAxis;
	static float m_gamepadLeftTriggerAxis;
	static float m_gamepadRightTriggerAxis;

	static bool m_initialized;

};
