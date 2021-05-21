#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(5), numEnemies(1), level(0), keysCollected(0), gameWon(false),gameLost(false),playerPos(150, 110, 30, 30),flagPos(DEFAULT_WINDOW_WIDTH/2 - 45,DEFAULT_WINDOW_HEIGHT/2 - 45,45,45) {
	loadFonts();
	loadImages();
	//	Loud Sound
	achGetSound = ResourceManager::loadSound("res/sounds/jingle-achievement.wav");

	gfx->setVerticalSync(true);

	setArenaBoundaries();
	
	initGameKeys() ; 
	initEnemies();
	initAchievements();

	//	Initialize Player
	playerBox = new SDL_Rect();
	SDLrectPtrEqualsRect(playerBox,playerPos);
	playerTexture = playerDownTexture;

	//	Initialize Flag
	flagBox = new SDL_Rect();
	SDLrectPtrEqualsRect(flagBox,flagPos);
	renderFlag = false;
}

void MyGame::loadFonts() {
	NameAchFont = ResourceManager::loadFont("res/fonts/BreadSmile.ttf", 30);
	MessageAchFont = ResourceManager::loadFont("res/fonts/RifficFree-Bold.ttf", 20);
	DefaultFont = ResourceManager::loadFont("res/fonts/RifficFree-Bold.ttf", 50);
}
void MyGame::loadImages() {
	coinTexture = ResourceManager::loadTexture("res/images/coin100.png", SDL_COLOR_WHITE);
	flagTexture = ResourceManager::loadTexture("res/images/endFlag.png", SDL_COLOR_BLACK);

	//	Player Textures
	playerRightTexture = ResourceManager::loadTexture("res/images/mainSpriteRight.png", SDL_COLOR_WHITE);
	playerLeftTexture = ResourceManager::loadTexture("res/images/mainSpriteLeft.png", SDL_COLOR_WHITE);
	playerUpTexture = ResourceManager::loadTexture("res/images/mainSpriteUp.png", SDL_COLOR_WHITE);
	playerDownTexture = ResourceManager::loadTexture("res/images/mainSpriteDown.png", SDL_COLOR_WHITE);
	playerDownLeftTexture = ResourceManager::loadTexture("res/images/mainSpriteDownLeft.png", SDL_COLOR_WHITE);
	playerDownRightTexture = ResourceManager::loadTexture("res/images/mainSpriteDownRight.png", SDL_COLOR_WHITE);
	playerUpLeftTexture = ResourceManager::loadTexture("res/images/mainSpriteUpLeft.png", SDL_COLOR_WHITE);
	playerUpRightTexture = ResourceManager::loadTexture("res/images/mainSpriteUpRight.png", SDL_COLOR_WHITE);

	//	Enemy Textures
	enemyUpTexture = ResourceManager::loadTexture("res/images/enemySpriteUp.png", SDL_COLOR_WHITE);
	enemyDownTexture = ResourceManager::loadTexture("res/images/enemySpriteDown.png", SDL_COLOR_WHITE);
	enemyLeftTexture = ResourceManager::loadTexture("res/images/enemySpriteLeft.png", SDL_COLOR_WHITE);
	enemyRightTexture = ResourceManager::loadTexture("res/images/enemySpriteRight.png", SDL_COLOR_WHITE);
}

void MyGame::setArenaBoundaries() {
	//	Creates the 4 corners of the arena
	topLeftArena = { 5, 100 };
	bottomLeftArena = { 5, DEFAULT_WINDOW_HEIGHT - 5 };
	topRightArena = { DEFAULT_WINDOW_WIDTH - 5, 100 };
	bottomRightArena = { DEFAULT_WINDOW_WIDTH - 5, DEFAULT_WINDOW_HEIGHT - 5 };
}

