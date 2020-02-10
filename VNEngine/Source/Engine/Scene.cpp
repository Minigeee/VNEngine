#include <Engine/Scene.h>
#include <Engine/Engine.h>
#include <Engine/Resource.h>

#include <SFML/Graphics.hpp>

using namespace vne;

// ============================================================================
// ============================================================================

Scene::Scene(Engine* engine) :
	mEngine			(engine)
{

}

Scene::~Scene()
{

}

// ============================================================================
// ============================================================================

SetupScene::SetupScene(Engine* engine) :
	Scene		(engine)
{

}

SetupScene::~SetupScene()
{
	auto scenes = mEngine->getSceneMap();

	// Delete all scenes
	for (auto it = scenes.begin(); it != scenes.end(); ++it)
		delete it->second;
}

// ============================================================================

void SetupScene::init()
{
	onInit();
	gotoFirstScene();
}

void SetupScene::gotoFirstScene()
{
	mEngine->gotoScene("main_menu");
}

// ============================================================================

void SetupScene::handleEvent(const sf::Event& e)
{

}

void SetupScene::update(float dt)
{

}

void SetupScene::render()
{

}

// ============================================================================
// ============================================================================

NovelScene::NovelScene(Engine* engine) :
	Scene		(engine),
	mUI			(engine)
{

}

NovelScene::~NovelScene()
{

}

// ============================================================================

void NovelScene::init()
{
	onInit();
}

// ============================================================================

void NovelScene::handleEvent(const sf::Event& e)
{
	mUI.handleEvent(e);
}

// ============================================================================

void NovelScene::update(float dt)
{
	// Custom update comes first
	onUpdate(dt);

	mUI.update(dt);
}

void NovelScene::onUpdate(float dt)
{

}

// ============================================================================

void NovelScene::render()
{
	sf::RenderWindow& target = mEngine->getWindow();

	target.draw(mUI);
}

// ============================================================================