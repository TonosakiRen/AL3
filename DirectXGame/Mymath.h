#pragma once
#define _USE_MATH_DEFINES
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <numbers>
#include <assert.h>
#include <cmath>
#include "WorldTransform.h"
struct OBB {
	Vector3 center;          // 中心点
	Vector3 orientations[3]; // 座標軸、正規化、直交座標
	Vector3 size;            // 座標軸方向の長さの半分。中心から面までの距離
};

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
inline float Degree(float radian) { return radian * 180.0f / std::numbers::pi_v<float>; }

#pragma region Vector3
// Vetor3

inline Vector3 GetXAxis(Matrix4x4 m) { return {m.m[0][0], m.m[0][1], m.m[0][2]}; }
inline Vector3 GetYAxis(Matrix4x4 m) { return {m.m[1][0], m.m[1][1], m.m[1][2]}; }
inline Vector3 GetZAxis(Matrix4x4 m) { return {m.m[2][0], m.m[2][1], m.m[2][2]}; }
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
inline Vector3 operator-(const Vector3& v1) { return {-v1.x, -v1.y, -v1.z}; }

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
//外積
inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return {(v1.y * v2.z - v1.z * v2.y), (v1.z * v2.x - v1.x * v2.z), (v1.x * v2.y - v1.y * v2.x)};
}
inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
	return {v1.x * v2.x , v1.y * v2.y , v1.z * v2.z};
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

inline bool closeRotate(Vector3& num, Vector3 goal, float speed) {
	if ((std::fabsf(num.x - goal.x) < std::fabsf(speed)) &&
	    (std::fabsf(num.y - goal.y) < std::fabsf(speed)) &&
	    (std::fabsf(num.z - goal.z) < std::fabsf(speed))) {
		num = goal;
		return true;
	}

	
	if (std::fabs(num.x - goal.x) < std::fabs(speed)) {
		num.x = goal.x;
	} else if (
	    (std::numbers::pi_v<float> - std::fabs(num.x)) +
	        (std::numbers::pi_v<float> - std::fabs(goal.x)) <
	    std::fabs(speed)) {
		num.x = goal.x;
	} else
	if (std::fabs(num.x - goal.x) > std::numbers::pi_v<float>) {
		if (goal.x < 0.0f) {
			num.x += speed;
			if (num.x > std::numbers::pi_v<float>) {
				num.x = -std::numbers::pi_v<float> + (num.x - std::numbers::pi_v<float>);
			}
		} else if (goal.x > 0.0f) {
			num.x -= speed;
			if (num.x < -std::numbers::pi_v<float>) {
				num.x= std::numbers::pi_v<float> - (-std::numbers::pi_v<float> - num.x);
			}
		}

	} else
	if (num.x < goal.x) {
		num.x += speed;
	}else
	if (num.x > goal.x) {
		num.x -= speed;
	}

	if (std::fabs(num.y - goal.y) < std::fabs(speed)) {
		num.y = goal.y;
	} else if (
	    (std::numbers::pi_v<float> - std::fabs(num.y)) +
	        (std::numbers::pi_v<float> - std::fabs(goal.y)) <
	    std::fabs(speed)) { 
		num.y = goal.y;
	}
	else
	if (std::fabs(num.y - goal.y) > std::numbers::pi_v<float>) {
		if (goal.y < 0.0f) {
			num.y += speed;
			if (num.y > std::numbers::pi_v<float>) {
				num.y = -std::numbers::pi_v<float> + (num.y - std::numbers::pi_v<float>);
			}
		}else
		if (goal.y > 0.0f) {
			num.y -= speed;
			if (num.y < -std::numbers::pi_v<float>) {
				num.y = std::numbers::pi_v<float> - (-std::numbers::pi_v<float> - num.y);
			}
		}
		
	}else
	if (num.y < goal.y) {
		num.y += speed;
	}else
	if (num.y > goal.y) {
		num.y -= speed;
	}

	if (std::fabs(num.z - goal.z) < std::fabs(speed)) {
		num.z = goal.z;
	} else if (
	    (std::numbers::pi_v<float> - std::fabs(num.z)) +
	        (std::numbers::pi_v<float> - std::fabs(goal.z)) <
	    std::fabs(speed)) {
		num.z = goal.z;
	} else
	if (std::fabs(num.z - goal.z) > std::numbers::pi_v<float>) {
		if (goal.z < 0.0f) {
			num.z += speed;
			if (num.z > std::numbers::pi_v<float>) {
				num.z = -std::numbers::pi_v<float> + (num.z - std::numbers::pi_v<float>);
			}
		} else if (goal.z > 0.0f) {
			num.z -= speed;
			if (num.z < -std::numbers::pi_v<float>) {
				num.z = std::numbers::pi_v<float> - (-std::numbers::pi_v<float> - num.z);
			}
		}

	} else
	if (num.z < goal.z) {
		num.z += speed;
	}else
	if (num.z > goal.z) {
		num.z -= speed;
	}
	return false;
}

