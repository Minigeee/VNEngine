#include <vld.h>

#include <Engine/Engine.h>

#include <Resource/ResourceMgr.h>

#include <iostream>

class A : public Loadable
{
public:
	bool Load(Uint8* data, Uint32 size) override
	{
		std::cout << (char*)data << "\n";
		free(data);

		mIsLoaded = true;

		return true;
	}

	void Free() override
	{
		std::cout << "Free Resource\n";

		mIsLoaded = false;
	}
};

int main()
{
	ResourceFolder::SetPath(".");
	ResourceMgr<A>::AddLocation("test.txt", "Test");
	A* test = ResourceMgr<A>::Get("Test");
	ResourceMgr<A>::Free("Test");

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