void MyGame::initGameKeys() {
	//	Loops through all keys
	for (int i = 0; i < numKeys; i++) {
		std::shared_ptr<GameKey> k = std::make_shared<GameKey>();
		k->isAlive = true;
		//	Creates the key in a random location within the arena
		k->keyPos = new Rect(getRandom(topLeftArena.x, topRightArena.x - 30), getRandom(topLeftArena.y, bottomLeftArena.y - 30), 30, 30);
		k->keyBox = new SDL_Rect();
		SDLrectPtrEqualsRectPtr(k->keyBox, k->keyPos);
		//	Pushes the key onto the gameKeys vector
		gameKeys.push_back(k);
	}
}

void MyGame::initEnemies() {
	//	Loops through all the enemies
	for (int i = 0; i < numEnemies; i++){
		std::shared_ptr<Enemy> E = std::make_shared<Enemy>();
		E->isAlive = true;
		//	Creates the Enemy in a random location within the arena
		E->enemyPos = new Rect(getRandom(topLeftArena.x, topRightArena.x - 30), getRandom(topLeftArena.y, bottomLeftArena.y - 30),30,30);
		E->enemyBox = new SDL_Rect();
		SDLrectPtrEqualsRectPtr(E->enemyBox, E->enemyPos);
		E->enemyTexture = enemyDownTexture;
		E->runEnemyTimer = true;
		//	Pushes the enemy onto the enemies vector
		enemies.push_back(E);
	}
}

void MyGame::initAchievements() {
	//	Creates instances of all the achievements
	upPtr = new MoveUpAch(mySystem);
	downPtr = new MoveDownAch(mySystem);
	leftPtr = new MoveLeftAch(mySystem);
	rightPtr = new MoveRightAch(mySystem);
	oneKeyPtr = new OneKeyAch(mySystem);
	thirdKeyPtr = new ThirdKeyAch(mySystem);
	tenthKeyPtr = new TenthKeyAch(mySystem);
	takeDamagePtr = new TakeDamageAch(mySystem);
	level0Ptr = new Level0Ach(mySystem);
	winGamePtr = new WinGameAch(mySystem);

	//	Creates an output pointer to a generic achievement
	mySystem->outputPtr = new Achievement();
	mySystem->outputPtr->Render = false;
	mySystem->runAchTimer = true;
	level0Complete = false;
}

MyGame::~MyGame() {
}

void MyGame::handleKeyEvents() {
	//	Variable for the speed of the player
	int playerSpeed = 3;

	//	Makes variables false when not pressing any buttons
	leftButtonPressed = false;
	rightButtonPressed = false;
	upButtonPressed = false;
	downButtonPressed = false;
	
	//	Function for when W is pressed
	if (eventSystem->isPressed(Key::W)) {
		//	Moves the player up
		velocity.y = -playerSpeed;
		//	Changes sprite to Up sprite
		playerTexture = playerUpTexture;
		//	When up is pressed, down isn't pressed
		upButtonPressed = true;
		downButtonPressed = false;
	}
	//	Function for when S is pressed
	if (eventSystem->isPressed(Key::S)) {
		//	Moves the player down
		velocity.y = playerSpeed;
		//	Changes sprite to Down sprite
		playerTexture = playerDownTexture;
		//	When down is pressed, up isn't pressed
		downButtonPressed = true;
		upButtonPressed = false;
	}
	//	Function for when A is pressed
	if (eventSystem->isPressed(Key::A)) {
		//	Moves the player left
		velocity.x = -playerSpeed;
		//	Changes sprite to Left sprite
		playerTexture = playerLeftTexture;
		//	When left is pressed, right isn't pressed
		leftButtonPressed = true;
		rightButtonPressed = false;
	}
	//	Function for when D is pressed
	if (eventSystem->isPressed(Key::D)) {
		//	Moves the player right
		velocity.x = playerSpeed;
		//	Changes sprite to Right sprite
		playerTexture = playerRightTexture;
		//	When right is pressed, left isn't pressed
		rightButtonPressed = true;	
		leftButtonPressed = false;
	}  

	// Changes player sprite when going in diagonals
	if (upButtonPressed && leftButtonPressed) {
		playerTexture = playerUpLeftTexture;
	}
	if (downButtonPressed && rightButtonPressed) {
		playerTexture = playerDownRightTexture;
	}
	if (upButtonPressed && rightButtonPressed) {
		playerTexture = playerUpRightTexture;
	}
	if (downButtonPressed && leftButtonPressed) {
		playerTexture = playerDownLeftTexture;
	}
}

