#ifndef SCENE_1_H
#define SCENE_1_H

#include <Engine/Scene.h>

// ============================================================================

class Scene1 : public vne::NovelScene
{
public:
	Scene1(vne::Engine* engine);
	~Scene1();

private:
	void onInit() override;
	void onUpdate(float dt) override;
};

// ============================================================================

#endif