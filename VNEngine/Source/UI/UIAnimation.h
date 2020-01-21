#ifndef UI_ANIMATION_H
#define UI_ANIMATION_H

#include <SFML/System.hpp>

#include <functional>

namespace vne
{

// ============================================================================

/// <summary>
/// Base animation class
/// </summary>
class I_UIAnimation
{
public:
	I_UIAnimation() :
		mTime		(0.0f),
		mDuration	(1.0f),
		mRepeat		(false)
	{ }

	virtual ~I_UIAnimation() { }

	/// <summary>
	/// Update animation
	/// </summary>
	/// <param name="dt">Elapsed time</param>
	virtual void update(float dt)
	{
		mTime += dt;

		// Loop time if needed
		if (mTime > mDuration)
		{
			if (mRepeat)
				mTime = fmodf(mTime, mDuration);
			else
				mTime = mDuration;
		}
	}

	/// <summary>
	/// Set animation duration in seconds (Default 1.0f)
	/// </summary>
	/// <param name="duration">Duration</param>
	void setDuration(float duration)
	{
		mDuration = duration;
	}

	/// <summary>
	/// Get animation duration in seconds
	/// </summary>
	/// <returns>Duration in seconds</returns>
	float getDuration() const
	{
		return mDuration;
	}

	/// <summary>
	/// Returns true if animation is completed
	/// </summary>
	/// <returns>Finished state</returns>
	bool isFinished() const
	{
		return mTime >= mDuration;
	}

protected:
	/// <summary>
	/// Current time in animation loop
	/// </summary>
	float mTime;

	/// <summary>
	/// Animation duration
	/// </summary>
	float mDuration;

	/// <summary>
	/// True if animation should repeat
	/// </summary>
	bool mRepeat;
};

// ============================================================================

template <typename T>
class UIAnimation : public I_UIAnimation
{
public:
	/// <summary>
	/// Property setter (The function that points to the property the animation is modifying)
	/// </summary>
	typedef std::function<void(const T&)> PropSetter;

public:
	UIAnimation() { }
	virtual ~UIAnimation() { }

	/// <summary>
	/// Set animation property setter function
	/// </summary>
	/// <param name="setter">Function object</param>
	void setPropSetter(const PropSetter& setter)
	{
		mSetter = setter;
	}

protected:
	/// <summary>
	/// Function object for property setter
	/// </summary>
	PropSetter mSetter;
};

// ============================================================================

/// <summary>
/// Animation for float properties in UIElements (i.e. rotation).
/// This animation applies linear transformation
/// </summary>
class FloatAnimation : public UIAnimation<float>
{
public:
	FloatAnimation() { }

	/// <summary>
	/// Initialize animation with given parameters
	/// </summary>
	/// <param name="s">Starting value of the animation</param>
	/// <param name="e">Ending value of the animation</param>
	/// <param name="duration">Duration of animation</param>
	FloatAnimation(const PropSetter& setter, float s, float e, float duration) :
		mStart	(s),
		mEnd	(e)
	{
		mDuration = duration;
		mSetter = setter;
	}

	/// <summary>
	/// Apply linear transformation to vector based on current time
	/// </summary>
	/// <param name="dt"></param>
	void update(float dt) override
	{
		// Update time
		UIAnimation<float>::update(dt);

		// Linear interpolation
		float factor = mTime / mDuration;
		float val = mStart + factor * (mEnd - mStart);

		// Set new value
		if (mSetter)
			mSetter(val);
	}

protected:
	/// <summary>
	/// Start value of animation
	/// </summary>
	float mStart;

	/// <summary>
	/// End value of animation
	/// </summary>
	float mEnd;
};

// ============================================================================

/// <summary>
/// Animation for Vector2f properties in UIElements (i.e. position, scale).
/// This animation applies linear transformation
/// </summary>
class Vec2Animation : public UIAnimation<sf::Vector2f>
{
public:
	Vec2Animation() { }

	/// <summary>
	/// Initialize animation with given parameters
	/// </summary>
	/// <param name="s">Starting value of the animation</param>
	/// <param name="e">Ending value of the animation</param>
	/// <param name="duration">Duration of animation</param>
	Vec2Animation(const PropSetter& setter, const sf::Vector2f& s, const sf::Vector2f& e, float duration) :
		mStart		(s),
		mEnd		(e)
	{
		mDuration = duration;
		mSetter = setter;
	}

