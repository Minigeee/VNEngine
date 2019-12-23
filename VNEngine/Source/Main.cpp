#include <vld.h>

#include <Engine/Engine.h>

int main()
{
	EngineParams params;
	params.mWindowWidth = 1280;
	params.mWindowHeight = 720;
	params.mWindowTitle = "VN Demo";
	params.mFullscreen = false;

	Engine engine;
	bool success = engine.Init(params);
	if (success)
		engine.Run();

	return 0;
}