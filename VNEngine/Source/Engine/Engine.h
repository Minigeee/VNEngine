#ifndef ENGINE_H
#define ENGINE_H

#include <Core/DataTypes.h>
#include <Core/Variant.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <Engine/Scene.h>
#include <Engine/Character.h>

namespace vne
{

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
	/// Use this to specify the setup scene.
	/// </summary>
	SetupScene* mSetupScene;
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
	/// Set the coordinate space dimensions.
	/// All sprite positions and scales will be based off of this size,
	/// and this coordinate space will be transformed based on window size.
	/// Note that this is different than the window size.
	/// All sizes are measured in pixels.
	/// By default, this space is 1920x1080 pixels.
	/// </summary>
	/// <param name="w">Width of the coordinate space</param>
	/// <param name="h">Height of the coordinate space</param>
	void setViewSize(Uint32 w, Uint32 h);


	/// <summary>
	/// Add a character for access during scenes
	/// </summary>
	/// <param name="character">Character</param>
	void addCharacter(const Character& character);

	/// <summary>
	/// Get a character by name
	/// </summary>
	/// <param name="name">Name of character</param>
	/// <returns>Character object</returns>
	Character& getCharacter(const sf::String& name);

	/// <summary>
	/// Set a global game variable (i.e. to keep track of affection points)
	/// </summary>
	/// <param name="name">Name of the variable</param>
	/// <param name="val">Value of the variable</param>
	template <typename T>
	void setVariable(const sf::String& name, const T& val)
	{
		mVariables[name.toUtf32()] = val;
	}

	/// <summary>
	/// Get a reference to a game variable
	/// </summary>
	/// <param name="name">Name of the variable to retrieve</param>
	/// <returns>Reference to the variable</returns>
	template <typename T>
	T getVariable(const sf::String& name) const
	{
		auto it = mVariables.find(name.toUtf32());
		if (it != mVariables.end())
			return it->second;
		return T();
	}

	/// <summary>
	/// Returns true if variable exists or has been created
	/// </summary>
	/// <param name="name">Name of the variable</param>
	/// <returns>Boolean</returns>
	bool variableExists(const sf::String& name) const;

	/// <summary>
	/// Add a scene to the engine
	/// </summary>
	/// <param name="name">Name of scene</param>
	/// <param name="scene">Pointer to scene</param>
	void addScene(const sf::String& name, Scene* scene);

	/// <summary>
	/// Gets pointer to scene
	/// </summary>
	/// <param name="name">Name of scene</param>
	/// <returns>Pointer to scene</returns>
	Scene* getScene(const sf::String& name) const;

	/// <summary>
	/// Switches current scene
	/// </summary>
	/// <param name="name">Name of scene to switch to</param>
	void gotoScene(const sf::String& name);

	/// <summary>
	/// Get unordered map of scenes, scene name is mapped to scene pointer
	/// </summary>
	/// <returns>The map of scenes</returns>
	std::unordered_map<std::basic_string<Uint32>, Scene*>& getSceneMap();


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
	/// The very first scene that is called, it sets up all resources,
	/// characters, variables, and other scenes.
	/// </summary>
	Scene* mSetupScene;

	/// <summary>
	/// Current scene
	/// </summary>
	Scene* mScene;

	/// <summary>
	/// This is where scenes are queued, so that scene switching doesn't occur in the middle of a game loop
	/// </summary>
	Scene* mNextScene;

	/// <summary>
	/// Main view used to rescale all renderables to fit in window
	/// </summary>
	sf::View mView;

	/// <summary>
	/// Map of game characters
	/// </summary>
	std::unordered_map<std::basic_string<Uint32>, Character> mCharacters;

	/// <summary>
	/// Map of global game variables
	/// </summary>
	std::unordered_map<std::basic_string<Uint32>, Variant> mVariables;

	/// <summary>
	/// Map of game scenes
	/// </summary>
	std::unordered_map<std::basic_string<Uint32>, Scene*> mScenes;
};

// ============================================================================

}

#endif
