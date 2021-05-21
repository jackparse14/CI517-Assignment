#include "MyAchievementSystem.h"

void MyAchievementSystem::regAchievement(Achievement* achievement) {
	//	Adds achievement to achievementVector vector
	achievementVector.push_back(achievement);
}

void MyAchievementSystem::unregAchievement(Achievement* achievement) {
	if (achievement->Completed == false) {
			//	Loops through all achievements that havent been completed
			for (auto Ach : achievementVector) {
				//	If the achievement has a bigger ID than the completed achievement
				if (Ach->ID > achievement->ID) {
					//	Take 1 from the achievement ID
					Ach->ID = Ach->ID - 1;
				}
			}
			//	Makes the completed variable true
			achievement->Completed = true;
			//	Tells the Game the achievement needs to be Rendered
			achievement->Render = true;
			
			//	Outputs to console the Achievement
			std::cout << achievement->Name << " - " << achievement->Message << std::endl;
			//	Deletes the completed achievement from achievementVector
			achievementVector.erase(achievementVector.begin() + (achievement->ID));	
			//	Adds completed achievement to the output queue
			nextAchOutputQueue.push(achievement);
			//	Adds comppleted achievement to unlocked vector
			unlockedAchVector.push_back(achievement);
	}
}

void MyAchievementSystem::achievementTimer() {
	//	Makes sure it only runs if there is an achievement in queue
	if (nextAchOutputQueue.empty()) {
		return;
	}
	else {
		//	Starts the timer and outputs the achievement at the front of the queue
		if (runAchTimer) {
			achTimer.measure();
			outputPtr->Render = true;
			outputPtr = nextAchOutputQueue.front();
			//	plays achievement sound by making true
			playSound = true;
			runAchTimer = false;
		}
		//	After 3 seconds the achievement stops being rendered and is taken out of the queue
		if (achTimer.getElapsed() >= 3000){
			outputPtr->Render = false;
			runAchTimer = true;
			nextAchOutputQueue.pop();
		}
	}
}

//	CONSTRUCTORS ---------------------------------------------------------------------------------------
TakeDamageAch::TakeDamageAch(std::shared_ptr<MyAchievementSystem> subject) {
	Name = "Take Damage!";
	Message = "You lost a life! :(";
	ID = 7;
	subject->regAchievement(this);
}

Level0Ach::Level0Ach(std::shared_ptr<MyAchievementSystem> subject) {
	Name = "Level 0!";
	Message = "You completed Level 1-1!";
	ID = 8;
	subject->regAchievement(this);
}

WinGameAch::WinGameAch(std::shared_ptr<MyAchievementSystem> subject) {
	Name = "Mission Accomplished!";
	Message = "You completed the game!";
	ID = 9;
	subject->regAchievement(this);
}

//	Movement Achievements
MoveUpAch::MoveUpAch(std::shared_ptr<MyAchievementSystem> subject) {
	Name = "Move Up!";
	Message = "You pressed W!";
	ID = 0;
	subject->regAchievement(this);
}

MoveDownAch::MoveDownAch(std::shared_ptr<MyAchievementSystem> subject) {
	Name = "Move Down!";
	Message = "You pressed S!";
	ID = 1;
	subject->regAchievement(this);
}

MoveLeftAch::MoveLeftAch(std::shared_ptr<MyAchievementSystem> subject) {
	Name = "Move Left!";
	Message = "You pressed A!";
	ID = 2;
	subject->regAchievement(this);
}

MoveRightAch::MoveRightAch(std::shared_ptr<MyAchievementSystem> subject) {
	Name = "Move Right!";
	Message = "You pressed D!";
	ID = 3;
	subject->regAchievement(this);
}

//	Key Achievements
OneKeyAch::OneKeyAch(std::shared_ptr<MyAchievementSystem> subject) {
	Name = "One Key!";
	Message = "You picked up your first Key!";
	ID = 4;
	subject->regAchievement(this);
}

ThirdKeyAch::ThirdKeyAch(std::shared_ptr<MyAchievementSystem> subject) {
	Name = "Third Key!";
	Message = "You picked up three Keys!";
	ID = 5;
	subject->regAchievement(this);
}

TenthKeyAch::TenthKeyAch(std::shared_ptr<MyAchievementSystem> subject) {
	Name = "Tenth Key!";
	Message = "You picked up ten Keys!";
	ID = 6;
	subject->regAchievement(this);
}

