#include <vld.h>

#include <Engine/Engine.h>

#include <Resource/ResourceMgr.h>

#include <iostream>

class A : public Resource
{
public:
	void Free() override
	{
		std::cout << "Free Resource\n";
	}
};

int main()
{
	A* test = ResourceMgr<A>::Create("Test");
	ResourceMgr<A>::Free("Test");
	int a = 0;

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