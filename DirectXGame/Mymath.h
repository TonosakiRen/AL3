#pragma once
#define _USE_MATH_DEFINES
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <numbers>
#include <assert.h>
#include <cmath>

inline float clamp(float num, float min, float max) {
	if (num < min) {
		return min;
	}
	if (num > max) {
		return max;
	}
	return num;
}

inline bool closeValue(float& num, float goal,float speed) {
	if (std::fabs(num - goal) < std::fabs(speed)) {
		num = goal;
		return true;
	}
	if (num < goal) {
		num += speed;
	}
	if (num > goal) {
		num -= speed;
	}
	return false;
}

inline float Radian(float degree) { return degree * std::numbers::pi_v<float> / 180.0f; }

#pragma region Vector3
// Vetor3
// 加算
inline Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 tmp;
	tmp.x = v1.x + v2.x;
	tmp.y = v1.y + v2.y;
	tmp.z = v1.z + v2.z;
	return tmp;
}
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}
// 減算
inline Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 tmp;
	tmp.x = v1.x - v2.x;
	tmp.y = v1.y - v2.y;
	tmp.z = v1.z - v2.z;
	return tmp;
}
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

inline Vector3& operator-=(Vector3& v1, const Vector3& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}
// スカラー倍
inline Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 tmp;
	tmp.x = v.x * scalar;
	tmp.y = v.y * scalar;
	tmp.z = v.z * scalar;
	return tmp;
}
inline Vector3 operator*(const Vector3& v, const float& scalar) {
	return {v.x * scalar, v.y * scalar, v.z * scalar};
}
// 内積
inline float Dot(const Vector3& v1, const Vector3& v2) {
	return {v1.x * v2.x + v1.y * v2.y + v1.z * v2.z};
}
inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
	return {v1.x * v2.x + v1.y * v2.y + v1.z * v2.z};
}
// 長さ
inline float Length(const Vector3& v) {
	float tmp = v.x * v.x + v.y * v.y + v.z * v.z;
	tmp = sqrtf(tmp);
	return tmp;
}
// 正規化
inline Vector3 Normalize(const Vector3& v) {
	float tmp = v.x * v.x + v.y * v.y + v.z * v.z;
	Vector3 result = {0.0f,0.0f,0.0f};
	tmp = sqrtf(tmp);
	if (tmp != 0.0f) {
		result.x = v.x / tmp;
		result.y = v.y / tmp;
		result.z = v.z / tmp;
	}
	return {result};
}
// ベクトル変換
inline Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {

	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};

	return result;
}
static const int kColumnWidth = 60;
static const int kRowHeight = 20;

// 線形補間
inline Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result = v1 + (v2 - v1) * t;
	return result;
}
// 球面線形補間
inline Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {

	const float epsilon = 0.0001f;

	Vector3 a = Normalize(v1);
	Vector3 b = Normalize(v2);

	float dot = Dot(a, b);

	if (std::abs(dot - 1.0f) < epsilon) {
		return a;
	} else if (std::abs(dot + 1.0f) < epsilon) {
		return Lerp(v1, v2, t);
	}

	float theta = std::acos(dot);

	float sinTheta = std::sin(theta);
	float factorA = std::sin((1.0f - t) * theta) / sinTheta;
	float factorB = std::sin(t * theta) / sinTheta;

	return Vector3{
	    factorA * a.x + factorB * b.x, factorA * a.y + factorB * b.y,
	    factorA * a.z + factorB * b.z};
}

inline bool closeVector3(Vector3& num, Vector3 goal, float speed) {
	if ((std::fabs(num.x - goal.x) < std::fabs(speed)) &&
	    (std::fabs(num.y - goal.y) < std::fabs(speed)) &&
	    (std::fabs(num.z - goal.z) < std::fabs(speed))) {
		num = goal;
		return true;
	}
	
	if (std::fabs(num.x - goal.x) < std::fabs(speed)) {
		num.x = goal.x;
	}else
	if (num.x < goal.x) {
		num.x += speed;
	}else
	if (num.x > goal.x) {
		num.x -= speed;
	}

	if (std::fabs(num.y - goal.y) < std::fabs(speed)) {
		num.y = goal.y;
	} else
	if (num.y < goal.y) {
		num.y += speed;
	}else
	if (num.y > goal.y) {
		num.y -= speed;
	}

	if (std::fabs(num.z - goal.z) < std::fabs(speed)) {
		num.z = goal.z;
	} else
	if (num.z < goal.z) {
		num.z += speed;
	}else
	if (num.z > goal.z) {
		num.z -= speed;
	}
	return false;
}

