#include <vld.h>

#include <Engine/Engine.h>

#include <Resource/ResourceMgr.h>

#include <iostream>

using namespace vne;

// ============================================================================

class A : public Loadable
{
public:
	bool load(Uint8* data, Uint32 size) override
	{
		std::cout << (char*)data << "\n";
		std::free(data);

		mIsLoaded = true;

		return true;
	}

	void free() override
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

// ============================================================================

int main()
{
	ResourceFolder::setKey(gResourceKey);
	ResourceFolder::setPath("TestAssets");

	EngineParams params;
	params.mWindowWidth = 1280;
	params.mWindowHeight = 720;
	params.mWindowTitle = "VN Demo";
	params.mFullscreen = false;

	Engine engine;
	bool success = engine.init(params);
	if (success)
		engine.run();

	return 0;
}