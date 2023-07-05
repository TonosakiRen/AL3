#pragma once
#include<vector>
#include "Mymath.h"
#include <ViewProjection.h>
class CatmullRomSpline {
public:
	//線分の数
	const size_t segmentCount = 2000;
	
	//線文で描画する用の頂点リスト
	std::vector<Vector3> points_;

public:
	Vector3 CatmullRomEasing(const std::vector<Vector3>& controlPoints, float t);
	void Initialize(const std::vector<Vector3>& controlPoints);
	void Draw();
 
};
