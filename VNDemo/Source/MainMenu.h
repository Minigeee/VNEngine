#ifndef VN_DEMO_MAIN_MENU_H
#define VN_DEMO_MAIN_MENU_H

#include <Engine/Scene.h>

// ============================================================================

class MainMenu : public vne::MainMenuScene
{
public:
	MainMenu(vne::Engine* engine);

private:
	void onInit() override;

	void onNewGame(vne::UIElement* btn, const sf::Event& e) override;
};

// ============================================================================

#endif