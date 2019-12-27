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

NovelScene::NovelScene(Engine* engine) :
	Scene		(engine),
	mUI			(engine)
{

}

NovelScene::~NovelScene()
{

}

// ============================================================================
// ============================================================================

void NovelScene::init()
{
	onInit();
}

// ============================================================================

void NovelScene::handleEvent(const sf::Event& e)
{

}

// ============================================================================

void NovelScene::update(float dt)
{

}

// ============================================================================

void NovelScene::render()
{
	sf::RenderWindow& target = mEngine->getWindow();

	target.draw(mUI);
}

// ============================================================================