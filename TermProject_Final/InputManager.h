#pragma once
enum class KEY_STATE { NONE, TAP, PRESSED, AWAY };

struct KeyInfo {
	KEY_STATE mState{};
	bool mIsPrevPushed{};
};

class InputManager {
	SINGLETON_PATTERN(InputManager)

public:
	InputManager();
	~InputManager();

private:
	std::unordered_map<int, KeyInfo> mKeys;

public:
	void Init();
	void Update();

public:
	KEY_STATE GetKeyState(int key)
	{
		return mKeys[key].mState;
	}
};


#define KEY_NONE(key)		InputManager::Inst()->GetKeyState(key) == KEY_STATE::NONE
#define KEY_TAP(key)		InputManager::Inst()->GetKeyState(key) == KEY_STATE::TAP
#define KEY_PRESSED(key)	InputManager::Inst()->GetKeyState(key) == KEY_STATE::PRESSED
#define KEY_AWAY(key)		InputManager::Inst()->GetKeyState(key) == KEY_STATE::AWAY