void MyGame::genNextLevel() {
	level++;
	numKeys = 5;
	initGameKeys();
	initEnemies();
}

void MyGame::update() {
	mySystem->achievementTimer();
	//	Runs as long as game isn't in a win or loss state
	if (gameLost == false && gameWon == false) {
		updatePlayer();
		updateGameKeys();
		updateEnemy();

		//	Stops the player when buttons arent pressed
		velocity.x = 0;
		velocity.y = 0;

		//	When the player has all the keys in a level
		if (numKeys == 0) {
			//	Creates flag for player to finish the level
			renderFlag = true;
			bool flagOverlapping = flagPos.contains(posPlayerTopLeft) || flagPos.contains(posPlayerBottomLeft) || flagPos.contains(posPlayerTopRight) || flagPos.contains(posPlayerBottomRight);
			//	If player touches flag
			if (flagOverlapping) {
				//	If the its the 5th level game is won
				if (level == 5) {
					gameWon = true;
				}
				//	Otherwise generate next level
				else if (level == 0) {
					level0Complete = true;
					genNextLevel();
				}
				else {
					genNextLevel();
				}
				renderFlag = false;
			}
		}

		//	Ends game if all lives are lost
		if (lives <= 0) { gameLost = true; }

		//	Keep Player in the Arena Boundary
		if (playerBox->x <= topLeftArena.x) {
			playerBox->x = topLeftArena.x;
		}
		if (playerBox->x + playerBox->w >= topRightArena.x) {
			playerBox->x = topRightArena.x - playerBox->w;
		}
		if (playerBox->y <= topLeftArena.y) {
			playerBox->y = topLeftArena.y;
		}
		if (playerBox->y + playerBox->h >= bottomLeftArena.y) {
			playerBox->y = bottomLeftArena.y - playerBox->h;
		}	
	}
	updateAchievements();
}

void MyGame::updatePlayer() {
	//	Moves the player when velocity is increased
	playerBox->x += velocity.x;
	playerBox->y += velocity.y;
	playerPos.x += velocity.x;
	playerPos.y += velocity.y;
	
	//	Assigns the players position to variables
	posPlayerTopLeft = { playerBox->x, playerBox->y };
	posPlayerBottomLeft = { playerBox->x, playerBox->y + playerBox->h };
	posPlayerTopRight = { playerBox->x + playerBox->w, playerBox->y };
	posPlayerBottomRight = { playerBox->x + playerBox->w, playerBox->y + playerBox->h };
}