	/// <summary>
	/// Apply linear transformation to vector based on current time
	/// </summary>
	/// <param name="dt"></param>
	void update(float dt) override
	{
		// Update time
		UIAnimation<sf::Vector2f>::update(dt);

		// Linear interpolation
		sf::Vector2f val;
		float factor = mTime / mDuration;
		val.x = mStart.x + factor * (mEnd.x - mStart.x);
		val.y = mStart.y + factor * (mEnd.y - mStart.y);

		// Set new value
		if (mSetter)
			mSetter(val);
	}

protected:
	/// <summary>
	/// Start value of animation
	/// </summary>
	sf::Vector2f mStart;

	/// <summary>
	/// End value of animation
	/// </summary>
	sf::Vector2f mEnd;
};

// ============================================================================

/// <summary>
/// Animation for color properties in UIElements.
/// This animation applies linear transformation
/// </summary>
class ColorAnimation : public UIAnimation<sf::Color>
{
public:
	ColorAnimation() { }

	/// <summary>
	/// Initialize animation with given parameters
	/// </summary>
	/// <param name="s">Starting value of the animation</param>
	/// <param name="e">Ending value of the animation</param>
	/// <param name="duration">Duration of animation</param>
	ColorAnimation(const PropSetter& setter, const sf::Color& s, const sf::Color& e, float duration) :
		mStart	(s),
		mEnd	(e)
	{
		mDuration = duration;
		mSetter = setter;
	}

	/// <summary>
	/// Apply linear transformation to vector based on current time
	/// </summary>
	/// <param name="dt"></param>
	void update(float dt) override
	{
		// Update time
		UIAnimation<sf::Color>::update(dt);

		// Linear interpolation
		sf::Color val;
		float factor = mTime / mDuration;
		val.r = (Uint8)(mStart.r + factor * (mEnd.r - mStart.r));
		val.g = (Uint8)(mStart.g + factor * (mEnd.g - mStart.g));
		val.b = (Uint8)(mStart.b + factor * (mEnd.b - mStart.b));
		val.a = (Uint8)(mStart.a + factor * (mEnd.a - mStart.a));

		// Set new value
		if (mSetter)
			mSetter(val);
	}

protected:
	/// <summary>
	/// Start value of animation
	/// </summary>
	sf::Color mStart;

	/// <summary>
	/// End value of animation
	/// </summary>
	sf::Color mEnd;
};

// ============================================================================

#include <Core/Macros.h>

#define ANIMATE_POS(element, start_val, end_val, duration) \
new Vec2Animation( \
	std::bind( \
		static_cast<void(std::remove_pointer<decltype(element)>::type::*)(const sf::Vector2f&)>(&std::remove_pointer<decltype(element)>::type::setPosition), \
	element, std::placeholders::_1), \
	start_val, end_val, duration)

#define ANIMATE_ROT(element, start_val, end_val, duration) \
new FloatAnimation( \
	std::bind( \
		static_cast<void(std::remove_pointer<decltype(element)>::type::*)(float)>(&std::remove_pointer<decltype(element)>::type::setRotation), \
	element, std::placeholders::_1), \
	start_val, end_val, duration)

#define ANIMATE_SIZE(element, start_val, end_val, duration) \
new Vec2Animation( \
	std::bind( \
		static_cast<void(std::remove_pointer<decltype(element)>::type::*)(const sf::Vector2f&)>(&std::remove_pointer<decltype(element)>::type::setSize), \
	element, std::placeholders::_1), \
	start_val, end_val, duration)

#define ANIMATE_FLOAT(element, prop_name, start_val, end_val, duration) \
new FloatAnimation( \
	std::bind( \
		static_cast<void(std::remove_pointer<decltype(element)>::type::*)(float)>(CONCAT(&std::remove_pointer<decltype(element)>::type::set, prop_name)), \
	element, std::placeholders::_1), \
	start_val, end_val, duration)

#define ANIMATE_VEC2(element, prop_name, start_val, end_val, duration) \
new Vec2Animation( \
	std::bind( \
		static_cast<void(std::remove_pointer<decltype(element)>::type::*)(const sf::Vector2f&)>(CONCAT(&std::remove_pointer<decltype(element)>::type::set, prop_name)), \
	element, std::placeholders::_1), \
	start_val, end_val, duration)

#define ANIMATE_COLOR(element, prop_name, start_val, end_val, duration) \
new ColorAnimation( \
	std::bind( \
		static_cast<void(std::remove_pointer<decltype(element)>::type::*)(const sf::Color&)>(CONCAT(&std::remove_pointer<decltype(element)>::type::set, prop_name)), \
	element, std::placeholders::_1), \
	start_val, end_val, duration)

// ============================================================================

}

#endif