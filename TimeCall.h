#pragma once

#include <functional>

/// <summary>
/// 時限発動
/// </summary>
class TimeCall {
public:
	//コンストラクタ
	TimeCall(std::function<void(void)> f, uint32_t time);
	//更新
	void Update();
	//完了ならtrueを返す
	bool IsFinished() { return isComplete_; }

private:
	//コールバック
	 std::function<void(void)> f_;
	 //残り時間
	 uint32_t time_;
	 //完了フラグ
	 bool isComplete_ = false;
};
