#ifndef SCENE_H
#define SCENE_H

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
};

// ============================================================================

}

#endif