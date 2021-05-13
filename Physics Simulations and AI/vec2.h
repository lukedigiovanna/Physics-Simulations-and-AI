#pragma once

struct vec2 {
public:
	float x, y;

	vec2(float x, float y) : x(x), y(y) {}
	vec2() : x(0), y(0) {}

	vec2 operator+(const vec2& rhs) { return vec2(this->x + rhs.x, this->y + rhs.y); }
	vec2 operator-(const vec2& rhs) { return vec2(this->x - rhs.x, this->y - rhs.y); }
	vec2 operator*(const float rhs) { return vec2(this->x * rhs, this->y * rhs); }
	vec2 operator*(const vec2& rhs) { return vec2(this->x * rhs.x, this->y * rhs.y); }
	vec2 operator/(const float rhs) { return vec2(this->x / rhs, this->y / rhs); }
	vec2 operator/(const vec2& rhs) { return vec2(this->x / rhs.x, this->y / rhs.y); }
	
	void operator=(const vec2& rhs) { this->x = rhs.x; this->y = rhs.y; }

	void operator+=(const vec2& rhs) { this->x += rhs.x; this->y += rhs.y; }
	void operator-=(const vec2& rhs) { this->x -= rhs.x; this->y -= rhs.y; }
	void operator*=(const float rhs) { this->x *= rhs; this->y *= rhs; }
	void operator/=(const float rhs) { this->x /= rhs; this->y /= rhs; }

	float squaredMagnitude() {
		return this->x * this->x + this->y * this->y;
	}
};