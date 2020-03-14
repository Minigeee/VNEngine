#include <Setup.h>

#include <Engine/Engine.h>

// All scenes
#include <MainMenu.h>
#include <Scene1.h>

using namespace vne;

// ============================================================================

Setup::Setup(Engine* engine) :
	SetupScene		(engine)
{

}

// ============================================================================

void Setup::onInit()
{
	// Add all scenes
	mEngine->addScene("main_menu", new MainMenu(mEngine));
	mEngine->addScene("s1", new Scene1(mEngine));

	// Add all resources
	Resource<sf::Font>::addLocation("Fonts/segoeui/segoeui.ttf", "segoeui");
	Resource<sf::Texture>::addLocation("Textures/YourName.jpg", "your_name");
	Resource<sf::Texture>::addLocation("Textures/Thing.png", "thing");
	Resource<sf::Texture>::addLocation("Textures/Thing2.png", "thing2");
	Resource<sf::Music>::addLocation("Music/5_yuri.ogg", "yuri");

	// Characters
	mEngine->addCharacter(Character("Mollusk"));
	mEngine->addCharacter(Character("Uncle Bob"));
	mEngine->addCharacter(Character("Uncle Billy"));
	mEngine->addCharacter(Character("Dr. Evil Webkie"));
	mEngine->addCharacter(Character("Fafa"));
	mEngine->addCharacter(Character("Bebe"));

	Character& dr = mEngine->getCharacter("Dr. Evil Webkie");
	dr.addImage("thing");
	dr.addImage("thing2");

	// Default variable values
	if (!mEngine->variableExists("test"))
		mEngine->setVariable("test", 100);

	// Set default font
	mEngine->setDefaultFont(Resource<sf::Font>::get("segoeui"));
}

// ============================================================================

void Setup::gotoFirstScene()
{
	mEngine->gotoScene("s1");
}

// ============================================================================