#pragma endregion
#pragma region Matrix3x3
inline Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 tmp;
	tmp.m[0][0] = m1.m[0][0] + m2.m[0][0];
	tmp.m[0][1] = m1.m[0][1] + m2.m[0][1];
	tmp.m[0][2] = m1.m[0][2] + m2.m[0][2];
	tmp.m[0][3] = m1.m[0][3] + m2.m[0][3];
	tmp.m[1][0] = m1.m[1][0] + m2.m[1][0];
	tmp.m[1][1] = m1.m[1][1] + m2.m[1][1];
	tmp.m[1][2] = m1.m[1][2] + m2.m[1][2];
	tmp.m[1][3] = m1.m[1][3] + m2.m[1][3];
	tmp.m[2][0] = m1.m[2][0] + m2.m[2][0];
	tmp.m[2][1] = m1.m[2][1] + m2.m[2][1];
	tmp.m[2][2] = m1.m[2][2] + m2.m[2][2];
	tmp.m[2][3] = m1.m[2][3] + m2.m[2][3];
	tmp.m[3][0] = m1.m[3][0] + m2.m[3][0];
	tmp.m[3][1] = m1.m[3][1] + m2.m[3][1];
	tmp.m[3][2] = m1.m[3][2] + m2.m[3][2];
	tmp.m[3][3] = m1.m[3][3] + m2.m[3][3];
	return tmp;
}
inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 tmp;
	tmp.m[0][0] = m1.m[0][0] + m2.m[0][0];
	tmp.m[0][1] = m1.m[0][1] + m2.m[0][1];
	tmp.m[0][2] = m1.m[0][2] + m2.m[0][2];
	tmp.m[0][3] = m1.m[0][3] + m2.m[0][3];
	tmp.m[1][0] = m1.m[1][0] + m2.m[1][0];
	tmp.m[1][1] = m1.m[1][1] + m2.m[1][1];
	tmp.m[1][2] = m1.m[1][2] + m2.m[1][2];
	tmp.m[1][3] = m1.m[1][3] + m2.m[1][3];
	tmp.m[2][0] = m1.m[2][0] + m2.m[2][0];
	tmp.m[2][1] = m1.m[2][1] + m2.m[2][1];
	tmp.m[2][2] = m1.m[2][2] + m2.m[2][2];
	tmp.m[2][3] = m1.m[2][3] + m2.m[2][3];
	tmp.m[3][0] = m1.m[3][0] + m2.m[3][0];
	tmp.m[3][1] = m1.m[3][1] + m2.m[3][1];
	tmp.m[3][2] = m1.m[3][2] + m2.m[3][2];
	tmp.m[3][3] = m1.m[3][3] + m2.m[3][3];
	return tmp;
}
inline Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 tmp;
	tmp.m[0][0] = m1.m[0][0] - m2.m[0][0];
	tmp.m[0][1] = m1.m[0][1] - m2.m[0][1];
	tmp.m[0][2] = m1.m[0][2] - m2.m[0][2];
	tmp.m[0][3] = m1.m[0][3] - m2.m[0][3];
	tmp.m[1][0] = m1.m[1][0] - m2.m[1][0];
	tmp.m[1][1] = m1.m[1][1] - m2.m[1][1];
	tmp.m[1][2] = m1.m[1][2] - m2.m[1][2];
	tmp.m[1][3] = m1.m[1][3] - m2.m[1][3];
	tmp.m[2][0] = m1.m[2][0] - m2.m[2][0];
	tmp.m[2][1] = m1.m[2][1] - m2.m[2][1];
	tmp.m[2][2] = m1.m[2][2] - m2.m[2][2];
	tmp.m[2][3] = m1.m[2][3] - m2.m[2][3];
	tmp.m[3][0] = m1.m[3][0] - m2.m[3][0];
	tmp.m[3][1] = m1.m[3][1] - m2.m[3][1];
	tmp.m[3][2] = m1.m[3][2] - m2.m[3][2];
	tmp.m[3][3] = m1.m[3][3] - m2.m[3][3];
	return tmp;
}
inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 tmp;
	tmp.m[0][0] = m1.m[0][0] - m2.m[0][0];
	tmp.m[0][1] = m1.m[0][1] - m2.m[0][1];
	tmp.m[0][2] = m1.m[0][2] - m2.m[0][2];
	tmp.m[0][3] = m1.m[0][3] - m2.m[0][3];
	tmp.m[1][0] = m1.m[1][0] - m2.m[1][0];
	tmp.m[1][1] = m1.m[1][1] - m2.m[1][1];
	tmp.m[1][2] = m1.m[1][2] - m2.m[1][2];
	tmp.m[1][3] = m1.m[1][3] - m2.m[1][3];
	tmp.m[2][0] = m1.m[2][0] - m2.m[2][0];
	tmp.m[2][1] = m1.m[2][1] - m2.m[2][1];
	tmp.m[2][2] = m1.m[2][2] - m2.m[2][2];
	tmp.m[2][3] = m1.m[2][3] - m2.m[2][3];
	tmp.m[3][0] = m1.m[3][0] - m2.m[3][0];
	tmp.m[3][1] = m1.m[3][1] - m2.m[3][1];
	tmp.m[3][2] = m1.m[3][2] - m2.m[3][2];
	tmp.m[3][3] = m1.m[3][3] - m2.m[3][3];
	return tmp;
}
inline Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 tmp;
	tmp.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
	              m1.m[0][3] * m2.m[3][0];
	tmp.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
	              m1.m[0][3] * m2.m[3][1];
	tmp.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
	              m1.m[0][3] * m2.m[3][2];
	tmp.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
	              m1.m[0][3] * m2.m[3][3];

	tmp.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
	              m1.m[1][3] * m2.m[3][0];
	tmp.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
	              m1.m[1][3] * m2.m[3][1];
	tmp.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
	              m1.m[1][3] * m2.m[3][2];
	tmp.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
	              m1.m[1][3] * m2.m[3][3];

	tmp.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
	              m1.m[2][3] * m2.m[3][0];
	tmp.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
	              m1.m[2][3] * m2.m[3][1];
	tmp.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
	              m1.m[2][3] * m2.m[3][2];
	tmp.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
	              m1.m[2][3] * m2.m[3][3];

	tmp.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
	              m1.m[3][3] * m2.m[3][0];
	tmp.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
	              m1.m[3][3] * m2.m[3][1];
	tmp.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
	              m1.m[3][3] * m2.m[3][2];
	tmp.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
	              m1.m[3][3] * m2.m[3][3];
	return tmp;
}
inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 tmp;
	tmp.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
	              m1.m[0][3] * m2.m[3][0];
	tmp.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
	              m1.m[0][3] * m2.m[3][1];
	tmp.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
	              m1.m[0][3] * m2.m[3][2];
	tmp.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
	              m1.m[0][3] * m2.m[3][3];

	tmp.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
	              m1.m[1][3] * m2.m[3][0];
	tmp.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
	              m1.m[1][3] * m2.m[3][1];
	tmp.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
	              m1.m[1][3] * m2.m[3][2];
	tmp.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
	              m1.m[1][3] * m2.m[3][3];

	tmp.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
	              m1.m[2][3] * m2.m[3][0];
	tmp.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
	              m1.m[2][3] * m2.m[3][1];
	tmp.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
	              m1.m[2][3] * m2.m[3][2];
	tmp.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
	              m1.m[2][3] * m2.m[3][3];

	tmp.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
	              m1.m[3][3] * m2.m[3][0];
	tmp.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
	              m1.m[3][3] * m2.m[3][1];
	tmp.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
	              m1.m[3][3] * m2.m[3][2];
	tmp.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
	              m1.m[3][3] * m2.m[3][3];
	return tmp;
}
inline Matrix4x4 Inverse(const Matrix4x4& m) {
	float lal = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
	            m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
	            m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]

	            - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	            m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
	            m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]

	            - m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
	            m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	            m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]

	            + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
	            m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
	            m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]

	            + m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
	            m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
	            m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]

	            - m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	            m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
	            m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]

	            - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
	            m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	            m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]

	            + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
	            m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
	            m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	Matrix4x4 tmp;
	tmp.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
	               m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
	               m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) /
	              lal;

	tmp.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
	               m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
	               m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) /
	              lal;

	tmp.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
	               m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
	               m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) /
	              lal;

	tmp.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
	               m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
	               m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) /
	              lal;

	tmp.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
	               m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
	               m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) /
	              lal;

	tmp.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
	               m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
	               m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) /
	              lal;

	tmp.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
	               m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
	               m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) /
	              lal;

	tmp.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
	               m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
	               m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) /
	              lal;

	tmp.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
	               m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
	               m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) /
	              lal;

	tmp.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
	               m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
	               m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) /
	              lal;

	tmp.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
	               m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
	               m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) /
	              lal;

	tmp.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
	               m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
	               m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) /
	              lal;

	tmp.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
	               m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
	               m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) /
	              lal;

	tmp.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
	               m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
	               m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) /
	              lal;

	tmp.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
	               m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
	               m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) /
	              lal;

	tmp.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
	               m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
	               m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) /
	              lal;

	return tmp;
}
inline Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 tmp;
	tmp.m[0][0] = m.m[0][0];
	tmp.m[0][1] = m.m[1][0];
	tmp.m[0][2] = m.m[2][0];
	tmp.m[0][3] = m.m[3][0];

	tmp.m[1][0] = m.m[0][1];
	tmp.m[1][1] = m.m[1][1];
	tmp.m[1][2] = m.m[2][1];
	tmp.m[1][3] = m.m[3][1];

	tmp.m[2][0] = m.m[0][2];
	tmp.m[2][1] = m.m[1][2];
	tmp.m[2][2] = m.m[2][2];
	tmp.m[2][3] = m.m[3][2];

	tmp.m[3][0] = m.m[0][3];
	tmp.m[3][1] = m.m[1][3];
	tmp.m[3][2] = m.m[2][3];
	tmp.m[3][3] = m.m[3][3];

	return tmp;
}
inline Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 tmp;
	tmp.m[0][0] = 1.0f;
	tmp.m[0][1] = 0.0f;
	tmp.m[0][2] = 0.0f;
	tmp.m[0][3] = 0.0f;

	tmp.m[1][0] = 0.0f;
	tmp.m[1][1] = 1.0f;
	tmp.m[1][2] = 0.0f;
	tmp.m[1][3] = 0.0f;

	tmp.m[2][0] = 0.0f;
	tmp.m[2][1] = 0.0f;
	tmp.m[2][2] = 1.0f;
	tmp.m[2][3] = 0.0f;

	tmp.m[3][0] = 0.0f;
	tmp.m[3][1] = 0.0f;
	tmp.m[3][2] = 0.0f;
	tmp.m[3][3] = 1.0f;

	return tmp;
}
inline Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 tmp;
	tmp.m[0][0] = 1;
	tmp.m[0][1] = 0;
	tmp.m[0][2] = 0;
	tmp.m[0][3] = 0;

	tmp.m[1][0] = 0;
	tmp.m[1][1] = 1;
	tmp.m[1][2] = 0;
	tmp.m[1][3] = 0;

	tmp.m[2][0] = 0;
	tmp.m[2][1] = 0;
	tmp.m[2][2] = 1;
	tmp.m[2][3] = 0;

	tmp.m[3][0] = translate.x;
	tmp.m[3][1] = translate.y;
	tmp.m[3][2] = translate.z;
	tmp.m[3][3] = 1;

	return tmp;
}
inline Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 tmp;
	tmp.m[0][0] = scale.x;
	tmp.m[0][1] = 0;
	tmp.m[0][2] = 0;
	tmp.m[0][3] = 0;

	tmp.m[1][0] = 0;
	tmp.m[1][1] = scale.y;
	tmp.m[1][2] = 0;
	tmp.m[1][3] = 0;

	tmp.m[2][0] = 0;
	tmp.m[2][1] = 0;
	tmp.m[2][2] = scale.z;
	tmp.m[2][3] = 0;

	tmp.m[3][0] = 0;
	tmp.m[3][1] = 0;
	tmp.m[3][2] = 0;
	tmp.m[3][3] = 1;

	return tmp;
}
inline Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}
inline Vector3 operator*(const Vector3& v, const Matrix4x4& m) {
	Vector3 result;
	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + 1.0f * m.m[3][0];
	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + 1.0f * m.m[3][1];
	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + 1.0f * m.m[3][2];
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + 1.0f * m.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

inline Matrix4x4 MakeRotateXMatrix(float radian) {

	Matrix4x4 tmp;

	tmp.m[0][0] = 1;
	tmp.m[0][1] = 0;
	tmp.m[0][2] = 0;
	tmp.m[0][3] = 0;

	tmp.m[1][0] = 0;
	tmp.m[1][1] = std::cos(radian);
	tmp.m[1][2] = std::sin(radian);
	tmp.m[1][3] = 0;

	tmp.m[2][0] = 0;
	tmp.m[2][1] = -std::sin(radian);
	tmp.m[2][2] = std::cos(radian);
	tmp.m[2][3] = 0;

	tmp.m[3][0] = 0;
	tmp.m[3][1] = 0;
	tmp.m[3][2] = 0;
	tmp.m[3][3] = 1;

	return tmp;
}
inline Matrix4x4 MakeRotateYMatrix(float radian) {

	Matrix4x4 tmp;

	tmp.m[0][0] = std::cos(radian);
	tmp.m[0][1] = 0;
	tmp.m[0][2] = -std::sin(radian);
	tmp.m[0][3] = 0;

	tmp.m[1][0] = 0;
	tmp.m[1][1] = 1;
	tmp.m[1][2] = 0;
	tmp.m[1][3] = 0;

	tmp.m[2][0] = std::sin(radian);
	tmp.m[2][1] = 0;
	tmp.m[2][2] = std::cos(radian);
	tmp.m[2][3] = 0;

	tmp.m[3][0] = 0;
	tmp.m[3][1] = 0;
	tmp.m[3][2] = 0;
	tmp.m[3][3] = 1;

	return tmp;
}
inline Matrix4x4 MakeRotateZMatrix(float radian) {

	Matrix4x4 tmp;

	tmp.m[0][0] = std::cos(radian);
	tmp.m[0][1] = std::sin(radian);
	tmp.m[0][2] = 0;
	tmp.m[0][3] = 0;

	tmp.m[1][0] = -sinf(radian);
	tmp.m[1][1] = std::cos(radian);
	tmp.m[1][2] = 0;
	tmp.m[1][3] = 0;

	tmp.m[2][0] = 0;
	tmp.m[2][1] = 0;
	tmp.m[2][2] = 1;
	tmp.m[2][3] = 0;

	tmp.m[3][0] = 0;
	tmp.m[3][1] = 0;
	tmp.m[3][2] = 0;
	tmp.m[3][3] = 1;

	return tmp;
}

inline Matrix4x4
    MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 tmp = scaleMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * translateMatrix;

	return tmp;
}