void MyGame::updateEnemy() {
	//	Variable for the speed of the enemies
	int enemySpeed = 2;
	//	Loops through all the enemies
	for (auto enemy : enemies) {
		if (enemy->runEnemyTimer) {
			enemy->enemyTimer.measure();
			//	Gets a random number (either 0 or 1) for simple enemy AI
			enemy->random0or1 = getRandom(0, 2);
			//	Ends the timer
			enemy->runEnemyTimer = false;
			//	Starts a timer for all the enemies every second
		} else if (enemy->enemyTimer.getElapsed() >= 1000) {
			enemy->runEnemyTimer = true;
		}

		//	Variable for the centre point of each enemy
		Point2 enemyPosCentre;
		enemyPosCentre.y = ((enemy->enemyBox->y * 2) + enemy->enemyBox->h) / 2;
		enemyPosCentre.x = ((enemy->enemyBox->x * 2) + enemy->enemyBox->w) / 2;

		//	If the random number is 0
		if (enemy->random0or1 == 0) {
			//	If the enemy is inline with the player on the x axis, then it needs to move on the y axis
			if (enemyPosCentre.x > posPlayerTopLeft.x && enemyPosCentre.x < posPlayerTopRight.x) {
				enemy->random0or1 = 1;
				return;
			}
			//	If the enemy needs to move right to get to the player
			else if (enemy->enemyBox->x < playerBox->x) {
				enemy->enemyTexture = enemyRightTexture;
				//	Move enemy
				enemy->enemyBox->x += enemySpeed;
				enemy->enemyPos->x += enemySpeed;
			}
			//	If the enemy needs to move left to get to the player
			else if (enemy->enemyBox->x > playerBox->x) {
				enemy->enemyTexture = enemyLeftTexture;
				//	Move enemy
				enemy->enemyBox->x -= enemySpeed;
				enemy->enemyPos->x -= enemySpeed;
			}
		//	If the random number is 1
		} else if (enemy->random0or1 == 1) {
			//	If the enemy is inline with the player on the y axis, then it needs to move on the x axis
			if (enemyPosCentre.y > posPlayerTopLeft.y && enemyPosCentre.y < posPlayerBottomLeft.y) {
				enemy->random0or1 = 0;
				return;
			}
			//	If the enemy needs to move up to get to the player
			else if (enemy->enemyBox->y > playerBox->y) {
				enemy->enemyTexture = enemyUpTexture;
				//	Move enemy
				enemy->enemyBox->y -= enemySpeed;
				enemy->enemyPos->y -= enemySpeed;
			}
			//	If the enemy needs to move down to get to the player
			else if (enemy->enemyBox->y < playerBox->y) {
				enemy->enemyTexture = enemyDownTexture;
				//	Move enemy
				enemy->enemyBox->y += enemySpeed;
				enemy->enemyPos->y += enemySpeed;
			}
		}
		bool enemyOverlapping = enemy->enemyPos->contains(posPlayerTopLeft) || enemy->enemyPos->contains(posPlayerBottomLeft) || enemy->enemyPos->contains(posPlayerTopRight) || enemy->enemyPos->contains(posPlayerBottomRight);
		//	If player touches an enemy
		if (enemy->isAlive && enemyOverlapping) {
			//	Kills enemy
			enemy->isAlive = false;
			//	Player takes damage
			lives--;
			//	Takes 500 from score
			score -= 500;
			//	Makes sure score doesn't go below 0
			if (score < 0) { score = 0; }
		}
	}
}

void MyGame::updateGameKeys() {
	for (auto key : gameKeys) {
		bool keyOverlapping = key->keyPos->contains(posPlayerTopLeft) || key->keyPos->contains(posPlayerBottomLeft) || key->keyPos->contains(posPlayerTopRight) || key->keyPos->contains(posPlayerBottomRight);
		//	If the player is touching a key
		if (key->isAlive && keyOverlapping) {
			//	Delete the key
			key->isAlive = false;
			numKeys--;
			keysCollected++;
			//	Add 100 to score
			score += 100;
		}
	}
}

void MyGame::updateAchievements() {
	//	Checks if achievements have been completed and unregisters them
	if (keysCollected == 10) {
		mySystem->unregAchievement(tenthKeyPtr);
	}
	if (keysCollected == 3) {
		mySystem->unregAchievement(thirdKeyPtr);
	}
	if (keysCollected == 1) {
		mySystem->unregAchievement(oneKeyPtr);
	}
	if (level0Complete) {
		mySystem->unregAchievement(level0Ptr);
	}
	if (gameWon == true) {
		mySystem->unregAchievement(winGamePtr);
	}
	if (lives < 3) {
		mySystem->unregAchievement(takeDamagePtr);
	}
	if (leftButtonPressed) {
		mySystem->unregAchievement(leftPtr);
	}
	if (rightButtonPressed) {
		mySystem->unregAchievement(rightPtr);
	}
	if (upButtonPressed) {
		mySystem->unregAchievement(upPtr);	
	}
	if (downButtonPressed) {
		mySystem->unregAchievement(downPtr);
	}
	//	If an achievement is completed we have to play the sound
	if (mySystem->playSound) {
			sfx->playSound(achGetSound);
			mySystem->playSound = false;
	}
}

