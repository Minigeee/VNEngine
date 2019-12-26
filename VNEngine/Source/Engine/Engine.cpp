#include <Engine/Engine.h>
#include <Engine/Scene.h>

using namespace vne;

// ============================================================================
// ============================================================================

Engine::Engine() :
	mScene			(0)
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
	if (params.mFullscreen)
		style = sf::Style::Fullscreen;
	if (!params.mResizable)
		style = style & ~sf::Style::Resize;

	// Create window
	mWindow.create(
		sf::VideoMode(params.mWindowWidth, params.mWindowHeight),
		params.mWindowTitle,
		style
	);
	// Enable v-sync
	mWindow.setVerticalSyncEnabled(true);


	// Setup first scene
	mScene = params.mStartScene;
	mScene->init();


	return true;
}

// ============================================================================
// ============================================================================

void Engine::run()
{
	sf::Clock clock;

	// Game loop
	while (mWindow.isOpen())
	{
		// Handle input
		pollEvents();

		// Do updates
		update(clock.restart().asSeconds());

		// Render scene
		render();
	}
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
	mStartScene			(0)
{

}

// ============================================================================