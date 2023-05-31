#include "TimeCall.h"
// コンストラクタ
TimeCall::TimeCall(std::function<void(void)> f, uint32_t time) { 
	time_ = time;
	f_ = f;
}
// 更新
void TimeCall::Update() {
	if (IsFinished()) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isComplete_ = true;
		//callback関数呼び出し
		f_();
	}
}
