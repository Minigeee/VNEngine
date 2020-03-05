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
	mBackground->setTexture(Resource<sf::Texture>::get("your_name"));

	Character& m = mEngine->getCharacter("Mollusk");
	Character& bob = mEngine->getCharacter("Uncle Bob");
	Character& billy = mEngine->getCharacter("Uncle Billy");
	Character& dr = mEngine->getCharacter("Dr. Evil Webkie");
	Character& f = mEngine->getCharacter("Fafa");
	Character& b = mEngine->getCharacter("Bebe");

	narrate("I am narrator");
	m.think("This is a pretty cool picture...");
	m.say("Once upon a time... there was a universe...");
	m.say("This universe was very, very big. Like super duper big... Like bigger than your face...");
	m.say("This universe was surrounded by a big brick wall, and there was nothing inside.");
	m.say("It was empty.");
	m.say("Life did not exist in this vast empty space.");
	m.say("But then!");
	m.say("One day, Littly Liar Bluey came into existence!");
	m.say("He drifted alone through the void for a long long super long time...");
	m.say("He didn't know what to do, there was nothing for him in this universe, so he became very bored.");
	dr.say("I am weird!!!!!!!!!!!!!");
	f.say("HEY Dr. Evil Webkin YOU SUCK BUTT why is you here??");
	b.say("uhhhhhh, wait what is he doing here. He be a real thinghole butthole facehole bellyhole hehehehe.");
}

// ============================================================================

void Scene1::onUpdate(float dt)
{

}

// ============================================================================