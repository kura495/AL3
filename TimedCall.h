#pragma once
#include <functional>
#include <initializer_list>
class TimedCall {
public:
	TimedCall(std::function<void()> function, uint32_t time);
	~TimedCall();

	void Update();

	bool IsConpleted() const { return IsConplete_; }

private:
	// �R�[���o�b�N
	std::function<void()> function_;
	// �c�莞��
	uint32_t time_;
	// �����t���O
	bool IsConplete_ = false;
};
