#ifndef UI_ANIMATION_H
#define UI_ANIMATION_H

// ============================================================================

template <typename T>
class UIAnimation
{
public:
	UIAnimation();
	~UIAnimation();

private:
	float mTime;
	bool mRepeat;
};

// ============================================================================

#endif