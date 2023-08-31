#pragma once
#include<cstdlib>
#include<ctime>

namespace Randam{

	/// <summary>
	/// 乱数
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>int型</returns>
	int RAND(int min, int max);

	float RAND(float min, float max);

	bool RAND();

	void SRAND();
	///srand((unsingned)time(NULL));
}