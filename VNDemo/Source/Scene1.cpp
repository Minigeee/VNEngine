#include <Scene1.h>

#include <UI/Button.h>
#include <UI/TextInupt.h>

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

	TextInput* input = mUI.create<TextInput>("Input1");
	input->setPosition(10, 200);
	input->setRotation(45.0f);
	input->setSize(300, 45);
	input->getBody().setFillColor(sf::Color(25, 25, 30));
	input->getText().setString("Hello World!");
	input->getText().setCharacterSize(25);
	mUI.addToRoot(input);
}

// ============================================================================