#include <Scene1.h>

#include <Engine/Engine.h>

#include <UI/UIAnimation.h>
#include <UI/Text.h>
#include <UI/Button.h>
#include <UI/TextInupt.h>
#include <UI/Slider.h>
#include <UI/ScrollView.h>
#include <UI/ListContainer.h>

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

void onValueChanged(Slider* slider, bool dragged)
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

	ListContainer* list = mUI.create<ListContainer>("List");

	Button* button = mUI.create<Button>("PlayBtn");
	button->setPosition(0.0f, 1470.0f);
	button->setSize(150.0f, 60.0f);
	button->getLabel().setCharacterSize(25);
	button->getLabel().setString("Play");
	button->setDefaultStateFunc(&defaultState);
	button->setHoverStateFunc(&hoverState);
	button->setPressStateFunc(&pressState);
	button->setFocusFunc(&onFocus);
	list->addChild(button);

	list->addChild(mUI.copy<Button>("PlayBtn2", "PlayBtn"));
	list->addChild(mUI.copy<Button>("PlayBtn3", "PlayBtn"));
	list->addChild(mUI.copy<Button>("PlayBtn4", "PlayBtn"));
	list->addChild(mUI.copy<Button>("PlayBtn5", "PlayBtn"));
	list->addChild(mUI.copy<Button>("PlayBtn6", "PlayBtn"));
	list->addChild(mUI.copy<Button>("PlayBtn7", "PlayBtn"));
	list->addChild(mUI.copy<Button>("PlayBtn8", "PlayBtn"));
	list->addChild(mUI.copy<Button>("PlayBtn9", "PlayBtn"));
	list->addChild(mUI.copy<Button>("PlayBtn10", "PlayBtn"));

	button = mUI.copy<Button>("TestBtn", "PlayBtn");
	button->setPosition(50.0f, 50.0f);
	button->setRotation(45.0f);
	button->getLabel().setString("Test");
	mUI.addToRoot(button);

	Vec2Animation* anim = ANIMATE_POS(button, button->getRelPosition(), sf::Vector2f(500.0f, 500.0f), 2.0f);
	anim->setDelay(2.0f);
	anim->setPowFactor(2.0f);
	mUI.addAnimation(anim);
	mUI.addAnimation(ANIMATE_ROT(button, 0.0f, 360.0f, 2.0f));
	mUI.addAnimation(ANIMATE_FLOAT(&button->getBody(), OutlineThickness, 0.0f, 20.0f, 2.0f));
	mUI.addAnimation(ANIMATE_COLOR(&button->getBody(), FillColor, sf::Color::Red, sf::Color::Blue, 2.0f));
	mUI.addAnimation(ANIMATE_COLOR(&button->getBody(), OutlineColor, sf::Color::Green, sf::Color::Red, 2.0f));

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
	slider->setSliderWidth(60.0f);
	slider->getBody().setFillColor(sf::Color(25, 25, 30));
	slider->getSlider().setFillColor(sf::Color(70, 70, 80));
	slider->setValueChangedFunc(&onValueChanged);
	mUI.addToRoot(slider);

	ScrollView* scroll = mUI.create<ScrollView>("ScrollView");
	scroll->setPosition(-310.0f, -410.0f);
	scroll->setAnchor(1.0f, 1.0f);
	scroll->setSize(300.0f, 400.0f);
	scroll->getBackground().setFillColor(sf::Color(25, 25, 30));
	scroll->getScrollBar()->getSlider().setFillColor(sf::Color(70, 70, 80));
	mUI.addToRoot(scroll);

	scroll->addChild(list);
}

// ============================================================================

void Scene1::onUpdate(float dt)
{

}

// ============================================================================