#ifndef UI_ANIMATION_H
#define UI_ANIMATION_H

#include <SFML/System.hpp>

#include <functional>

namespace vne
{

// ============================================================================

/// <summary>
/// Interpolate linearly between _1 and _2, using factor
/// </summary>
/// <param name="_1">The first value used for interpolation</param>
/// <param name="_2">The second value used for interpolation</param>
/// <param name="factor">Factor of how much to mix between the two end points (0.0f - 1.0f)</param>
/// <returns>Interpolated value</returns>
inline float interpLinear(float _1, float _2, float factor)
{
	return _1 + factor * (_2 - _1);
}

/// <summary>
/// Interpolate between _1 and _2, using factor in a polynomial function.
/// The function is f(x) = x ^ a
/// </summary>
/// <param name="_1">The first value used for interpolation</param>
/// <param name="_2">The second value used for interpolation</param>
/// <param name="factor">Factor used in parabola function, which will become the interpolation factor. This will be clamped to 0.0f - 1.0f</param>
/// <param name="a">The first (a) value in the parabola equation (Default 2.0f)</param>
/// <returns>Interpolated value</returns>
inline float interpPolynomial(float _1, float _2, float factor, float a = 2.0f)
{
	return interpLinear(_1, _2, powf(factor, a));
}

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
		mDelay		(0.0f),
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
			if (mRepeat && mDuration != 0.0f)
				mTime = fmodf(mTime, mDuration) - mDelay;
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
	/// Set animation delay (How long to wait before actually starting the animation).
	/// This could be used create sequences of animations
	/// (i.e. create a jumping animation by adding a move up animation,
	/// followed by a move down element after a delay)
	/// </summary>
	/// <param name="delay">Delay in seconds</param>
	void setDelay(float delay)
	{
		mDelay = delay;
	}

