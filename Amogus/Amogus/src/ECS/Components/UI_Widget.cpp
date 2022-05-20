#include "UI_Widget.h"

UI_WidgetComponent::UI_WidgetComponent()
{
}

UI_WidgetComponent::~UI_WidgetComponent()
{
	for (UI_BaseElement* element : m_elements)
		delete element;
}

UI_BaseElement::UI_BaseElement()
	: m_absolutePosition(0, 0),
	m_absoluteSize(200, 200),
	m_relativePosition(0, 0),
	m_relativeSize(0, 0),
	m_hidden(false),
	m_zIndex(0),
	m_colour(1, 1, 1),
	m_elementType(ET_Base),
	m_name(""),
	m_widget(nullptr)
{
}

UI_BaseElement::~UI_BaseElement()
{
}

UI_Image::UI_Image()
	: m_texture(TextureLoader::CreateTexture2DFromFile("Texture", "TestImages/hi.png"))
{
	m_elementType = ET_Image;
}

UI_Image::~UI_Image()
{
}

UI_Text::UI_Text()
	: m_centered(true)
{
	m_absoluteSize= glm::vec2(1, 1);
	m_relativePosition = glm::vec2(0.5, 0.5);
	m_elementType = ET_Text;
	m_text = "abcdefghijklmopqrstuvwxyz";
}

UI_Text::~UI_Text()
{
}

UI_ImageButton::UI_ImageButton()
	: m_state(BS_None),
	m_clickBus(new EventBus())
{
	m_elementType = ET_ImageButton;
}

UI_ImageButton::~UI_ImageButton()
{
	delete m_clickBus;
}