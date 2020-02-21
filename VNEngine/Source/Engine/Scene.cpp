#include <Engine/Scene.h>
#include <Engine/Engine.h>
#include <Engine/Resource.h>

#include <SFML/Graphics.hpp>

#include <UI/Button.h>

using namespace vne;

// ============================================================================
// ============================================================================

Scene::Scene(Engine* engine) :
	mEngine			(engine)
{

}

Scene::~Scene()
{
	// Remove all object pools
	for (auto it = mObjectPools.begin(); it != mObjectPools.end(); ++it)
		delete it->second;
}

// ============================================================================

void Scene::cleanup()
{
	// Remove all objects from object pools
	for (auto it = mObjectPools.begin(); it != mObjectPools.end(); ++it)
		it->second->free();
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

void SetupScene::cleanup()
{
	Scene::cleanup();
}

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

MainMenuScene::MainMenuScene(Engine* engine) :
	Scene			(engine),
	mUI				(engine),
	mNewBtn			(0),
	mLoadBtn		(0),
	mSettingsBtn	(0),
	mExitBtn		(0)
{

}

MainMenuScene::~MainMenuScene()
{

}

// ============================================================================

void MainMenuScene::onDefaultBtn(Button* btn, Button::State prevState)
{
	btn->getBody().setFillColor(sf::Color(25, 25, 30));
}

void MainMenuScene::onHoverBtn(Button* btn, Button::State prevState)
{
	btn->getBody().setFillColor(sf::Color(35, 35, 40));
}

void MainMenuScene::onPressBtn(Button* btn, Button::State prevState)
{
	btn->getBody().setFillColor(sf::Color(25, 25, 30));
}

// ============================================================================

void MainMenuScene::onNewGame(UIElement* btn, const sf::Event& e)
{
	// Override this and goto first scene
}

void MainMenuScene::onLoadGame(UIElement* btn, const sf::Event& e)
{

}

void MainMenuScene::onSettingsBtn(UIElement* btn, const sf::Event& e)
{

}

void MainMenuScene::onExitBtn(UIElement* btn, const sf::Event& e)
{
	mEngine->close();
}

// ============================================================================

void MainMenuScene::init()
{
	// Must set default font
	mUI.setDefaultFont(Resource<sf::Font>::get("segoeui"));

	// Get view size
	const sf::Vector2f& viewSize = mEngine->getWindow().getView().getSize();

	float x = viewSize.x * 0.5f;
	float y = 400.0f;

	using namespace std::placeholders;

	// New game button
	mNewBtn = mUI.create<Button>("NewBtn");
	mNewBtn->setPosition(x, y);
	mNewBtn->setSize(300.0f, 60.0f);
	mNewBtn->setOrigin(UIPosition::Center);
	mNewBtn->getLabel().setString("New");
	mNewBtn->getLabel().setCharacterSize(25);
	mNewBtn->setDefaultStateFunc(std::bind(&MainMenuScene::onDefaultBtn, this, _1, _2));
	mNewBtn->setHoverStateFunc(std::bind(&MainMenuScene::onHoverBtn, this, _1, _2));
	mNewBtn->setPressStateFunc(std::bind(&MainMenuScene::onPressBtn, this, _1, _2));
	mNewBtn->setMouseReleaseFunc(std::bind(&MainMenuScene::onNewGame, this, _1, _2));
	mUI.addToRoot(mNewBtn);

	// Load game button
	mLoadBtn = mUI.copy<Button>("LoadBtn", "NewBtn");
	mLoadBtn->setPosition(x, y + 70.0f);
	mLoadBtn->getLabel().setString("Load");
	mLoadBtn->setMouseReleaseFunc(std::bind(&MainMenuScene::onLoadGame, this, _1, _2));
	mUI.addToRoot(mLoadBtn);

	// Settings button
	mSettingsBtn = mUI.copy<Button>("SettingsBtn", "NewBtn");
	mSettingsBtn->setPosition(x, y + 140.0f);
	mSettingsBtn->getLabel().setString("Settings");
	mSettingsBtn->setMouseReleaseFunc(std::bind(&MainMenuScene::onSettingsBtn, this, _1, _2));
	mUI.addToRoot(mSettingsBtn);

	// Exit button
	mExitBtn = mUI.copy<Button>("ExitBtn", "NewBtn");
	mExitBtn->setPosition(x, y + 210.0f);
	mExitBtn->getLabel().setString("Exit");
	mExitBtn->setMouseReleaseFunc(std::bind(&MainMenuScene::onExitBtn, this, _1, _2));
	mUI.addToRoot(mExitBtn);

	onInit();
}

void MainMenuScene::onInit()
{

}

// ============================================================================

void MainMenuScene::cleanup()
{
	Scene::cleanup();

	// Detach all children element from root
	mUI.getRoot()->removeAllChildren();
}

// ============================================================================

void MainMenuScene::handleEvent(const sf::Event& e)
{
	mUI.handleEvent(e);
}

// ============================================================================

void MainMenuScene::update(float dt)
{
	// Custom update comes first
	onUpdate(dt);

	mUI.update(dt);
}

void MainMenuScene::onUpdate(float dt)
{

}

// ============================================================================

void MainMenuScene::render()
{
	sf::RenderWindow& target = mEngine->getWindow();

	target.draw(mUI);
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

void NovelScene::cleanup()
{
	Scene::cleanup();

	// Detach all children element from root
	mUI.getRoot()->removeAllChildren();
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