#include <Setup.h>

#include <Engine/Engine.h>

// All scenes
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
	mEngine->addScene("s1", new Scene1(mEngine));

	// Add all resources
	Resource<sf::Font>::addLocation("Fonts/segoeui/segoeui.ttf", "segoeui");
	Resource<sf::Texture>::addLocation("Textures/YourName.jpg", "your_name");

	// Default variable values
	if (!mEngine->variableExists("test"))
		mEngine->setVariable("test", 100);
}

// ============================================================================

void Setup::gotoFirstScene()
{
	mEngine->gotoScene("s1");
}

// ============================================================================