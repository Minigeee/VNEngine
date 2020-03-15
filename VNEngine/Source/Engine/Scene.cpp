#include <Engine/Scene.h>
#include <Engine/Engine.h>
#include <Engine/Resource.h>

#include <SFML/Graphics.hpp>

#include <UI/Button.h>

using namespace vne;

// ============================================================================
// ============================================================================

Scene::Scene(Engine* engine) :
	mEngine			(engine),
	mActionIndex	(-1)
{

}

Scene::~Scene()
{
	// Remove all object pools
	for (auto it = mObjectPools.begin(); it != mObjectPools.end(); ++it)
		delete it->second;
}

// ============================================================================

void Scene::addAction(Action* action)
{
	action->setScene(this);
	mActions.push_back(action);
}

void Scene::addAnimation(I_Animation* anim)
{
	anim->reset();
	mAnimations.push_back(anim);
}

// ============================================================================

void Scene::update(float dt)
{
	if (mActions.size())
	{
		if (mActionIndex >= 0 && mActionIndex < mActions.size())
			mActions[mActionIndex]->update(dt);

		// If the current action is completed or the index is negative
		if (mActionIndex < 0 || (mActionIndex < mActions.size() && mActions[mActionIndex]->isComplete()))
		{
			// Skip actions until we find one with conditions met
			while (++mActionIndex < mActions.size() && !mActions[mActionIndex]->isConditionMet())
				mActions[mActionIndex]->setComplete(true);

			if (mActionIndex < mActions.size())
				// Run action
				mActions[mActionIndex]->run();
		}
	}

	// Update animations
	for (Uint32 i = 0; i < mAnimations.size(); ++i)
	{
		// Update animation
		I_Animation* anim = mAnimations[i];
		anim->update(dt);

		// Remove if finished
		if (anim->isFinished())
		{
			mAnimations[i--] = mAnimations.back();
			mAnimations.pop_back();
		}
	}
}

