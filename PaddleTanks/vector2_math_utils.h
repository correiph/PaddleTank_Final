#pragma once
#include <cmath>
#include <limits>

/// <summary>
/// A collection of useful functions for dealing with vector2s. Note that
/// these functions are compatible with any Vector2 implementation that
/// has x and y component named "x" and "y".
/// </summary>
namespace vec2utils {

	//Explicitly convert between two different types of vectors.
	template<typename T, typename U>
	inline U ConvertVectorType(T const &v) {
		return U(v.x, v.y);
	}

	//Calculate the length (magnitude) of the vector.
	template<typename T, typename U = float>
	inline U Length(T const &v) {
		return std::sqrt((v.x * v.x) + (v.y*v.y));
	}

	//Calculate the length squared (magnitude squared) of the vector.
	template<typename T, typename U = float>
	inline U LengthSq(T const &v) {
		return (v.x * v.x) + (v.y*v.y);
	}

	//Calculates the dot product between two vectors.
	// v1 * v2
	template<typename T, typename U = float>
	inline U Dot(T const &v1, T const &v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	//Calculates a vector perpendicular to a vector. Beware of
	// handedness when using.
	template<typename T>
	inline T PerpTo(T const &v) {
		return T(-v.y, v.x);
	}

	//Calculates the normal vector to a vector.
	template<typename T>
	inline T NormalOf(T const &v) {
		T t(v);
		float l = Length(v);
		if (l > std::numeric_limits::epsilon()) {
			t.x /= l;
			t.y /= l;
		}
		else {
			t.x = 0;
			t.y = 0;
		}
		return t;
	}

	//Calculates the normal vector to a vector.
	// Used when T is an integer type vector
	// that cannot express a normalised vector
	// value.
	template<typename T, typename U>
	inline U NormalOf(T const &v) {
		U u = ConvertVectorType<T, U>(v);
		float l = Length(v);
		if (l > std::numeric_limits<float>::epsilon()) {
			u.x /= l;
			u.y /= l;
		}
		else {
			u.x = 0;
			u.y = 0;
		}
		return u;
	}


	//Note if vectors are parallel returns clockwise
	enum { clockwise = 1, counterclockwise = -1 };
	template<typename T>
	inline int Sign(T const &v1, T const &v2) {
		return (v1.y * v2.x) > (v1.x * v2.y) ? counterclockwise : clockwise;
	}

	


}