void MyGame::render() {
	if (gameWon == false && gameLost == false) {
		//	Renders the player
		gfx->drawTexture(playerTexture, playerBox);
		
		//	Renders the enemies if they are alive
		for (auto enemy : enemies) {
			if (enemy->isAlive) {
				gfx->drawTexture(enemy->enemyTexture, enemy->enemyBox);
			}
		}
		
		//	Renders the gameKeys if they are alive
		for (auto key : gameKeys) {
			if (key->isAlive) {
				gfx->drawTexture(coinTexture, key->keyBox);
			}
		}

		//	Renders the arena
		gfx->setDrawColor(SDL_COLOR_YELLOW);
		gfx->drawLine(topLeftArena, bottomLeftArena);
		gfx->drawLine(bottomLeftArena, bottomRightArena);
		gfx->drawLine(bottomRightArena, topRightArena);
		gfx->drawLine(topRightArena, topLeftArena);
	}
	//	Renders the flag if all the keys have been collected
	if (renderFlag) {
		gfx->drawTexture(flagTexture, flagBox);
	}
}

void MyGame::renderUI() {
	//	Turns score, level and lives to strings so they can be rendered
	std::string scoreStr = "Score: " + std::to_string(score);
	std::string levelStr = "Level: " + std::to_string(level);
	std::string livesStr = "Lives: " + std::to_string(lives);
	
	renderAchievement();
	// Decides what UI to render depending on game state
	if (gameWon) {
		gfx->useFont(DefaultFont);
		gfx->setDrawColor(SDL_COLOR_GREEN);
		//	Renders You Won messsage
		gfx->drawText("YOU WON!", 250, DEFAULT_WINDOW_HEIGHT/2);
		gfx->setDrawColor(SDL_COLOR_YELLOW);
		//	Moves Score to the middle of the screen
		gfx->drawText(scoreStr, 245, DEFAULT_WINDOW_HEIGHT / 2 - 45);
		pauseGame();
	}
	else if (gameLost) {
		gfx->useFont(DefaultFont);
		gfx->setDrawColor(SDL_COLOR_RED);
		//	Render You Lose message
		gfx->drawText("YOU LOSE!", 250, DEFAULT_WINDOW_HEIGHT / 2);
		pauseGame();
	}
	else {
		gfx->useFont(MessageAchFont);
		gfx->setDrawColor(SDL_COLOR_YELLOW);
		//	Renders the players score
		gfx->drawText(scoreStr, 780 - scoreStr.length() * 12, 70);
		gfx->setDrawColor(SDL_COLOR_AQUA);
		//	Renders the level
		gfx->drawText(levelStr,780 - scoreStr.length() * 12, 10);
		gfx->setDrawColor(SDL_COLOR_GREEN);
		//	Renders the players lives
		gfx->drawText(livesStr, 780 - scoreStr.length() * 12, 40);
	}
}	

void MyGame::renderAchievement() {
	//	If the outPtr has achievement to Render 
	if (mySystem->outputPtr->Render == true) {
		gfx->useFont(MessageAchFont);
		gfx->setDrawColor(SDL_COLOR_YELLOW);
		//	Render Achievement Complete:
		gfx->drawText("Achievement Complete:", 25, 10);
		gfx->setDrawColor(SDL_COLOR_AQUA);
		//	Render achievement message
		gfx->drawText(mySystem->outputPtr->Message, 40, 70);
		gfx->useFont(NameAchFont);
		//	Render achievement Name
		gfx->drawText(mySystem->outputPtr->Name, 30, 30);
	}
}

void MyGame::pauseGame() {
	//	If there are no achievements to be displayed, pause the game
	if (mySystem->nextAchOutputQueue.empty()) {
		paused = true;
	}
}