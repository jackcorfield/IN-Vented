#pragma once
#include <Resources/Texture2D.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../../Handlers/EventHandler.h"

/*
TODO COMPONENTS:
- 

- Image
- ImageButton
- Text                                                                                                                                                                                        
*/

class UI_WidgetComponent;

enum ElementType
{
	ET_Base,
	ET_Image,
	ET_ImageButton,
	ET_Text,
	NumElementTypes
};

static const std::vector<std::string> s_ElementType
{
	"BaseElement",
	"ImageElement",
	"ImageButtonElement",
	"TextElement",
};

class UI_BaseElement
{
public:
	UI_BaseElement();
	~UI_BaseElement();

	UI_WidgetComponent* m_widget;

	int m_elementType;
	glm::vec2 m_absolutePosition;
	glm::vec2 m_relativePosition;
	glm::vec2 m_absoluteSize;
	glm::vec2 m_relativeSize;
	bool m_hidden;
	int m_zIndex;
	glm::vec3 m_colour;
	std::string m_name;
};

class UI_Image : public UI_BaseElement
{
public:
	UI_Image();
	~UI_Image();

	Texture2D m_texture;
};

enum ButtonState
{
	BS_None,
	BS_Hover,
	BS_Click
};

class UI_ImageButton : public UI_Image
{
public:
	UI_ImageButton();
	~UI_ImageButton();

	Texture2D m_hoveredTexture;
	Texture2D m_clickedTexture;
	EventBus* m_clickBus;
	int m_state;
};

class UI_Text : public UI_BaseElement
{
public:
	std::string m_text;
	bool m_centered;

	UI_Text();
	~UI_Text();
};

class UI_WidgetComponent
{
public:
	UI_WidgetComponent();
	~UI_WidgetComponent();

	UI_BaseElement* AddElement(int type);
	void RemoveElement(std::string name);

	std::vector<UI_BaseElement*> m_elements;

	UI_BaseElement* GetElement(std::string name)
	{
		for (UI_BaseElement* element : m_elements)
		{
			if (element->m_name == name)
				return element;
		}
		return nullptr;
	}

	UI_ImageButton* GetButton(std::string name)
	{
		UI_BaseElement* e = GetElement(name);
		if (e && e->m_elementType == ET_ImageButton)
		{
			return (UI_ImageButton*)e;
		}

		return nullptr;
	}

	UI_Text* GetText(std::string name)
	{
		UI_BaseElement* e = GetElement(name);
		if (e && e->m_elementType == ET_Text)
		{
			return (UI_Text*)e;
		}

		return nullptr;
	}
};