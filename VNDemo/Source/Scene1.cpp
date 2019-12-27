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

void Scene1::onInit()
{
	Resource<sf::Font>::addLocation("Fonts/segoeui/segoeui.ttf", "SegoeUI");

	sf::Font* font = Resource<sf::Font>::get("SegoeUI");

	Button* button = mUI.create<Button>("PlayBtn");
	button->setSize(150.0f, 60.0f);
	button->getBody().setFillColor(sf::Color(130, 130, 250));
	button->getLabel().setFont(*font);
	button->getLabel().setString("Play");
	mUI.addToRoot(button);
}

// ============================================================================