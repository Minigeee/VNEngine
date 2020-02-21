#include <Engine/Engine.h>
#include <Engine/Scene.h>
#include <Engine/Resource.h>
#include <Engine/Cursor.h>

using namespace vne;

// ============================================================================
// ============================================================================

Engine::Engine() :
	mSetupScene		(0),
	mScene			(0),
	mNextScene		(0),
	mView			(sf::FloatRect(0.0f, 0.0f, 1920.0f, 1080.0f))
{

}

Engine::~Engine()
{

}

// ============================================================================
// ============================================================================

bool Engine::init(const EngineParams& params)
{
	// Set window style based on params
	sf::Uint32 style = sf::Style::Default;
	sf::VideoMode vmode(params.mWindowWidth, params.mWindowHeight);

	// Fullscreen
	if (params.mFullscreen)
	{
		style = sf::Style::Fullscreen;
		// Use fullscreen resolution
		vmode = sf::VideoMode::getDesktopMode();
	}

	// Resizeable
	if (!params.mResizable)
		style = style & ~sf::Style::Resize;

	// Enable anti-aliasing
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	// Create window
	mWindow.create(
		vmode,
		params.mWindowTitle,
		style,
		settings
	);
	// Enable v-sync
	mWindow.setVerticalSyncEnabled(true);

	// Main view
	mWindow.setView(mView);

	// Initialize cursors
	Cursor::init(&mWindow);


	// Setup scene
	mSetupScene = params.mSetupScene;
	if (!mSetupScene) return false;
	mSetupScene->init();

	// Make sure the setup scene set a default font
	if (!mFont) return false;

	return true;
}

// ============================================================================
// ============================================================================

void Engine::run()
{
	// Set next scene to current scene, and reset
	mScene = mNextScene;
	mScene->init();
	mNextScene = 0;


	// Poll events once before starting
	pollEvents();

	sf::Clock clock;

	// Game loop
	while (mWindow.isOpen())
	{
		// If scene switch is requested, then switch scenes
		if (mNextScene)
		{
			// Cleanup old scene
			if (mScene)
				mScene->cleanup();

			mScene = mNextScene;
			mNextScene = 0;

			// Initialize new scene
			mScene->init();
		}

		// Handle input
		pollEvents();

		// Do updates
		update(clock.restart().asSeconds());

		// Render scene
		render();
	}

	// Free all SFML resources
	Resource<sf::Texture>::free();
	Resource<sf::Font>::free();
	Resource<sf::SoundBuffer>::free();
	Resource<sf::Music>::free();
}

// ============================================================================

void Engine::pollEvents()
{
	sf::Event e;

	while (mWindow.pollEvent(e))
	{
		// Handle window close
		if (e.type == sf::Event::Closed)
			mWindow.close();

		// Handle window resize
		else if (e.type == sf::Event::Resized)
		{
			float ARs = (float)e.size.width / e.size.height;
			const sf::Vector2f& viewSize = mView.getSize();
			float ARc = viewSize.x / viewSize.y;

			float w, h, x, y;
			if (ARs >= ARc)
			{
				// Width is relatively bigger
				w = ARc / ARs;
				h = 1.0f;
				x = (1.0f - w) * 0.5f;
				y = 0.0f;
			}
			else
			{
				// Height is relatively bigger
				w = 1.0f;
				h = ARs / ARc;
				x = 0.0f;
				y = (1.0f - h) * 0.5f;
			}

			// Apply view
			mView.setViewport(sf::FloatRect(x, y, w, h));
			mWindow.setView(mView);
		}

		mScene->handleEvent(e);
	}
}

// ============================================================================

void Engine::update(float dt)
{
	mScene->update(dt);
}

// ============================================================================

void Engine::render()
{
	// Clear screen
	mWindow.clear();

	// Render stuff
	mScene->render();

	// Swap buffers
	mWindow.display();
}

// ============================================================================

void Engine::close()
{
	mWindow.close();
}

// ============================================================================
// ============================================================================

void Engine::setDefaultFont(sf::Font* font)
{
	mFont = font;
}

sf::Font* Engine::getDefaultFont() const
{
	return mFont;
}

// ============================================================================
// ============================================================================

void Engine::addCharacter(const Character& character)
{
	mCharacters[character.getName().toUtf32()] = character;
}

Character& Engine::getCharacter(const sf::String& name)
{
	return mCharacters[name.toUtf32()];
}

bool Engine::variableExists(const sf::String& name) const
{
	return mVariables.find(name.toUtf32()) != mVariables.end();
}

void Engine::addScene(const sf::String& name, Scene* scene)
{
	mScenes[name.toUtf32()] = scene;
}

Scene* Engine::getScene(const sf::String& name) const
{
	auto it = mScenes.find(name.toUtf32());
	if (it != mScenes.end())
		return it->second;
	return 0;
}

void Engine::gotoScene(const sf::String& name)
{
	Scene* scene = getScene(name);
	if (scene)
		mNextScene = scene;
}

std::unordered_map<std::basic_string<Uint32>, Scene*>& Engine::getSceneMap()
{
	return mScenes;
}

// ============================================================================

sf::RenderWindow& Engine::getWindow()
{
	return mWindow;
}

// ============================================================================
// ============================================================================

EngineParams::EngineParams() :
	mWindowWidth		(1280),
	mWindowHeight		(720),
	mWindowTitle		("VN Game"),
	mFullscreen			(false),
	mResizable			(true),
	mSetupScene			(0)
{

}

// ============================================================================