#include <Scene1.h>

#include <UI/Button.h>

using namespace vne;

// ============================================================================
// ============================================================================

Scene1::Scene1(Engine* engine) :
	NovelScene			(engine)
{

}

Scene1::~Scene1()
{

}

// ============================================================================
// ============================================================================

#include <iostream>

void defaultState(Button* btn, Button::State prevState)
{
	btn->getBody().setFillColor(sf::Color(130, 130, 250));
}

void hoverState(Button* btn, Button::State prevState)
{
	btn->getBody().setFillColor(sf::Color(150, 150, 255));
}

void pressState(Button* btn, Button::State prevState)
{
	btn->getBody().setFillColor(sf::Color(110, 110, 230));
}

void onFocus(UIElement* element)
{
	Button* btn = (Button*)element;
	std::cout << btn->getLabel().getString().toAnsiString() << " focused\n";
}

void Scene1::onInit()
{
	Resource<sf::Font>::addLocation("Fonts/segoeui/segoeui.ttf", "SegoeUI");
	Resource<sf::Texture>::addLocation("Textures/YourName.jpg", "YourName");

	mUI.setDefaultFont(Resource<sf::Font>::get("SegoeUI"));

	Button* button = mUI.create<Button>("PlayBtn");
	button->setSize(150.0f, 60.0f);
	button->getLabel().setCharacterSize(25);
	button->getLabel().setString("Play");
	button->setDefaultStateFunc(&defaultState);
	button->setHoverStateFunc(&hoverState);
	button->setPressStateFunc(&pressState);
	button->setFocusFunc(&onFocus);
	mUI.addToRoot(button);

	button = mUI.copy<Button>("TestBtn", "PlayBtn");
	button->setPosition(50.0f, 50.0f);
	button->setRotation(45.0f);
	button->getLabel().setString("Test");
	mUI.addToRoot(button);
}

// ============================================================================