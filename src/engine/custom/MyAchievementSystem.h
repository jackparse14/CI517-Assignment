#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__

#include "../EngineCommon.h"
#include <vector>
#include "../Timer.h"
#include <queue>

class Achievement {
	public:
		std::string Name;
		std::string Message;
		bool Completed = false;
		bool Render = false;
		int ID;
};

class MyAchievementSystem {
	friend class XCube2Engine;
private:
	std::vector<Achievement*> achievementVector;
public:
	Achievement* outputPtr;
	Timer achTimer;
	bool runAchTimer;
	bool playSound = false;
	std::queue<Achievement*> nextAchOutputQueue;
	std::vector<Achievement*> unlockedAchVector;
	void regAchievement(Achievement*);
	void unregAchievement(Achievement*);
	void achievementTimer();
};

//	Movement Achievements --------------------------------------------------------------------------------------
class MoveUpAch : public Achievement {
	public:
		MoveUpAch(std::shared_ptr<MyAchievementSystem>);
};

class MoveDownAch : public Achievement {
	public:
		MoveDownAch(std::shared_ptr<MyAchievementSystem>);
};

class MoveLeftAch : public Achievement {
	public:
		MoveLeftAch(std::shared_ptr<MyAchievementSystem>);
};

class MoveRightAch : public Achievement {
	public:
		MoveRightAch(std::shared_ptr<MyAchievementSystem>);
};

//	Key Achievements -------------------------------------------------------------------------------------------
class OneKeyAch : public Achievement {
	public:
		OneKeyAch(std::shared_ptr<MyAchievementSystem>);
};

class ThirdKeyAch : public Achievement {
public:
	ThirdKeyAch(std::shared_ptr<MyAchievementSystem>);
};

class TenthKeyAch : public Achievement {
public:
	TenthKeyAch(std::shared_ptr<MyAchievementSystem>);
};

//	Other Achievements -----------------------------------------------------------------------------------------
class TakeDamageAch : public Achievement {
public:
	TakeDamageAch(std::shared_ptr<MyAchievementSystem>);
};

class Level0Ach : public Achievement {
public:
	Level0Ach(std::shared_ptr<MyAchievementSystem>);
};

class WinGameAch : public Achievement {
public:
	WinGameAch(std::shared_ptr<MyAchievementSystem>);
};

#endif