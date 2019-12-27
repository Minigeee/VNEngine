#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <SFML/System.hpp>

namespace vne
{

// ============================================================================

typedef sf::Int8		Int8;
typedef sf::Uint8		Uint8;

typedef sf::Int16		Int16;
typedef sf::Uint16		Uint16;

typedef sf::Int32		Int32;
typedef sf::Uint32		Uint32;

typedef sf::Int64		Int64;
typedef sf::Uint64		Uint64;

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


}

#endif