void Scene::cleanup()
{
	// Remove all objects from object pools
	for (auto it = mObjectPools.begin(); it != mObjectPools.end(); ++it)
		it->second->free();

	// Clear actions list
	mActions.clear();
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
	// No actions in setup scene
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
	mUI.init();

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
	Scene::update(dt);

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

UI& MainMenuScene::getUI()
{
	return mUI;
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
	mUI.init();

	// Get view size
	const sf::Vector2f& viewSize = mEngine->getWindow().getView().getSize();

	mBackground = mUI.create<ImageBox>("Background");
	mBackground->setSize(viewSize);
	mBackground->setColor(sf::Color::White);
	mUI.addToRoot(mBackground);

	mDialogueBox = mUI.create<ImageBox>("DialogueBox");
	mDialogueBox->setSize(viewSize.x * 0.8f, viewSize.y * 0.25f);
	mDialogueBox->setOrigin(UIPosition::BotCenter);
	mDialogueBox->setAnchor(UIPosition::BotCenter);
	mDialogueBox->setPosition(0.0f, -10.0f);
	mDialogueBox->setColor(sf::Color(25, 25, 30, 120));
	mUI.addToRoot(mDialogueBox);

	mNameBox = mUI.create<ImageBox>("NameBox");
	mNameBox->setSize(250.0f, 60.0f);
	mNameBox->setOrigin(UIPosition::BotLeft);
	mNameBox->setAnchor(UIPosition::TopLeft);
	mNameBox->setPosition(0.0f, 0.0f);
	mNameBox->setColor(sf::Color(40, 40, 45, 120));
	mDialogueBox->addChild(mNameBox);

	mDialogueText = mUI.create<TextBox>("DialogueText");
	mDialogueText->setPosition(20.0f, 20.0f);
	mDialogueText->setString("Hello World! The quick brown fox jumps over the lazy dog.");
	mDialogueText->setCharacterSize(35);
	mDialogueText->setWordWrap(viewSize.x * 0.8f - 40.0f);
	mDialogueBox->addChild(mDialogueText);

	mNameText = mUI.create<TextBox>("NameText");
	mNameText->setOrigin(UIPosition::Left);
	mNameText->setAnchor(UIPosition::Left);
	mNameText->setPosition(20.0f, -3.0f);
	mNameText->setString("Name");
	mNameText->setCharacterSize(35);
	mNameBox->addChild(mNameText);

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
	bool handled = mUI.handleEvent(e);

	if (mActionIndex >= 0 && mActionIndex < mActions.size() && !handled)
		mActions[mActionIndex]->handleEvent(e);
}

// ============================================================================

void NovelScene::update(float dt)
{
	Scene::update(dt);

	// Custom update comes first
	onUpdate(dt);

	mUI.update(dt);
}

void NovelScene::onUpdate(float dt)
{

}

// ============================================================================

void NovelScene::addAction(Action* action)
{
	if (mActionGroups.empty())
		Scene::addAction(action);
	else
		mActionGroups.top()->addAction(action);
}

// ============================================================================

UI& NovelScene::getUI()
{
	return mUI;
}

ImageBox* NovelScene::getBackground() const
{
	return mBackground;
}

ImageBox* NovelScene::getDialogueBox() const
{
	return mDialogueBox;
}

TextBox* NovelScene::getDialogueText() const
{
	return mDialogueText;
}

ImageBox* NovelScene::getNameBox() const
{
	return mNameBox;
}

TextBox* NovelScene::getNameText() const
{
	return mNameText;
}

std::stack<ActionGroup*>& NovelScene::getActionGroups()
{
	return mActionGroups;
}

// ============================================================================

void NovelScene::startGroup(bool parallel, const std::function<bool()>& condition)
{
	ActionGroup* action = alloc<ActionGroup>();
	action->setParallel(parallel);
	action->setCondition(condition);

	// Add to top of stack
	mActionGroups.push(action);

	// Add action to list
	Scene::addAction(mActionGroups.top());
}

void NovelScene::endGroup()
{
	// Pop
	mActionGroups.pop();
}

void NovelScene::narrate(const sf::String& dialogue)
{
	DialogueAction* action = alloc<DialogueAction>();
	action->setName("");
	action->setDialogue(dialogue);
	action->setTextStyle(sf::Text::Regular);
	addAction(action);
}

void NovelScene::background(const sf::String& bgName, Transition effect, float duration)
{
	background(Resource<sf::Texture>::get(bgName), effect, duration);
}

void NovelScene::background(sf::Texture* texture, Transition effect, float duration)
{
	BackgroundAction* action = alloc<BackgroundAction>();
	action->setTexture(texture);
	action->setTransition(effect);
	action->setDuration(duration);
	addAction(action);
}

void NovelScene::start(const sf::String& music, float volume, Transition effect, float duration)
{
	start(Resource<sf::Music>::get(music), volume, effect, duration);
}

void NovelScene::start(sf::Music* music, float volume, Transition effect, float duration)
{
	MusicAction* action = alloc<MusicAction>();
	action->setMusic(music);
	action->setVolume(volume);
	action->setMode(MusicAction::Start);
	action->setTransition(effect);
	action->setDuration(duration);
	addAction(action);
}

void NovelScene::stop(const sf::String& music, Transition effect, float duration)
{
	stop(Resource<sf::Music>::get(music), effect, duration);
}

void NovelScene::stop(sf::Music* music, Transition effect, float duration)
{
	MusicAction* action = alloc<MusicAction>();
	action->setMusic(music);
	action->setMode(MusicAction::Stop);
	action->setTransition(effect);
	action->setDuration(duration);
	addAction(action);
}

void NovelScene::sound(const sf::String& name, float volume)
{
	sound(Resource<sf::SoundBuffer>::get(name), volume);
}

void NovelScene::sound(sf::SoundBuffer* buffer, float volume)
{
	SoundAction* action = alloc<SoundAction>();
	action->setBuffer(buffer);
	action->setVolume(volume);
	addAction(action);
}

// ============================================================================

void NovelScene::render()
{
	sf::RenderWindow& target = mEngine->getWindow();

	target.draw(mUI);
}

// ============================================================================