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

	DialogueAction* d1 = alloc<DialogueAction>();
	d1->setName("Mollusk");
	d1->setDialogue("Hello! My name is Bello. I like to play the cello. My favorite food is jello. Bananas are yellow.");
	addAction(d1);

	DialogueAction* d2 = alloc<DialogueAction>();
	d2->setName("Fabefabe");
	d2->setDialogue("I is awesome!");
	addAction(d2);
}

// ============================================================================

void Scene1::onUpdate(float dt)
{

}

// ============================================================================