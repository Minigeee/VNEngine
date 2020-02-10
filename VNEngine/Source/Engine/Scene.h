#ifndef SCENE_H
#define SCENE_H

#include <UI/UI.h>

#include <SFML/Window.hpp>

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

protected:
	/// <summary>
	/// Access to main engine
	/// </summary>
	Engine* mEngine;
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