#include <vld.h>

#include <Engine/Engine.h>
#include <Engine/Resource.h>

#include <Scene1.h>

#include <iostream>

using namespace vne;

// ============================================================================

Uint8 gResourceKey[] =
{
	0x37,
	0x77,
	0x21,
	0x7A,
	0x24,
	0x43,
	0x26,
	0x46,
	0x29,
	0x4A,
	0x40,
	0x4E,
	0x63,
	0x52,
	0x66,
	0x55
};

// ============================================================================

int main()
{
	ResourceFolder::setKey(gResourceKey);
	ResourceFolder::setPath("TestAssets");

	Engine engine;
	Scene1 scene(&engine);

	EngineParams params;
	params.mWindowWidth = 1280;
	params.mWindowHeight = 720;
	params.mWindowTitle = "VN Demo";
	params.mFullscreen = false;
	params.mStartScene = &scene;

	bool success = engine.init(params);
	if (success)
		engine.run();

	return 0;
}