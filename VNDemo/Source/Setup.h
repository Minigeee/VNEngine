#ifndef VN_DEMO_SETUP_SCENE
#define VN_DEMO_SETUP_SCENE

#include <Engine/Scene.h>

// ============================================================================

class Setup : public vne::SetupScene
{
public:
	Setup(vne::Engine* engine);

private:
	void onInit() override;
	void gotoFirstScene() override;
};

// ============================================================================

#endif