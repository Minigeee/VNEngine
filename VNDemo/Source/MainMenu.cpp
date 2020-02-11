#include <MainMenu.h>

#include <Engine/Engine.h>

using namespace vne;

// ============================================================================

MainMenu::MainMenu(Engine* engine) :
	MainMenuScene	(engine)
{

}

// ============================================================================

void MainMenu::onInit()
{

}

// ============================================================================

void MainMenu::onNewGame(UIElement* btn, const sf::Event& e)
{
	mEngine->gotoScene("s1");
}

// ============================================================================