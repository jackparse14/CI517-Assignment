#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

struct GameKey {
	bool isAlive;
	SDL_Rect* keyBox;
	Rect* keyPos;
};

struct Enemy {
	bool isAlive;
	SDL_Rect* enemyBox;
	Rect* enemyPos;
	Timer enemyTimer;
	bool runEnemyTimer;
	int random0or1;
	SDL_Texture* enemyTexture;
};


class MyGame : public AbstractGame {
	private:
		// Arena Corners
		Point2 topLeftArena;
		Point2 topRightArena;
		Point2 bottomLeftArena;
		Point2 bottomRightArena;

		//Input Booleans
		bool leftButtonPressed;
		bool rightButtonPressed;
		bool upButtonPressed;
		bool downButtonPressed;

		//	Player Object Variables
		SDL_Rect* playerBox;
		Rect playerPos;
		int lives;
		Point2 posPlayerTopLeft;
		Point2 posPlayerBottomLeft;
		Point2 posPlayerTopRight;
		Point2 posPlayerBottomRight;

		//	Flag Object Variables
		SDL_Rect* flagBox;
		Rect flagPos;
		bool renderFlag;

		//	Achievement Objects
		MoveUpAch* upPtr;
		MoveDownAch* downPtr;
		MoveLeftAch* leftPtr;
		MoveRightAch* rightPtr;
		OneKeyAch* oneKeyPtr;
		ThirdKeyAch* thirdKeyPtr;
		TenthKeyAch* tenthKeyPtr;
		TakeDamageAch* takeDamagePtr;
		Level0Ach* level0Ptr;
		WinGameAch* winGamePtr;

		//	Sound Object
		Mix_Chunk* achGetSound;

		//	Font Objects
		TTF_Font* NameAchFont;
		TTF_Font* MessageAchFont;
		TTF_Font* DefaultFont;

		//	Image Objects
		SDL_Texture* coinTexture;
		SDL_Texture* flagTexture;

		//	Player:
		SDL_Texture* playerTexture;
		SDL_Texture* playerRightTexture;
		SDL_Texture* playerLeftTexture;
		SDL_Texture* playerUpTexture;
		SDL_Texture* playerDownTexture;
		SDL_Texture* playerDownLeftTexture;
		SDL_Texture* playerDownRightTexture;
		SDL_Texture* playerUpLeftTexture;
		SDL_Texture* playerUpRightTexture;

		//	Enemy:
		SDL_Texture* enemyUpTexture;
		SDL_Texture* enemyDownTexture;
		SDL_Texture* enemyLeftTexture;
		SDL_Texture* enemyRightTexture;

		//	Vectors for storing multiple keys and enemies
		std::vector<std::shared_ptr<GameKey>> gameKeys;
		std::vector<std::shared_ptr<Enemy>> enemies;

		/* GAMEPLAY */
		int score, numKeys, level, keysCollected, numEnemies;
		Vector2i velocity;
		bool gameWon, gameLost, level0Complete;

		//	Input Function
		void handleKeyEvents();

		//	Load Functions
		void loadFonts();
		void loadImages();
		
		//	Initialize Functions
		void initAchievements();
		void initGameKeys();
		void initEnemies();
		void setArenaBoundaries();

		//	Update Functions
		void update();
		void updatePlayer();
		void updateEnemy();
		void updateGameKeys();
		void updateAchievements();

		//	Render Functions
		void render();
		void renderUI();
		void renderAchievement();
		
		//	Other Functions
		void genNextLevel();
		void pauseGame();
	public:
        MyGame();
		~MyGame();
};
#endif