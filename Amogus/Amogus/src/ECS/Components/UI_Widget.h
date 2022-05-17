#pragma once
#include <Resources/Texture2D.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

/*
TODO COMPONENTS:
- Frame
- Image
- ImageButton
- Text                                                                                                                                                                                        
*/

enum ElementType
{
	ET_Base,
	ET_Frame,
	ET_Image,
	ET_ImageButton,
	ET_Text,
	NumElementTypes
};

static const std::vector<std::string> s_ElementType
{
	"Base Element",
	"Frame Element",
	"Image Element",
	"Image Button Element",
	"Text Element",
};

class UI_BaseElement
{
public:
	UI_BaseElement();
	~UI_BaseElement();

	int m_elementType;
	glm::vec2 m_absolutePosition;
	glm::vec2 m_relativePosition;
	glm::vec2 m_absoluteSize;
	glm::vec2 m_relativeSize;
	bool m_hidden;
	int m_zIndex;
	glm::vec3 m_backgroundColour;
	std::string m_name;
};

class UI_Frame : public UI_BaseElement
{
public:
	UI_Frame();
	~UI_Frame();
};

class UI_Image : public UI_BaseElement
{
public:
	UI_Image();
	~UI_Image();

	Texture2D m_texture;
};

class UI_ImageButton : public UI_Image
{
public:
	UI_ImageButton();
	~UI_ImageButton();

	Texture2D m_texture;
};

class UI_Text : public UI_BaseElement
{
public:
	std::string m_text;

	UI_Text();
	~UI_Text();
};

class UI_WidgetComponent
{
public:
	UI_WidgetComponent();
	~UI_WidgetComponent();

	std::vector<UI_BaseElement*> m_elements;
};

/*
class UI_Image
{
public:
	UI_Image() : m_texture(TextureLoader::CreateTexture2DFromFile("UI_test", "hi.png")), m_position(0, 0, 0, 0), m_size(0, 100, 0, 100), m_zIndex(1) {}
	UI_Image(const Texture2D& texture, glm::vec4 pos, glm::vec4 size, int zIdx) : m_texture(texture), m_position(pos), m_size(size), m_zIndex(zIdx) {}

	Texture2D m_texture;
	int m_zIndex;
	// FORMAT: RELATIVE X, ABSOLUTE X, RELATIVE Y, ABSOLUTE Y
	glm::vec4 m_position;
	glm::vec4 m_size;
};
*/