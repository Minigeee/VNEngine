#include <Engine/Engine.h>
#include <Engine/Scene.h>
#include <Engine/Resource.h>

using namespace vne;

// ============================================================================
// ============================================================================

Engine::Engine() :
	mScene			(0),
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

	// Main view
	mWindow.setView(mView);


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

	// Free all SFML resources
	Resource<sf::Texture>::free();
}

// ============================================================================

#include <iostream>

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