	/// <summary>
	/// Set animation repeat flag.
	/// Animation will loop if this is true
	/// </summary>
	/// <param name="repeat">Repeat mode</param>
	void setRepeat(bool repeat)
	{
		mRepeat = repeat;
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
	/// Get animation delay in seconds
	/// </summary>
	/// <returns>Delay in seconds</returns>
	float getDelay() const
	{
		return mDelay;
	}

	/// <summary>
	/// Returns true if animation is completed
	/// </summary>
	/// <returns>Finished state</returns>
	bool isFinished() const
	{
		return mTime >= mDuration;
	}

	/// <summary>
	/// Reset time to 0 seconds.
	/// This does not immediately reset property values
	/// </summary>
	void reset()
	{
		mTime = -mDelay;
	}

	/// <summary>
	/// Finish animation by setting time value to animation duration.
	/// This does not affect repeat mode, and will not immediately update property value
	/// </summary>
	void finish()
	{
		mTime = mDuration;
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
	/// Animation delay (to create sequences)
	/// </summary>
	float mDelay;

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

	UIAnimation(const PropSetter& setter, const T& s, const T& e) :
		mSetter		(setter),
		mStart		(s),
		mEnd		(e)
	{ }

	/// <summary>
	/// Set animation property setter function
	/// </summary>
	/// <param name="setter">Function object</param>
	void setPropSetter(const PropSetter& setter)
	{
		mSetter = setter;
	}

	/// <summary>
	/// Set starting value for animation
	/// </summary>
	/// <param name="val">Starting value</param>
	void setStartValue(const T& val)
	{
		mStart = val;
	}

	/// <summary>
	/// Set ending value for animation
	/// </summary>
	/// <param name="val">Ending value</param>
	void setEndValue(const T& val)
	{
		mEnd = val;
	}

	/// <summary>
	/// Cancel animation and reset property values
	/// </summary>
	void cancel()
	{
		mRepeat = false;
		mDuration = 0.0f;
		mTime = 0.0f;

		// Reset values
		mSetter(mStart);
	}

protected:
	/// <summary>
	/// Function object for property setter
	/// </summary>
	PropSetter mSetter;

	/// <summary>
	/// Starting value for animation
	/// </summary>
	T mStart;

	/// <summary>
	/// Ending value for animation
	/// </summary>
	T mEnd;
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
	/// <param name="setter">The setter function of the property you want to animate</param>
	/// <param name="s">Starting value of the property you want to animate</param>
	/// <param name="e">Ending value of the property you want to animate</param>
	/// <param name="duration">Duration of animation</param>
	FloatAnimation(const PropSetter& setter, float s, float e, float duration) :
		UIAnimation		(setter, s, e),
		mPowFactor		(1.0f)
	{
		mDuration = duration;
	}

	/// <summary>
	/// Apply linear transformation to vector based on current time
	/// </summary>
	/// <param name="dt"></param>
	void update(float dt) override
	{
		// Update time
		UIAnimation<float>::update(dt);

		if (mTime >= 0.0f)
		{
			// Linear interpolation
			float factor = mTime / mDuration;
			float val = interpPolynomial(mStart, mEnd, factor, mPowFactor);

			// Set new value
			if (mSetter)
				mSetter(val);
		}
	}

	/// <summary>
	/// Set the power factor that is applied to the time interpolation factor.
	/// This can be used to create nonlinear animation transitions (i.e. can be used to create a gravity effect).
	/// Applied to interpolation factor using this function: f(x) = x ^ a, where x is the interpolation factor.
	/// (Default 1.0f)
	/// </summary>
	/// <param name="factor">Power factor</param>
	void setPowFactor(float factor)
	{
		mPowFactor = factor;
	}

protected:
	/// <summary>
	/// The power factor that is applied to time factor
	/// </summary>
	float mPowFactor;
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
	/// <param name="setter">The setter function of the property you want to animate</param>
	/// <param name="s">Starting value of the property you want to animate</param>
	/// <param name="e">Ending value of the property you want to animate</param>
	/// <param name="duration">Duration of animation</param>
	Vec2Animation(const PropSetter& setter, const sf::Vector2f& s, const sf::Vector2f& e, float duration) :
		UIAnimation		(setter, s, e),
		mPowFactor		(1.0f)
	{
		mDuration = duration;
	}

	/// <summary>
	/// Apply linear transformation to vector based on current time
	/// </summary>
	/// <param name="dt"></param>
	void update(float dt) override
	{
		// Update time
		UIAnimation<sf::Vector2f>::update(dt);

		if (mTime >= 0.0f)
		{
			// Linear interpolation
			sf::Vector2f val;
			float factor = mTime / mDuration;
			val.x = interpPolynomial(mStart.x, mEnd.x, factor, mPowFactor);
			val.y = interpPolynomial(mStart.y, mEnd.y, factor, mPowFactor);

			// Set new value
			if (mSetter)
				mSetter(val);
		}
	}

	/// <summary>
	/// Set the power factor that is applied to the time interpolation factor.
	/// This can be used to create nonlinear animation transitions (i.e. can be used to create a gravity effect).
	/// Applied to interpolation factor using this function: f(x) = x ^ a, where x is the interpolation factor.
	/// (Default 1.0f)
	/// </summary>
	/// <param name="factor">Power factor</param>
	void setPowFactor(float factor)
	{
		mPowFactor = factor;
	}

protected:
	/// <summary>
	/// The power factor that is applied to time factor
	/// </summary>
	float mPowFactor;
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
	/// <param name="setter">The setter function of the property you want to animate</param>
	/// <param name="s">Starting value of the property you want to animate</param>
	/// <param name="e">Ending value of the property you want to animate</param>
	/// <param name="duration">Duration of animation</param>
	ColorAnimation(const PropSetter& setter, const sf::Color& s, const sf::Color& e, float duration) :
		UIAnimation		(setter, s, e),
		mPowFactor		(1.0f)
	{
		mDuration = duration;
	}

	/// <summary>
	/// Apply linear transformation to vector based on current time
	/// </summary>
	/// <param name="dt"></param>
	void update(float dt) override
	{
		// Update time
		UIAnimation<sf::Color>::update(dt);


		if (mTime >= 0.0f)
		{
			// Linear interpolation
			sf::Color val;
			float factor = mTime / mDuration;
			val.r = (Uint8)interpPolynomial(mStart.r, mEnd.r, factor, mPowFactor);
			val.g = (Uint8)interpPolynomial(mStart.g, mEnd.g, factor, mPowFactor);
			val.b = (Uint8)interpPolynomial(mStart.b, mEnd.b, factor, mPowFactor);
			val.a = (Uint8)interpPolynomial(mStart.a, mEnd.a, factor, mPowFactor);

			// Set new value
			if (mSetter)
				mSetter(val);
		}
	}

	/// <summary>
	/// Set the power factor that is applied to the time interpolation factor.
	/// This can be used to create nonlinear animation transitions (i.e. can be used to create a gravity effect).
	/// Applied to interpolation factor using this function: f(x) = x ^ a, where x is the interpolation factor.
	/// (Default 1.0f)
	/// </summary>
	/// <param name="factor">Power factor</param>
	void setPowFactor(float factor)
	{
		mPowFactor = factor;
	}

protected:
	/// <summary>
	/// The power factor that is applied to time factor
	/// </summary>
	float mPowFactor;
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