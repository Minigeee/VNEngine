#ifndef SCENE_H
#define SCENE_H

#include <UI/UI.h>
#include <UI/Button.h>

#include <SFML/Window.hpp>

#include <unordered_map>
#include <typeinfo>
#include <typeindex>

namespace vne
{

// ============================================================================

class Engine;

/// <summary>
/// Self contained section of gameplay -
/// * Can be used to contain a mini arc within the story
/// * Can be used to implement a custom mini game, etc.
/// </summary>
class Scene
{
public:
	Scene(Engine* engine);
	virtual ~Scene();

	/// <summary>
	/// Initialize any resources in this function.
	/// Called once on scene initialization
	/// </summary>
	virtual void init() = 0;

	/// <summary>
	/// Used to cleanup any UI elements or resources that were used, or just as a custom callback.
	/// Called when the scene switches from being the current to noncurrent
	/// </summary>
	virtual void cleanup();

	/// <summary>
	/// Handle window input
	/// </summary>
	/// <param name="e"></param>
	virtual void handleEvent(const sf::Event& e) = 0;

	/// <summary>
	/// Do any game updates (i.e. animation, etc.)
	/// Called once a frame
	/// </summary>
	/// <param name="dt">Time elapsed since last frame</param>
	virtual void update(float dt) = 0;

	/// <summary>
	/// Render any sprites or UI elements to screen.
	/// Called after scene update
	/// </summary>
	virtual void render() = 0;

	/// <summary>
	/// Allocate an object from the scene's managed memory
	/// </summary>
	template <typename T, typename... Args>
	T* alloc(Args&&... args)
	{
		std::type_index typeID = typeid(T);

		IObjectPool*& pool = mObjectPools[typeID];
		if (!pool)
			pool = new ObjectPool<T>();

		return ((ObjectPool<T>*)pool)->create(std::forward<Args>(args)...);
	}

	/// <summary>
	/// Free an object that was allocated with the scene's managed memory
	/// </summary>
	template <typename T>
	void free(T* ptr)
	{
		std::type_index typeID = typeid(T);

		void*& pool = mObjectPools[typeID];
		if (pool)
			((ObjectPool<T>*)pool)->free(ptr);
	}

protected:
	/// <summary>
	/// Access to main engine
	/// </summary>
	Engine* mEngine;

	/// <summary>
	/// Map of object pool for various types
	/// </summary>
	std::unordered_map<std::type_index, IObjectPool*> mObjectPools;
};

// ============================================================================

/// <summary>
/// Very first scene that is initialized, and is meant to initialize resources,
/// characters, and variables for all other scenes
/// </summary>
class SetupScene : public Scene
{
public:
	SetupScene(Engine* engine);
	virtual ~SetupScene();

	/// <summary>
	/// Do all initialization for game
	/// </summary>
	void init() override;

	/// <summary>
	/// Does nothing
	/// </summary>
	void cleanup() override;

	/// <summary>
	/// Does nothing
	/// </summary>
	/// <param name="e"></param>
	void handleEvent(const sf::Event& e) override;

	/// <summary>
	/// Does nothing
	/// </summary>
	/// <param name="dt">Time elapsed since last frame</param>
	void update(float dt) override;

	/// <summary>
	/// Does nothing
	/// </summary>
	void render() override;

protected:
	/// <summary>
	/// This is where all resources, scenes, variables, and characters should be setup
	/// </summary>
	virtual void onInit() = 0;

	/// <summary>
	/// Override this to have custom first scenes.
	/// The default first scene is the main menu
	/// </summary>
	virtual void gotoFirstScene();
};

// ============================================================================

class MainMenuScene : public Scene
{
public:
	MainMenuScene(Engine* engine);
	virtual ~MainMenuScene();

	/// <summary>
	/// Setup UI
	/// </summary>
	void init() override;

	/// <summary>
	/// Cleans UI resources
	/// </summary>
	void cleanup() override;