inline bool closeVector3(Vector3& num, Vector3 goal, float speed) {
	if ((std::fabsf(num.x - goal.x) < std::fabsf(speed)) &&
	    (std::fabsf(num.y - goal.y) < std::fabsf(speed)) &&
	    (std::fabsf(num.z - goal.z) < std::fabsf(speed))) {
		num = goal;
		return true;
	}

	if (std::fabs(num.x - goal.x) < std::fabs(speed)) {
		num.x = goal.x;
	}  else if (num.x < goal.x) {
		num.x += speed;
	} else if (num.x > goal.x) {
		num.x -= speed;
	}

	if (std::fabs(num.y - goal.y) < std::fabs(speed)) {
		num.y = goal.y;
	}  else if (num.y < goal.y) {
		num.y += speed;
	} else if (num.y > goal.y) {
		num.y -= speed;
	}

	if (std::fabs(num.z - goal.z) < std::fabs(speed)) {
		num.z = goal.z;
	} else if (num.z < goal.z) {
		num.z += speed;
	} else if (num.z > goal.z) {
		num.z -= speed;
	}
	return false;
}

inline Vector3 LoseY(Vector3 loseY) { return Vector3{loseY.x, 0.0f, loseY.z}; }

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
inline Matrix4x4 MakeRotateMatrix(const Vector3& rotate) {
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;
	return rotateMatrix;
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

inline bool IsCollision(const OBB& obb1, const OBB& obb2) {
	Vector3 separationAxisCandidate[15];
	// 各軸
	separationAxisCandidate[0] = obb1.orientations[0];
	separationAxisCandidate[1] = obb1.orientations[1];
	separationAxisCandidate[2] = obb1.orientations[2];
	separationAxisCandidate[3] = obb2.orientations[0];
	separationAxisCandidate[4] = obb2.orientations[1];
	separationAxisCandidate[5] = obb2.orientations[2];
	// 各辺のクロス積
	separationAxisCandidate[6] = Cross(obb1.orientations[0], obb2.orientations[0]);
	separationAxisCandidate[7] = Cross(obb1.orientations[0], obb2.orientations[1]);
	separationAxisCandidate[8] = Cross(obb1.orientations[0], obb2.orientations[2]);
	separationAxisCandidate[9] = Cross(obb1.orientations[1], obb2.orientations[0]);
	separationAxisCandidate[10] = Cross(obb1.orientations[1], obb2.orientations[1]);
	separationAxisCandidate[11] = Cross(obb1.orientations[1], obb2.orientations[2]);
	separationAxisCandidate[12] = Cross(obb1.orientations[2], obb2.orientations[0]);
	separationAxisCandidate[13] = Cross(obb1.orientations[2], obb2.orientations[1]);
	separationAxisCandidate[14] = Cross(obb1.orientations[2], obb2.orientations[2]);
	// 頂点
	Matrix4x4 obb1WorldMatrix = {
	    obb1.orientations[0].x, obb1.orientations[0].y, obb1.orientations[0].z, 0.0f,
	    obb1.orientations[1].x, obb1.orientations[1].y, obb1.orientations[1].z, 0.0f,
	    obb1.orientations[2].x, obb1.orientations[2].y, obb1.orientations[2].z, 0.0f,
	    obb1.center.x,          obb1.center.y,          obb1.center.z,          1.0f};
	Matrix4x4 obb2WorldMatrix = {
	    obb2.orientations[0].x, obb2.orientations[0].y, obb2.orientations[0].z, 0.0f,
	    obb2.orientations[1].x, obb2.orientations[1].y, obb2.orientations[1].z, 0.0f,
	    obb2.orientations[2].x, obb2.orientations[2].y, obb2.orientations[2].z, 0.0f,
	    obb2.center.x,          obb2.center.y,          obb2.center.z,          1.0f};

	Vector3 vertices1[] = {
	    -obb1.size,
	    {obb1.size.x,  -obb1.size.y, -obb1.size.z},
	    {obb1.size.x,  -obb1.size.y, obb1.size.z },
	    {-obb1.size.x, -obb1.size.y, obb1.size.z },
	    {-obb1.size.x, obb1.size.y,  -obb1.size.z},
	    {obb1.size.x,  obb1.size.y,  -obb1.size.z},
	    obb1.size,
	    {-obb1.size.x, obb1.size.y,  obb1.size.z }
    };

	Vector3 vertices2[] = {
	    -obb2.size,
	    {obb2.size.x,  -obb2.size.y, -obb2.size.z},
	    {obb2.size.x,  -obb2.size.y, obb2.size.z },
	    {-obb2.size.x, -obb2.size.y, obb2.size.z },
	    {-obb2.size.x, obb2.size.y,  -obb2.size.z},
	    {obb2.size.x,  obb2.size.y,  -obb2.size.z},
	    obb2.size,
	    {-obb2.size.x, obb2.size.y,  obb2.size.z }
    };

	for (int i = 0; i < 8; i++) {
		vertices1[i] = vertices1[i] * obb1WorldMatrix;
		vertices2[i] = vertices2[i] * obb2WorldMatrix;
	}

	// 各軸
	for (int i = 0; i < 15; i++) {
		// 影の長さの合計
		float sumSpan;
		// 2つの影の両端の差分
		float longSpan;
		// 射影した最大値最小値
		float max1, min1, max2, min2;
		// 差分が形状を分離軸に射影した長さ
		float L1, L2;
		// すべての頂点を射影した値
		float Dot1[8];
		float Dot2[8];
		// 各頂点
		for (int j = 0; j < 8; j++) {
			Dot1[j] = Dot(separationAxisCandidate[i], vertices1[j]);
		}
		for (int k = 0; k < 8; k++) {
			Dot2[k] = Dot(separationAxisCandidate[i], vertices2[k]);
		}
		max1 = (std::max)({Dot1[0], Dot1[1], Dot1[3], Dot1[4], Dot1[5], Dot1[6], Dot1[7]});
		min1 = (std::min)({Dot1[0], Dot1[1], Dot1[3], Dot1[4], Dot1[5], Dot1[6], Dot1[7]});
		L1 = max1 - min1;
		max2 = (std::max)({Dot2[0], Dot2[1], Dot2[3], Dot2[4], Dot2[5], Dot2[6], Dot2[7]});
		min2 = (std::min)({Dot2[0], Dot2[1], Dot2[3], Dot2[4], Dot2[5], Dot2[6], Dot2[7]});
		L2 = max2 - min2;

		sumSpan = L1 + L2;
		longSpan = (std::max)(max1, max2) - (std::min)(min1, min2);
		if (sumSpan < longSpan) {
			return false;
		}
	}
	return true;
};

inline bool IsOBBCollision(const WorldTransform& worldTransform1, const WorldTransform& worldTransform2) { 
	
	
	OBB obb1;
	OBB obb2;
	obb1.center = worldTransform1.worldTranslation_;
	obb2.center = worldTransform2.worldTranslation_;
	obb1.size = worldTransform1.worldScale_;
	obb2.size = worldTransform2.worldScale_;
	obb1.orientations[0] = GetXAxis(worldTransform1.worldRotateMatrix_);
	obb1.orientations[1] = GetYAxis(worldTransform1.worldRotateMatrix_);
	obb1.orientations[2] = GetZAxis(worldTransform1.worldRotateMatrix_);
	obb2.orientations[0] = GetXAxis(worldTransform2.worldRotateMatrix_);
	obb2.orientations[1] = GetYAxis(worldTransform2.worldRotateMatrix_);
	obb2.orientations[2] = GetZAxis(worldTransform2.worldRotateMatrix_);

	return IsCollision(obb1, obb2);;
}

#pragma endregion
