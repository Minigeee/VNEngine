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

int main()
{
	ResourceFolder::SetKey(gResourceKey);

	if (false)
	{
		ResourceFolder::SetPath("TestAssets");
		ResourceFolder::Pack("resources.assets");
	}
	else
	{
		ResourceFolder::SetPath("resources.assets");

		ResourceMgr<A>::AddLocation("Shaders/Default.frag", "Test");
		A* test = ResourceMgr<A>::Get("Test");
	}

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