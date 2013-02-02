#ifndef Vector2_HPP__
#define Vector2_HPP__

#include "cinder/gl/gl.h"

template<typename T>
T clamp(T v, T low, T high) {
	if ( v <= low )
		return low;
	if ( v >= high )
		return high;
	return v;
}

struct Vector2 {
	float x, y;
	
	Vector2() {}
	Vector2(float x, float y) : x(x), y(y) {}
	
	Vector2 &operator+=(const Vector2 &rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	
	Vector2 &operator-=(const Vector2 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	
	Vector2 operator+(const Vector2 &rhs) const {
		return Vector2(x + rhs.x, y + rhs.y);
	}
	
	Vector2 operator-(const Vector2 &rhs) const {
		return Vector2(x - rhs.x, y - rhs.y);
	}
	float operator*(const Vector2 &rhs) const {
		return x * rhs.x + y * rhs.y;
	}
	
	Vector2 operator*(float s) const {
		return Vector2(s * x, s * y);
	}
	
	Vector2 operator-() const {
		return Vector2(-x, -y);
	}
	
	Vector2 perp() const {
		return Vector2(-y, x);
	}
	float sqMag() const { return x * x + y * y; }
	
	float mag() const { return sqrt(sqMag()); }
	
	void normalize() {
		float d = mag();
		if ( mag() < 0.00001 ) return;
		d = 1.0 / d;
		
		x *= d;
		y *= d;
	}
	ci::Vec2f toV() const {
		return ci::Vec2f(x, y);
	}
};

Vector2 operator*(float s, const Vector2 &rhs) {
	return Vector2(rhs.x * s, rhs.y * s);
}

#endif