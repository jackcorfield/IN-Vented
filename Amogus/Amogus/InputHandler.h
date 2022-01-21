#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "EventHandler.h"

class MouseClickInputEvent : public Event
{
public:
	MouseClickInputEvent(int button, int action, int mods) : m_button(button), m_action(action), m_mods(mods)
	{

	}
	int m_button;
	int m_action;
	int m_mods;
};

class KeyInputEvent : public Event
{
public:
	KeyInputEvent(int key, int scancode, int action, int mods) : m_key(key), m_scancode(scancode), m_action(action), m_mods(mods)
	{

	}
	int m_key;
	int m_scancode;
	int m_action;
	int m_mods;
};

class InputMapping
{
public:
	InputMapping(std::string name, std::vector<int> enums) : m_name(name), m_enums(enums)
	{
		m_bus = new EventBus();
	}

	~InputMapping()
	{
		delete m_bus;
	}

	std::string m_name;
	std::vector<int> m_enums;
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

	static void ReadMappings();
	static void Cleanup();
	static void WriteMappings();
	static InputMapping* GetMapping(std::string name);

	static double GetMousePosition(double& x, double& y)
	{
		x = m_mouseX;
		y = m_mouseY;
	}

	static EventBus* m_mouseClickBus;

private:
	static std::vector<InputMapping*> m_keyMappings;

	static double m_mouseX;
	static double m_mouseY;

	static bool m_initialized;

};