	/// <summary>
	/// Handle window input for UI
	/// </summary>
	/// <param name="e"></param>
	void handleEvent(const sf::Event& e) override;

	/// <summary>
	/// Do animation updates, update displayed text
	/// </summary>
	/// <param name="dt">Time elapsed since last frame</param>
	void update(float dt) override;

	/// <summary>
	/// Render UI and sprites
	/// </summary>
	void render() override;

protected:
	/// <summary>
	/// This is where the actions list should be populated
	/// </summary>
	virtual void onInit();

	/// <summary>
	/// Use this function to implement any extra features needed
	/// </summary>
	virtual void onUpdate(float dt);

	/// <summary>
	/// Called when menu buttons enter their default state
	/// </summary>
	/// <param name="btn">Button that entered default state</param>
	/// <param name="prevState">The state it switched from</param>
	virtual void onDefaultBtn(Button* btn, Button::State prevState);

	/// <summary>
	/// Called when menu buttons enter their hover state
	/// </summary>
	/// <param name="btn">Button that entered hover state</param>
	/// <param name="prevState">The state it switched from</param>
	virtual void onHoverBtn(Button* btn, Button::State prevState);

	/// <summary>
	/// Called when menu buttons enter their press state
	/// </summary>
	/// <param name="btn">Button that entered press state</param>
	/// <param name="prevState">The state it switched from</param>
	virtual void onPressBtn(Button* btn, Button::State prevState);

	/// <summary>
	/// Called when "New" button is pressed
	/// </summary>
	/// <param name="btn">Button</param>
	virtual void onNewGame(UIElement* btn, const sf::Event& e);

	/// <summary>
	/// Called when "Load" button is pressed
	/// </summary>
	/// <param name="btn">Button</param>
	virtual void onLoadGame(UIElement* btn, const sf::Event& e);

	/// <summary>
	/// Called when "Settings" button is pressed
	/// </summary>
	/// <param name="btn">Button</param>
	virtual void onSettingsBtn(UIElement* btn, const sf::Event& e);

	/// <summary>
	/// Called when "Exit" button is pressed
	/// </summary>
	/// <param name="btn">Button</param>
	virtual void onExitBtn(UIElement* btn, const sf::Event& e);

protected:
	/// <summary>
	/// UI system
	/// </summary>
	UI mUI;

	/// <summary>
	/// The new game button
	/// </summary>
	Button* mNewBtn;

	/// <summary>
	/// The load game button
	/// </summary>
	Button* mLoadBtn;

	/// <summary>
	/// The settings button
	/// </summary>
	Button* mSettingsBtn;

	/// <summary>
	/// The exit button
	/// </summary>
	Button* mExitBtn;
};

// ============================================================================

/// <summary>
/// A scene specialized for visual novel story / text
/// </summary>
class NovelScene : public Scene
{
public:
	NovelScene(Engine* engine);
	virtual ~NovelScene();

	/// <summary>
	/// Load resources, setup UI
	/// </summary>
	void init() override;

	/// <summary>
	/// Cleans UI resources
	/// </summary>
	void cleanup() override;

	/// <summary>
	/// Handle window input for UI
	/// </summary>
	/// <param name="e"></param>
	void handleEvent(const sf::Event& e) override;

	/// <summary>
	/// Do animation updates, update displayed text
	/// </summary>
	/// <param name="dt">Time elapsed since last frame</param>
	void update(float dt) override;

	/// <summary>
	/// Render UI and sprites
	/// </summary>
	void render() override;

protected:
	/// <summary>
	/// This is where the actions list should be populated
	/// </summary>
	virtual void onInit() = 0;

	/// <summary>
	/// Use this function to implement any extra features needed
	/// </summary>
	virtual void onUpdate(float dt);

protected:
	/// <summary>
	/// UI system
	/// </summary>
	UI mUI;
	
	
};

// ============================================================================

}

#endif