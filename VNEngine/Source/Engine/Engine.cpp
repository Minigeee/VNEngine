#include <Engine/Engine.h>

// ============================================================================
// ============================================================================

Engine::Engine()
{

}

Engine::~Engine()
{

}

// ============================================================================
// ============================================================================

bool Engine::Init(const EngineParams& params)
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


	return true;
}

// ============================================================================
// ============================================================================

void Engine::Run()
{
	sf::Clock clock;

	// Game loop
	while (mWindow.isOpen())
	{
		// Handle input
		PollEvents();

		// Do updates
		Update(clock.restart().asSeconds());

		// Render scene
		Render();
	}
}

// ============================================================================

void Engine::PollEvents()
{
	sf::Event e;

	while (mWindow.pollEvent(e))
	{
		// Handle window close
		if (e.type == sf::Event::Closed)
			mWindow.close();
	}
}

// ============================================================================

void Engine::Update(float dt)
{

}

// ============================================================================

void Engine::Render()
{
	// Clear screen
	mWindow.clear();

	// Render stuff

	// Swap buffers
	mWindow.display();
}

// ============================================================================
// ============================================================================

void Engine::Close()
{
	mWindow.close();
}

// ============================================================================
// ============================================================================



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