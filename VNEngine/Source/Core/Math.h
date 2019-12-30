#ifndef MATH_FUNCS_H
#define MATH_FUNCS_H

#include <SFML/System.hpp>

// ============================================================================

namespace vne
{

inline float toRadians(float angle)
{
	return angle * 3.141592654f / 180.f;
}

inline float toDegrees(float angle)
{
	return angle * 180.0f / 3.141592654f;
}

}

// ============================================================================

template <typename T>
inline sf::Vector2<T> operator*(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
{
	return sf::Vector2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
inline sf::Vector2<T>& operator*=(sf::Vector2<T>& a, const sf::Vector2<T>& b)
{
	a = a * b;
	return a;
}

template <typename T>
inline sf::Vector2<T> operator/(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
{
	return sf::Vector2<T>(a.x / b.x, a.y / b.y);
}

template <typename T>
inline sf::Vector2<T>& operator/=(sf::Vector2<T>& a, const sf::Vector2<T>& b)
{
	a = a / b;
	return a;
}

// ============================================================================

template <typename T>
inline float length(const sf::Vector2<T>& x)
{
	return static_cast<float>(sqrt(x.x * x.x + x.y * x.y));
}

template <typename T>
inline float lengthSquared(const sf::Vector2<T>& x)
{
	return static_cast<float>(x.x * x.x + x.y * x.y);
}

template <typename T>
inline sf::Vector2<T> normalize(const sf::Vector2<T>& x)
{
	float len = length(x);
	return sf::Vector2<T>(x.x / len, x.y / len);
}

// ============================================================================

#endif