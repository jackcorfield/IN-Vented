#include "UI_Widget.h"

UI_WidgetComponent::UI_WidgetComponent()
{
}

UI_WidgetComponent::~UI_WidgetComponent()
{
}

UI_BaseElement::UI_BaseElement()
	: m_absolutePosition(0, 0),
	m_absoluteSize(200, 200),
	m_relativePosition(0, 0),
	m_relativeSize(0, 0),
	m_hidden(false),
	m_zIndex(0),
	m_backgroundColour(0, 0, 0),
	m_elementType(ET_Base),
	m_name("")
{
}

UI_BaseElement::~UI_BaseElement()
{
}

UI_Frame::UI_Frame()
{
	m_elementType = ET_Frame;
}

UI_Frame::~UI_Frame()
{
}

UI_Image::UI_Image()
	: m_texture(TextureLoader::CreateTexture2DFromFile("Texture", "hi.png"))
{
	m_elementType = ET_Image;
}

UI_Image::~UI_Image()
{
}

UI_Text::UI_Text()
{
	m_elementType = ET_Text;
}

UI_Text::~UI_Text()
{
}

UI_ImageButton::UI_ImageButton()
{
	m_elementType = ET_ImageButton;
}

UI_ImageButton::~UI_ImageButton()
{
}