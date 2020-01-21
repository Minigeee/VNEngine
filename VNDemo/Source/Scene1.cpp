#include <Scene1.h>

#include <Engine/Engine.h>

#include <UI/Text.h>
#include <UI/Button.h>
#include <UI/TextInupt.h>
#include <UI/Slider.h>

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

void onSubmit(TextInput* input)
{
	std::cout << input->getText().getString().toAnsiString() << "\n";
}

void onValueChanged(Slider* slider)
{
	std::cout << slider->getValue() << "\n";
}

void Scene1::onInit()
{
	Resource<sf::Font>::addLocation("Fonts/segoeui/segoeui.ttf", "SegoeUI");
	Resource<sf::Texture>::addLocation("Textures/YourName.jpg", "YourName");

	Text text;
	text.setView(&mEngine->getWindow().getView());
	text.setCharacterSize(30);

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
	button->moveToBack();

	TextInput* input = mUI.create<TextInput>("Input1");
	input->setPosition(10, 200);
	input->setSize(300, 45);
	input->getBody().setFillColor(sf::Color(25, 25, 30));
	input->getText().setString("Hello World!");
	input->getText().setCharacterSize(25);
	input->setSubmitFunc(&onSubmit);
	mUI.addToRoot(input);

	Slider* slider = mUI.create<Slider>("Slider1");
	slider->setPosition(10, 260);
	slider->setSize(300, 30);
	slider->getBody().setFillColor(sf::Color(25, 25, 30));
	slider->getSlider().setSize(sf::Vector2f(50.0f, 20.0f));
	slider->getSlider().setFillColor(sf::Color(70, 70, 80));
	slider->setValueChangedFunc(&onValueChanged);
	mUI.addToRoot(slider);
}

// ============================================================================