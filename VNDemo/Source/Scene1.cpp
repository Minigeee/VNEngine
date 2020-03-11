#include <Scene1.h>

#include <Engine/Engine.h>
#include <Engine/Action.h>

#include <UI/UIAnimation.h>
#include <UI/Text.h>
#include <UI/Button.h>
#include <UI/TextInupt.h>
#include <UI/Slider.h>
#include <UI/ScrollView.h>
#include <UI/ListContainer.h>
#include <UI/ImageBox.h>
#include <UI/TextBox.h>

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
	mBackground->setColor(sf::Color::Black);

	Character& m = mEngine->getCharacter("Mollusk");
	Character& bob = mEngine->getCharacter("Uncle Bob");
	Character& billy = mEngine->getCharacter("Uncle Billy");
	Character& dr = mEngine->getCharacter("Dr. Evil Webkie");
	Character& f = mEngine->getCharacter("Fafa");
	Character& b = mEngine->getCharacter("Bebe");

	narrate("I am narrator");
	m.think("This is a pretty cool picture...");
	m.show("thing");
	dr.say("Mwahahahahaha!!!!!!!!!!!! I am here to destroy you!");
}

// ============================================================================

void Scene1::onUpdate(float dt)
{

}

// ============================================================================