inline Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) {
	m1 = m1 * m2;
	return m1;
}
// 向いてるベクトル角度
inline Vector3 LookAtOfEulerAngle(Vector3 v1) {
	Vector3 result = {0.0f, 0.0f, 0.0f};
	v1 = Normalize(v1);
	result.y = std::atan2(v1.x, v1.z);
	Matrix4x4 tmp = MakeRotateYMatrix(-std::atan2(v1.x, v1.z));
	Vector3 velocityZ = Transform(v1, tmp);
	result.x = std::atan2(-velocityZ.y, velocityZ.z);
	return result;
}

// ビューポート変換行列
inline Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 tmp;
	tmp.m[0][0] = width / 2.0f;
	tmp.m[0][1] = 0;
	tmp.m[0][2] = 0;
	tmp.m[0][3] = 0;
	tmp.m[1][0] = 0;
	tmp.m[1][1] = -height / 2.0f;
	tmp.m[1][2] = 0;
	tmp.m[1][3] = 0;
	tmp.m[2][0] = 0;
	tmp.m[2][1] = 0;
	tmp.m[2][2] = maxDepth - minDepth;
	tmp.m[2][3] = 0;
	tmp.m[3][0] = left + (width / 2.0f);
	tmp.m[3][1] = top + (height / 2.0f);
	tmp.m[3][2] = minDepth;
	tmp.m[3][3] = 1;
	return tmp;
}

//ベクトルの方向に向ける
inline void OrientVector(Vector3& rotation, Vector3& focus) { 
	Vector3 toFocus = Normalize(focus); 
	rotation.y = std::atan2(toFocus.x, toFocus.z);
	Matrix4x4 tmp = MakeRotateYMatrix(-std::atan2(toFocus.x, toFocus.z));
	Vector3 tmpZ = Transform(toFocus, tmp);
	rotation.x = std::atan2(-tmpZ.y, tmpZ.z);
}

#pragma endregion
