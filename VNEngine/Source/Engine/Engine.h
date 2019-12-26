#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace vne
{

// ============================================================================

class Scene;

// ============================================================================

/// <summary>
/// This contains parameters used when starting the engine.
/// </summary>
struct EngineParams
{
	EngineParams();

	/// <summary>
	/// Initial width of the game window
	/// </summary>
	int mWindowWidth;

	/// <summary>
	/// Initial height of the game window
	/// </summary>
	int mWindowHeight;

	/// <summary>
	/// Title of the game window.
	/// </summary>
	sf::String mWindowTitle;

	/// <summary>
	/// The game window will be in fullscreen mode if this parameter is set to true.
	/// </summary>
	bool mFullscreen;

	/// <summary>
	/// If true, the window will be resizable
	/// </summary>
	bool mResizable;

	/// <summary>
	/// Use this to specify the initial scene.
	/// Leave this as NULL to start at the main menu
	/// </summary>
	Scene* mStartScene;
};

// ============================================================================

/// <summary>
/// Main engine object that manages execution of the game.
/// Call Init() to initialize engine, the Run() to start the game loop.
/// </summary>
class Engine
{
public:
	Engine();
	~Engine();

	/// <summary>
	/// Initialize the game with the given parameters.
	/// This function sets up the game window, sets up the game systems, and loads initial resources.
	/// </summary>
	/// <param name="params">Parameter object that specifies certain settings (i.e. window size)</param>
	/// <returns>True if engine successfully initialized</returns>
	bool init(const EngineParams& params);

	/// <summary>
	/// Start game loop.
	/// </summary>
	void run();

	/// <summary>
	/// Stops the engine after completing the current frame
	/// </summary>
	void close();


	/// <summary>
	/// Get main game window
	/// </summary>
	/// <returns>SFML window</returns>
	sf::RenderWindow& getWindow();

private:
	/// <summary>
	/// Handles all input events from the window
	/// </summary>
	void pollEvents();

	/// <summary>
	/// Do any game updates, including animations, displaying text...
	/// </summary>
	/// <param name="dt">Time elapsed since last frame (in seconds)</param>
	void update(float dt);

	/// <summary>
	/// Render all UI elements, sprites, textures...
	/// </summary>
	void render();

private:
	/// <summary>
	/// Main game window.
	/// </summary>
	sf::RenderWindow mWindow;

	/// <summary>
	/// Current scene
	/// </summary>
	Scene* mScene;
};

// ============================================================================

}

#endif
