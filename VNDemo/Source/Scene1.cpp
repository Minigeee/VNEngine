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

void onHoverStart(UIElement* element, const sf::Event& e)
{
	Button* btn = dynamic_cast<Button*>(element);
	btn->getBody().setFillColor(sf::Color(150, 150, 255));
}

void onHoverStop(UIElement* element, const sf::Event& e)
{
	Button* btn = dynamic_cast<Button*>(element);
	btn->getBody().setFillColor(sf::Color(130, 130, 250));
}

void onPressStart(UIElement* element, const sf::Event& e)
{
	Button* btn = dynamic_cast<Button*>(element);
	std::cout << btn->getLabel().getString().toAnsiString() << " pressed\n";
}

void onPressStop(UIElement* element, const sf::Event& e)
{
	Button* btn = dynamic_cast<Button*>(element);
	std::cout << btn->getLabel().getString().toAnsiString() << " released\n";
}

void onKeyPress(UIElement* element, const sf::Event& e)
{
	Button* btn = dynamic_cast<Button*>(element);
	std::cout << btn->getLabel().getString().toAnsiString() << " key pressed\n";
}

void onKeyRelease(UIElement* element, const sf::Event& e)
{
	Button* btn = dynamic_cast<Button*>(element);
	std::cout << btn->getLabel().getString().toAnsiString() << " key released\n";
}

void onTextEntered(UIElement* element, const sf::Event& e)
{
	Button* btn = dynamic_cast<Button*>(element);
	std::cout << (char)e.text.unicode;
}

void Scene1::onInit()
{
	Resource<sf::Font>::addLocation("Fonts/segoeui/segoeui.ttf", "SegoeUI");

	sf::Font* font = Resource<sf::Font>::get("SegoeUI");

	Button* button = mUI.create<Button>("PlayBtn");
	button->setSize(150.0f, 60.0f);
	button->getBody().setFillColor(sf::Color(130, 130, 250));
	button->getLabel().setFont(*font);
	button->getLabel().setString("Play");
	button->setMouseEnterFunc(&onHoverStart);
	button->setMouseExitFunc(&onHoverStop);
	button->setMousePressFunc(&onPressStart);
	button->setMouseReleaseFunc(&onPressStop);
	button->setTextEnteredFunc(&onTextEntered);
	mUI.addToRoot(button);

	button = mUI.create<Button>("TestBtn");
	button->setSize(150.0f, 60.0f);
	button->setPosition(50.0f, 50.0f);
	button->getBody().setFillColor(sf::Color(130, 130, 250));
	button->getLabel().setFont(*font);
	button->getLabel().setString("Test");
	button->setMouseEnterFunc(&onHoverStart);
	button->setMouseExitFunc(&onHoverStop);
	button->setMousePressFunc(&onPressStart);
	button->setMouseReleaseFunc(&onPressStop);
	button->setKeyPressFunc(&onKeyPress);
	button->setKeyReleaseFunc(&onKeyRelease);
	mUI.addToRoot(button);
}

// ============================================================================