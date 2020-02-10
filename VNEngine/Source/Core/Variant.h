#ifndef VARIANT_H
#define VARIANT_H

#include <Core/DataTypes.h>

namespace vne
{

// ============================================================================

class Variant
{
public:
	enum Type
	{
		/// <summary>
		/// Boolean
		/// </summary>
		Bool,

		/// <summary>
		/// Signed 8-bit character
		/// </summary>
		Char,

		/// <summary>
		/// Signed 32-bit integer
		/// </summary>
		Int,

		/// <summary>
		/// Unsigned 32-bit integer
		/// </summary>
		Uint,

		/// <summary>
		/// Single precision float
		/// </summary>
		Float,

		/// <summary>
		/// Double precision float
		/// </summary>
		Double,

		/// <summary>
		/// 2D floating point vector
		/// </summary>
		Vec2,

		/// <summary>
		/// 3D floating point vector
		/// </summary>
		Vec3,

		/// <summary>
		/// Other unknown data type
		/// </summary>
		Unknown
	};

public:
	Variant() { memset(mData, 0, 64); }

	template <typename T>
	Variant(const T& val) { *this = val; }

	template <typename T>
	T& operator=(const T& val)
	{
		mType = Unknown;
		memset(mData, 0, 64);
		return (*reinterpret_cast<T*>(mData) = val);
	}

	template <> bool& operator=(const bool& val)
	{
		mType = Bool;
		memset(mData, 0, 64);
		return (*reinterpret_cast<bool*>(mData) = val);
	}

	template <> char& operator=(const char& val)
	{
		mType = Char;
		memset(mData, 0, 64);
		return (*reinterpret_cast<char*>(mData) = val);
	}

	template <> int& operator=(const int& val)
	{
		mType = Int;
		memset(mData, 0, 64);
		return (*reinterpret_cast<int*>(mData) = val);
	}

	template <> Uint32& operator=(const Uint32& val)
	{
		mType = Uint;
		memset(mData, 0, 64);
		return (*reinterpret_cast<Uint32*>(mData) = val);
	}

	template <> float& operator=(const float& val)
	{
		mType = Float;
		memset(mData, 0, 64);
		return (*reinterpret_cast<float*>(mData) = val);
	}

	template <> double& operator=(const double& val)
	{
		mType = Double;
		memset(mData, 0, 64);
		return (*reinterpret_cast<double*>(mData) = val);
	}

	template <> sf::Vector2f& operator=(const sf::Vector2f& val)
	{
		mType = Vec2;
		memset(mData, 0, 64);
		return (*reinterpret_cast<sf::Vector2f*>(mData) = val);
	}

	template <> sf::Vector3f& operator=(const sf::Vector3f& val)
	{
		mType = Vec3;
		memset(mData, 0, 64);
		return (*reinterpret_cast<sf::Vector3f*>(mData) = val);
	}

	template <typename T>
	operator T() const { return *reinterpret_cast<const T*>(mData); }

	bool operator==(const Variant& other) { return memcmp(mData, other.mData, 64) == 0; }
	bool operator!=(const Variant& other) { return memcmp(mData, other.mData, 64) != 0; }

	Type getType() const { return mType; }

private:
	/// <summary>
	/// Variant type
	/// </summary>
	Type mType;

	/// <summary>
	/// Buffer for data
	/// </summary>
	Uint8 mData[64];
};

// ============================================================================

}

#endif