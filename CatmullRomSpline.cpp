#include "CatmullRomSpline.h"
#include <algorithm>
#include "PrimitiveDrawer.h"
Vector3 CatmullRomSpline::CatmullRomEasing(const std::vector<Vector3>& controlPoints, float t) {
	//制御点の数
	int numPoints = static_cast<int>( controlPoints.size());
	//tの範囲を制限
	t = std::clamp(t, 0.0f, 1.0f);
	//tを使用してどのセグメントに属するかを計算する
	float segment = t * (numPoints - 1);
	//セグメントの整数部分と小数部分を分離する
	int segmentIndex = static_cast<int>(std::floor(segment));
	float segmentFraction = segment - segmentIndex;

	//制御点のインデックスを計算する
	int p0 = (segmentIndex - 1 + numPoints) % numPoints;
	if (segmentIndex == 0) {
		p0 = 0;
	}
	int p1 = (segmentIndex + numPoints) % numPoints;
	int p2 = (segmentIndex + 1 + numPoints) % numPoints;
	int p3 = (segmentIndex + 2 + numPoints) % numPoints;
	if (segmentIndex == numPoints - 2 || segmentIndex == numPoints - 1) {
		p3 = numPoints - 1;
	}

	//セグメントにおける補間の重みを計算する

	float segmentFraction2 = segmentFraction * segmentFraction;
	float segmentFraction3 = segmentFraction2 * segmentFraction;


	float t0 = -segmentFraction3 + 2.0f * segmentFraction2 - segmentFraction;
	float t1 = 3.0f * segmentFraction3 - 5.0f * segmentFraction2 + 2.0f; 
	float t2 = -3.0f * segmentFraction3 + 4.0f * segmentFraction2 + segmentFraction;
	float t3 = segmentFraction3 - segmentFraction2;

	 // Catmull-Romスプラインの線形補間を計算する
	Vector3 interpolatedPoint;
	interpolatedPoint.x = (t0 * controlPoints[p0].x + t1 * controlPoints[p1].x + t2 * controlPoints[p2].x + t3 * controlPoints[p3].x)  * 0.5f;
	interpolatedPoint.y = (t0 * controlPoints[p0].y + t1 * controlPoints[p1].y + t2 * controlPoints[p2].y + t3 * controlPoints[p3].y)  * 0.5f;
	interpolatedPoint.z = (t0 * controlPoints[p0].z + t1 * controlPoints[p1].z + t2 * controlPoints[p2].z + t3 * controlPoints[p3].z)  * 0.5f;

	return interpolatedPoint;
}
void CatmullRomSpline::Initialize(const std::vector<Vector3>& controlPoints) {
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRomEasing(controlPoints, t);
		// 描画用頂点リストに追加
		points_.push_back(pos);
	}
}
void CatmullRomSpline::Draw() { 
	for (int i = 0; i < points_.size() - 1; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(points_[i], points_[i + 1], {1.0f,0.0f,0.0f,1.0f}); 	
	}
}