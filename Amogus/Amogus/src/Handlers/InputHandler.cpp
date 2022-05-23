// Refactor fo style standards

#include "InputHandler.h"
#include <Core/source.h>
#include <nlohmann/include/nlohmann/json.hpp>
#include <fstream>

extern Application* g_app;

double InputHandler::m_mouseX = 0.0f;
double InputHandler::m_mouseY = 0.0f;
glm::vec2 InputHandler::m_gamepadLeftStickAxis = glm::vec2(0.0f, 0.0f);
glm::vec2 InputHandler::m_gamepadRightStickAxis = glm::vec2(0.0f, 0.0f);;
float InputHandler::m_gamepadLeftTriggerAxis = 1.0f;
float InputHandler::m_gamepadRightTriggerAxis = 1.0f;
bool InputHandler::m_initialized = false;
EventBus* InputHandler::m_mouseClickBus = new EventBus();
std::vector<InputMapping*> InputHandler::m_mappings = std::vector<InputMapping*>();
std::vector<ClickedButton> InputHandler::m_clickedButtons = std::vector<ClickedButton>();

bool AABB(float aX, float aY, float aW, float aH, float bX, float bY, float bW, float bH)
{
	if (aX < bX + bW &&
		aX + aW > bX &&
		aY < bY + bH &&
		aY + aH > bY)
		return true;

	return false;
}

void InputHandler::TransformToFitScreen(glm::vec3& pos, glm::vec3& size)
{
	glm::vec4 screenInfo = g_app->GetGameScreenInfo();
	glm::vec3 tempPos = pos;
	glm::vec3 tempSize = size;

	tempPos.x -= screenInfo.x;
	float x_scalar = screenInfo.z / g_app->m_windowParams.windowWidth;
	tempPos.x *= x_scalar;
	tempSize.x *= x_scalar;

	tempPos.y -= screenInfo.y;
	float y_scalar = screenInfo.w / g_app->m_windowParams.windowHeight;
	tempPos.y *= y_scalar;
	tempSize.y *= y_scalar;

	pos = tempPos;
	size = tempSize;
}

bool InputHandler::ButtonIsSelected(UI_ImageButton* button)
{
	glm::vec2 adjustedRelativePos = button->m_relativePosition * glm::vec2((float)g_app->GetGameScreenInfo().z, (float)g_app->m_windowParams.windowHeight);
	glm::vec3 finalPos = glm::vec3(adjustedRelativePos + button->m_absolutePosition, button->m_zIndex);
	glm::vec2 adjustedRelativeSize = button->m_relativeSize * glm::vec2((float)g_app->m_windowParams.windowWidth, (float)g_app->m_windowParams.windowHeight);
	glm::vec3 finalSize = glm::vec3(adjustedRelativeSize + button->m_absoluteSize, 1);

	if (g_app->m_windowParams.windowWidth != g_app->GetGameScreenInfo().z)
	{
		TransformToFitScreen(finalPos, finalSize);
	}

	// some values just for more readable code
	float mouseW = 2;
	float mouseH = 2;

	std::cout << "x: " << m_mouseX << " y: " << m_mouseY << "\n";


	if (AABB(m_mouseX, m_mouseY, mouseW, mouseH, finalPos.x - (finalSize.x / 2), finalPos.y - (finalSize.y / 2), finalSize.x, finalSize.y))
		return true;

	return false;
}

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
	glm::vec4 screenInfo = g_app->GetGameScreenInfo();

	// We only care if we're in the screen
	if (!AABB(xpos, ypos, 2, 2, screenInfo.x, screenInfo.y, screenInfo.z, screenInfo.w))
		return;

	if (g_app->m_windowParams.windowWidth != screenInfo.z)
	{
		// Transform the mouse position to be within the editor screen
		m_mouseX = xpos;
		m_mouseX -= screenInfo.x;
		float x_scalar = screenInfo.z / g_app->m_windowParams.windowWidth;
		m_mouseX *= x_scalar;

		m_mouseY = ypos;
		m_mouseY -= screenInfo.y;
		float y_scalar = screenInfo.w / g_app->m_windowParams.windowHeight;
		m_mouseY *= y_scalar;
	}

	//std::string msg = std::to_string(m_mouseX) + std::string(", ") + std::to_string(m_mouseY);
	//g_app->m_debugger->Log(msg, LL_DEBUG);

	Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
	if (activeScene)
	{
		std::vector<UI_WidgetComponent*> widgets = activeScene->m_entityManager->GetAllComponentsOfType<UI_WidgetComponent>();
		if (widgets.size() != 0)
		{
			for (UI_WidgetComponent* widget : widgets)
			{
				for (int i = 0; i < widget->m_elements.size(); i++)
				{
					if (widget->m_elements[i]->m_elementType != ET_ImageButton) continue;
					UI_ImageButton* button = (UI_ImageButton*)widget->m_elements[i];

					if (ButtonIsSelected(button))
					{
						if (button->m_state == BS_None)
						{
							button->m_state = BS_Hover;
						}
					}
					else if (button->m_state == BS_Hover)
					{
						button->m_state = BS_None;
					}
				}
			}
		}
	}
}

void InputHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
	if (activeScene)
	{
		std::vector<UI_WidgetComponent*> widgets = activeScene->m_entityManager->GetAllComponentsOfType<UI_WidgetComponent>();
		if (widgets.size() != 0)
		{
			for (UI_WidgetComponent* widget : widgets)
			{
				for (int i = 0; i < widget->m_elements.size(); i++)
				{
					if (widget->m_elements[i]->m_elementType != ET_ImageButton) continue;
					UI_ImageButton* button = (UI_ImageButton*)widget->m_elements[i];

					if (button->m_state == BS_Hover)
					{
						button->m_state = BS_Click;
						g_app->m_debugger->Log("Clicked the button lol", LL_DEBUG);
						button->m_clickBus->publish(new Event());
						m_clickedButtons.push_back(ClickedButton(button));
					}
				}
			}
		}
	}

	InputEvent* e = new InputEvent();
	e->m_mouseInput = button;
	e->m_action = action;
	e->m_mods = mods;

	m_mouseClickBus->publish(e);
}

void InputHandler::GamepadButtonPressed(int button, const unsigned char action)
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
				e->m_action = action;

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


	for (InputMapping* mapping : m_mappings)
	{
		if (mapping->m_name != "Input_Movement")
			continue;

		InputEvent* e = new InputEvent();

		e->m_leftStickAxis = m_gamepadLeftStickAxis;
		e->m_rightStickAxis = m_gamepadRightStickAxis;

		mapping->m_bus->publish(e);
	}
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
				GamepadButtonPressed(i, buttons[i]);
			}
		}
	}
}

void InputHandler::Update(float dt)
{
	for (int i = 0; i < m_clickedButtons.size(); i++)
	{
		m_clickedButtons[i].timeSinceClick += dt;
		if (m_clickedButtons[i].timeSinceClick > 0.2f)
		{
			if (ButtonIsSelected(m_clickedButtons[i].button))
			{
				m_clickedButtons[i].button->m_state = BS_Hover;
			}
			else
			{
				m_clickedButtons[i].button->m_state = BS_None;
			}
			m_clickedButtons.erase(m_clickedButtons.begin() + i);
		}
	}
}
