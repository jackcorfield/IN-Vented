#include "UI_Widget.h"

UI_WidgetComponent::UI_WidgetComponent()
{
}

UI_WidgetComponent::~UI_WidgetComponent()
{
	for (UI_BaseElement* element : m_elements)
		delete element;
}

UI_BaseElement* UI_WidgetComponent::AddElement(int type)
{
	UI_BaseElement* newElement;

	switch (type)
	{
	case(ElementType::ET_Image):
		newElement = new UI_Image();
		break;
	case(ElementType::ET_ImageButton):
		newElement = new UI_ImageButton();
		break;
	case(ElementType::ET_Text):
		newElement = new UI_Text();
		break;
	default:
		newElement = new UI_BaseElement();
		break;
	}

	newElement->m_widget = this;

	int count = 0;
	for (UI_BaseElement* e : m_elements)
	{
		if (e->m_elementType == type)
			count++;
	}

	newElement->m_name = s_ElementType[type] + std::to_string(count);

	m_elements.push_back(newElement);
	return newElement;
}

void UI_WidgetComponent::RemoveElement(std::string name)
{
	for (int i = 0; i < m_elements.size(); i++)
	{
		UI_BaseElement* e = m_elements[i];
		if (e->m_name == name)
		{
			delete e;
			m_elements.erase(m_elements.begin() + i);
		}
	}
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
	m_clickBus(new EventBus()),
	m_hoveredTexture(TextureLoader::CreateTexture2DFromFile("Texture", "TestImages/button_hovered.png")),
	m_clickedTexture(TextureLoader::CreateTexture2DFromFile("Texture", "TestImages/button_clicked.png"))
{
	m_texture = TextureLoader::CreateTexture2DFromFile("Texture", "TestImages/button_normal.png");
	m_elementType = ET_ImageButton;
	m_absoluteSize = glm::vec2(200, 80);
	m_relativePosition = glm::vec2(0.5, 0.5);
}

UI_ImageButton::~UI_ImageButton()
{
	delete m_clickBus;
}