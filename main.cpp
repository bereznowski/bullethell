#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

extern "C"
{
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

//======================================================================================================================//

#define maxNumberOfMissilesInWave 30
#define maxNumberOfMissileWaves 10

#define maxNumberOfExplodingFields 5

#define numberOfLevels 3

#define screenWidth 640
#define screenHeight 480

#define widthOfFirstBoard 1920
#define heightOfFirstBoard 1080
#define widthOfSecondBoard 1920
#define heightOfSecondBoard 1080
#define widthOfThirdBoard 1920
#define heightOfThirdBoard 1080

#define playerDefaultXOnFirstBoard widthOfFirstBoard/4
#define playerDefaultYOnFirstBoard heightOfFirstBoard/4
#define playerDefaultXOnSecondBoard widthOfSecondBoard/4
#define playerDefaultYOnSecondBoard heightOfSecondBoard/4
#define playerDefaultXOnThirdBoard widthOfThirdBoard/4
#define playerDefaultYOnThirdBoard heightOfThirdBoard/4

#define enemyDefaultXOnFirstBoard widthOfFirstBoard/2
#define enemyDefaultYOnFirstBoard heightOfFirstBoard/2
#define enemyDefaultXOnSecondBoard widthOfSecondBoard/2
#define enemyDefaultYOnSecondBoard heightOfSecondBoard/2
#define enemyDefaultXOnThirdBoard widthOfThirdBoard/2
#define enemyDefaultYOnThirdBoard heightOfThirdBoard/2

#define levelOneNumberOfWaves maxNumberOfMissileWaves/3
#define levelTwoNumberOfWaves maxNumberOfMissileWaves/2
#define levelThreeNumberOfWaves maxNumberOfMissileWaves

#define levelOneNumberOfMissilesInWaves maxNumberOfMissilesInWave/4
#define levelTwoNumberOfMissilesInWaves maxNumberOfMissilesInWave/2
#define levelThreeNumberOfMissilesInWaves maxNumberOfMissilesInWave

#define levelOneWaveDuration 1
#define levelTwoWaveDuration 2
#define levelThreeWaveDuration 2

#define levelOnePatternInterval 5
#define levelTwoPatternInterval 4
#define levelThreePatternInterval 3

#define maxLengthOfErrorMessage 100

#define maxLengthOfPath 100

#define levelOne 0
#define levelTwo 1
#define levelThree 2

#define playerSize 28
#define enemySize 56
#define normalMissileSize 14
#define bigMissileSize 50
#define playerMissileSize 10
#define explodingFieldCircleSize 150
#define explodingFieldSquareSize 100
#define explodingMissileSize 20

#define enemySpeed 50
#define normalMissileSpeed 150
#define bigMissileSpeed 75
#define playerMissileSpeed 100

#define explodingFieldSquareDelay 2
#define explodingFieldCircleDelay 3

#define maxLengthOfText 200

#define upKey "\030"
#define downKey "\031"
#define leftKey "\032"
#define rightKey "\033"

#define playerMoveSize 15

#define waveTypeDuration 5

#define spiralType 0
#define circleType 1

#define notMoving false
#define moving true

#define probabilityOfExplodingFieldSquare 0.002
#define probabilityOfExplodingFieldCircle 0.002
#define probabilityOfExplodingMissile 0.002
#define probabilityOfBigMissile 0.002
#define probabilityOfHealth 0.002

#define circle 0
#define square 1

#define maxPlayerHealth 100.0
#define maxEnemyHealth 500.0
#define hitByExplodingField 5.0
#define hitByNormalMissile 1.0
#define hitByBigMissile 10.0
#define hitByExplodingMissile 7.0

#define maxNumberOfExplodingMissiles 3
#define numberOfMissilesFromExplosion 10

#define numberOfPlayerMissiles 50

#define left 1
#define right 2
#define up 3
#define down 4

#define menuOptionsX 40
#define menuOption1Y screenHeight/6
#define menuOption2Y 2 * screenHeight/6
#define menuOption3Y 3 * screenHeight/6
#define menuOption4Y 4 * screenHeight/6
#define menuOptionsWidth screenWidth - 2 * menuOptionsX
#define menuOptionsHeight screenHeight/12

#define invincibilityTime 0.5

#define maxNumberOfBigMissilesWaves 1
#define maxNumberOfBigMissiles 5

#define gameOver 100

#define maxDifferenceBetweenHits 1.0

#define maxNumberOfHealthFields 15
#define healthSize 20
#define healthDelay 5
#define bonusHealth 10.0
#define bonusPoints 100.0

//======================================================================================================================//

struct WindowParameters
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* screenSurface = NULL;
	SDL_Texture* screenTexture = NULL;
	SDL_Rect* renderedRectangle = NULL;
};

struct ColorParameters
{
	int boardColor;
	int textBackgroundColor;
	int textColor;
	int healthColor;
	int healthTextColor;
};

struct TimeParameters
{
	int t1;
	int t2;
	int frames;
	double delta;
	double worldTime;
	int worldTimeInt;
	double fpsTimer;
	double fps;
	double lastWave;
	double lastMissile;
	double frameDelay;
};

struct InformationParameters
{
	int x;
	int y;
	int width;
	int height;
	int playerHealthX;
	int playerHealthY;
	int playerHealthWidth;
	int playerHealthHeight;
	int enemyHealthX;
	int enemyHealthY;
	int enemyHealthWidth;
	int enemyHealthHeight;
};

struct MissilesParameters
{
	int currentCircularWave;
	int currentSpiralWave;
	int currentMissileInSpiralWave;
	int currentExplodingFieldCircle;
	int currentExplodingFieldSquare;
	int currentExplodingMissile;
	int currentBigMissile;
	int currentHealth;
};

struct Missile
{
	int x;
	int y;
	int launchingX;
	int launchingY;
	int size;
	double angle;
	double angleSin;
	double angleCos;
	double speed;
	double launchedAt;
	bool existence;
	SDL_Surface* picture = NULL;
};

struct ExplodingMissile
{
	int x;
	int y;
	int launchingX;
	int launchingY;
	int destinationX;
	int destinationY;
	int size;
	double angle;
	double angleSin;
	double angleCos;
	double speed;
	double launchedAt;
	bool existence;
	SDL_Surface* picture = NULL;
	Missile** normalMissiles = NULL;
};

struct ExplodingField
{
	int x;
	int y;
	int size;
	double launchedAt;
	double activationDelay;
	bool existence;
	SDL_Surface* picture = NULL;
};

struct Player
{
	int x;
	int y;
	int size;
	int currentMissile;
	int combo;
	double lastHitAt;
	double lastShotAt;
	bool invincibility;
	bool foundBonus;
	double health;
	double lastHealth;
	SDL_Surface* picture = NULL;
	Missile** missiles;
};

struct Enemy
{
	int x;
	int y;
	int newX;
	int newY;
	int oldX;
	int oldY;
	int size;
	int combo;
	double lastHitAt;
	double angleSin;
	double angleCos;
	double speed;
	double health;
	double lastHealth;
	double startedMovingAt;
	bool movement;
	SDL_Surface* picture = NULL;
};

struct Level
{
	int playerStartingX;
	int playerStartingY;
	int enemyStartingX;
	int enemyStartingY;
	int boardWidth;
	int boardHeight;
	int numberOfWaves;
	int numberOfMissilesInWaves;
	int patternInterval;
	double waveDuration;
	double missilesInterval;
	bool enemyMoving;
};

struct GameParameters
{
	int lastKey;
	SDL_Surface* charset = NULL;
	WindowParameters* windowParameters = NULL;
	ColorParameters* colorParameters = NULL;
	TimeParameters* timeParameters = NULL;
	InformationParameters* informationParameters = NULL;
	MissilesParameters* missilesParameters = NULL;
	Player* player = NULL;
	Enemy* enemy = NULL;
	Missile*** missiles = NULL;
	Missile** bigMissiles = NULL;
	ExplodingMissile** explodingMissiles = NULL;
	ExplodingField** explodingFieldsSquare = NULL;
	ExplodingField** explodingFieldsCircle = NULL;
	ExplodingField** health = NULL;
	Level** levels = NULL;
};

//======================================================================================================================//

Missile* allocateMemoryForMissile()
{
	Missile* missile = (Missile*)malloc(sizeof(Missile));
	return missile;
}

Missile** allocateMemoryForGroupOfMissiles(int numberOfMissilesInGroup)
{
	Missile** waveOfMissiles = (Missile**)malloc(sizeof(Missile*) * numberOfMissilesInGroup);

	for(int i = 0; i < numberOfMissilesInGroup; i++)
		waveOfMissiles[i] = allocateMemoryForMissile();

	return waveOfMissiles;
}

Missile*** allocateMemoryForMissiles()
{
	Missile*** missiles = (Missile***)malloc(sizeof(Missile**) * maxNumberOfMissilesInWave * maxNumberOfMissileWaves);

	for(int i = 0; i < maxNumberOfMissileWaves; i++)
		missiles[i] = allocateMemoryForGroupOfMissiles(maxNumberOfMissilesInWave);

	return missiles;
}

Player* allocateMemoryForPlayer()
{
	Player* player = (Player*)malloc(sizeof(Player));

	player->missiles = allocateMemoryForGroupOfMissiles(numberOfPlayerMissiles);

	return player;
}

ExplodingMissile* allocateMemoryForExplodingMissile(int numberOfMissilesInGroup)
{
	ExplodingMissile* explodingMissile = (ExplodingMissile*)malloc(sizeof(ExplodingMissile));

	explodingMissile->normalMissiles = allocateMemoryForGroupOfMissiles(numberOfMissilesInGroup);

	return explodingMissile;
}

ExplodingMissile** allocateMemoryForExplodingMissiles()
{
	ExplodingMissile** explodingMissiles = (ExplodingMissile**)malloc(sizeof(ExplodingMissile) * maxNumberOfExplodingMissiles);

	for(int i = 0; i < maxNumberOfExplodingMissiles; i++)
		explodingMissiles[i] = allocateMemoryForExplodingMissile(numberOfMissilesFromExplosion);

	return explodingMissiles;
}

ExplodingField** allocateMemoryForExplodingFields(int numberOfExplodingFields)
{
	ExplodingField** explodingFields = (ExplodingField**)malloc(sizeof(ExplodingField*) * numberOfExplodingFields);

	for(int i = 0; i < numberOfExplodingFields; i++)
		explodingFields[i] = (ExplodingField*)malloc(sizeof(ExplodingField));

	return explodingFields;
}

Level** allocateMemoryForLevels()
{
	Level** levels = (Level**)malloc(sizeof(Level*) * numberOfLevels);

	for(int i = 0; i < numberOfLevels; i++)
		levels[i] = (Level*)malloc(sizeof(Level));

	return levels;
}

GameParameters* allocateMemoryForGameParameters()
{
	GameParameters* gameParameters = (GameParameters*)malloc(sizeof(GameParameters));
	gameParameters->windowParameters = (WindowParameters*)malloc(sizeof(WindowParameters));
	gameParameters->colorParameters = (ColorParameters*)malloc(sizeof(ColorParameters));
	gameParameters->timeParameters = (TimeParameters*)malloc(sizeof(TimeParameters));
	gameParameters->informationParameters = (InformationParameters*)malloc(sizeof(InformationParameters));
	gameParameters->missilesParameters = (MissilesParameters*)malloc(sizeof(MissilesParameters));
	gameParameters->player = allocateMemoryForPlayer();
	gameParameters->enemy = (Enemy*)malloc(sizeof(Enemy));
	gameParameters->missiles = allocateMemoryForMissiles();
	gameParameters->bigMissiles = allocateMemoryForGroupOfMissiles(maxNumberOfBigMissiles);
	gameParameters->explodingMissiles = allocateMemoryForExplodingMissiles();
	gameParameters->explodingFieldsSquare = allocateMemoryForExplodingFields(maxNumberOfExplodingFields);
	gameParameters->explodingFieldsCircle = allocateMemoryForExplodingFields(maxNumberOfExplodingFields);
	gameParameters->health = allocateMemoryForExplodingFields(maxNumberOfHealthFields);
	gameParameters->levels = allocateMemoryForLevels();

	return gameParameters;
}

bool startSDL(WindowParameters* windowParameters)
{
    bool success = true;
    int rc;

    // initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL_Init error: %s\n", SDL_GetError());
        success = false;
    }

    // create window
    rc = SDL_CreateWindowAndRenderer(screenWidth, screenHeight, 0,
	                                 &(windowParameters->window), &(windowParameters->renderer));
	if(rc != 0)
    {
        success = false;
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
	}

    return success;
}

void configureWindow(WindowParameters* windowParameters)
{
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(windowParameters->renderer, screenWidth, screenHeight);
	SDL_SetRenderDrawColor(windowParameters->renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(windowParameters->window, "Bullet Hell");

    windowParameters->screenSurface = SDL_CreateRGBSurface(0, widthOfFirstBoard, heightOfFirstBoard, 32,
	                                      0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    windowParameters->screenTexture = SDL_CreateTexture(windowParameters->renderer, SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_STREAMING, widthOfFirstBoard, heightOfFirstBoard);
	windowParameters->renderedRectangle = (SDL_Rect*)malloc(sizeof(SDL_Rect));

	SDL_ShowCursor(SDL_DISABLE);
}

void loadPictureForGroupOfMissiles(Missile** groupOfMissiles, char* missileBMP, int numberOfMissilesInGroup)
{
	for(int i = 0; i < numberOfMissilesInGroup; i++)
		groupOfMissiles[i]->picture = SDL_LoadBMP(missileBMP);
}

void loadPictureForMissiles(Missile*** missiles, char* missileBMP, int numberOfGroupsOfMissiles, int numberOfMissilesInGroup)
{
	for(int i = 0; i < numberOfGroupsOfMissiles; i++)
		loadPictureForGroupOfMissiles(missiles[i], missileBMP, numberOfMissilesInGroup);
}

void loadPictureForExplodingMissiles(ExplodingMissile** explodingMissiles, char* explodingMissilesBMP, char* missileBMP)
{
	for(int i = 0; i < maxNumberOfExplodingMissiles; i++)
	{
		explodingMissiles[i]->picture = SDL_LoadBMP(explodingMissilesBMP);
		loadPictureForGroupOfMissiles(explodingMissiles[i]->normalMissiles, missileBMP, numberOfMissilesFromExplosion);
	}
}

void loadPictureForExplodingFields(ExplodingField** explodingFields, char* explodingFieldBMP, int numberOfExplodingFields)
{
	for(int i = 0; i < numberOfExplodingFields; i++)
		explodingFields[i]->picture = SDL_LoadBMP(explodingFieldBMP);
}

void displayError(char* errorMessage)
{
	printf("%s error: %s\n", errorMessage, SDL_GetError());
}

void freeGroupOfMissilesSurfaces(Missile** missiles, int numberOfMissilesInGroup)
{
	for(int i = 0; i < numberOfMissilesInGroup; i++)
		SDL_FreeSurface(missiles[i]->picture);
}

void freeMissilesSurfaces(Missile*** missiles, int numberOfGroupsofMissiles, int numberOfMissilesInGroup)
{
	for(int i = 0; i < numberOfGroupsofMissiles; i++)
		freeGroupOfMissilesSurfaces(missiles[i], numberOfMissilesInGroup);
}

void freeExplodingMissilesSurfaces(ExplodingMissile** explodingMissiles)
{
	for(int i = 0; i < maxNumberOfExplodingMissiles; i++)
	{
		SDL_FreeSurface(explodingMissiles[i]->picture);
		freeGroupOfMissilesSurfaces(explodingMissiles[i]->normalMissiles, numberOfMissilesFromExplosion);
	}
}

void freeExplodingFieldsSurfaces(ExplodingField** explodingField, int numberOfExplodingFields)
{
	for(int i = 0; i < numberOfExplodingFields; i++)
		SDL_FreeSurface(explodingField[i]->picture);
}

void endSDL(SDL_Surface* charset, Player* player, Enemy* enemy, Missile*** missiles, Missile** bigMissiles, ExplodingMissile** explodingMissiles, ExplodingField** explodingFieldsSquare, ExplodingField** explodingFieldsCircle, WindowParameters* windowParameters, ExplodingField** health)
{
	SDL_FreeSurface(charset);
	SDL_FreeSurface(player->picture);
	SDL_FreeSurface(enemy->picture);
	freeMissilesSurfaces(missiles, maxNumberOfMissileWaves, maxNumberOfMissilesInWave);
	freeGroupOfMissilesSurfaces(bigMissiles, maxNumberOfBigMissiles);
	freeGroupOfMissilesSurfaces(player->missiles, numberOfPlayerMissiles);
	freeExplodingMissilesSurfaces(explodingMissiles);
	freeExplodingFieldsSurfaces(explodingFieldsSquare, maxNumberOfExplodingFields);
	freeExplodingFieldsSurfaces(explodingFieldsCircle, maxNumberOfExplodingFields);
	freeExplodingFieldsSurfaces(health, maxNumberOfHealthFields);
	free(windowParameters->renderedRectangle);
	SDL_FreeSurface(windowParameters->screenSurface);
	SDL_DestroyTexture(windowParameters->screenTexture);
	SDL_DestroyRenderer(windowParameters->renderer);
	SDL_DestroyWindow(windowParameters->window);
	SDL_Quit();
}

void freeLevels(Level** levels)
{
	for(int i = 0; i < numberOfLevels; i++)
		free(levels[i]);

	free(levels);	
}

void freeExplodingFields(ExplodingField** explodingFields, int numberOfExplodingFields)
{
	for(int i = 0; i < numberOfExplodingFields; i++)
		free(explodingFields[i]);

	free(explodingFields);
}

void freeGroupOfMissiles(Missile** missiles, int numberOfMissilesInGroup)
{
	for(int i = 0; i < numberOfMissilesInGroup; i++)
		free(missiles[i]);

	free(missiles);
}

void freeMissiles(Missile*** missiles)
{
	for(int i = 0; i < maxNumberOfMissileWaves; i++)
		freeGroupOfMissiles(missiles[i], maxNumberOfMissilesInWave);
	
	free(missiles);
}

void freeExplodingMissiles(ExplodingMissile** explodingMissiles)
{
	for(int i = 0; i < maxNumberOfExplodingMissiles; i++)
		freeGroupOfMissiles(explodingMissiles[i]->normalMissiles, numberOfMissilesFromExplosion);
	
	free(explodingMissiles);
}

void freeMemory(GameParameters* gameParameters)
{
	endSDL(gameParameters->charset, gameParameters->player, gameParameters->enemy, gameParameters->missiles, gameParameters->bigMissiles, gameParameters->explodingMissiles, gameParameters->explodingFieldsSquare, gameParameters->explodingFieldsCircle, gameParameters->windowParameters, gameParameters->health);
	freeLevels(gameParameters->levels);
	freeExplodingFields(gameParameters->health, maxNumberOfHealthFields);
	freeExplodingFields(gameParameters->explodingFieldsSquare, maxNumberOfExplodingFields);
	freeExplodingFields(gameParameters->explodingFieldsCircle, maxNumberOfExplodingFields);
	freeExplodingMissiles(gameParameters->explodingMissiles);
	freeGroupOfMissiles(gameParameters->bigMissiles, maxNumberOfBigMissiles);
	freeMissiles(gameParameters->missiles);
	free(gameParameters->enemy);
	free(gameParameters->player);
	free(gameParameters->missilesParameters);
	free(gameParameters->informationParameters);
	free(gameParameters->timeParameters);
	free(gameParameters->colorParameters);
	free(gameParameters->windowParameters);
	free(gameParameters);
}

bool checkIfPicturesLoadedSuccessfully(GameParameters* gameParameters)
{
	if(gameParameters->charset == NULL)
	{
		char errorMessage[maxLengthOfErrorMessage] = "SDL_LoadBMP(cs8x8.bmp)";
		displayError(errorMessage);
		freeMemory(gameParameters);
		return false;
	}
	else if(gameParameters->player->picture == NULL)
	{
		char errorMessage[maxLengthOfErrorMessage] = "SDL_LoadBMP(player.bmp)";
		displayError(errorMessage);
		freeMemory(gameParameters);
		return false;
	}
	else if(gameParameters->enemy->picture == NULL)
	{
		char errorMessage[maxLengthOfErrorMessage] = "SDL_LoadBMP(enemy1.bmp)";
		displayError(errorMessage);
		freeMemory(gameParameters);
		return false;
	}
	else if(gameParameters->missiles[0][0]->picture == NULL)
	{
		char errorMessage[maxLengthOfErrorMessage] = "SDL_LoadBMP(normal_missile.bmp)";
		displayError(errorMessage);
		freeMemory(gameParameters);
		return false;
	}
	else if(gameParameters->bigMissiles[0]->picture == NULL)
	{
		char errorMessage[maxLengthOfErrorMessage] = "SDL_LoadBMP(big_missile.bmp)";
		displayError(errorMessage);
		freeMemory(gameParameters);
		return false;
	}
	else if(gameParameters->player->missiles[0]->picture == NULL)
	{
		char errorMessage[maxLengthOfErrorMessage] = "SDL_LoadBMP(player_missile.bmp)";
		displayError(errorMessage);
		freeMemory(gameParameters);
		return false;
	}
	else if(gameParameters->explodingMissiles[0]->picture == NULL)
	{
		char errorMessage[maxLengthOfErrorMessage] = "SDL_LoadBMP(exploding_missile.bmp)";
		displayError(errorMessage);
		freeMemory(gameParameters);
		return false;
	}
	else if(gameParameters->explodingFieldsSquare[0]->picture == NULL)
	{
		char errorMessage[maxLengthOfErrorMessage] = "SDL_LoadBMP(exploding_field_square.bmp)";
		displayError(errorMessage);
		freeMemory(gameParameters);
		return false;
	}
	else if(gameParameters->explodingFieldsCircle[0]->picture == NULL)
	{
		char errorMessage[maxLengthOfErrorMessage] = "SDL_LoadBMP(exploding_field_circle.bmp)";
		displayError(errorMessage);
		freeMemory(gameParameters);
		return false;
	}
	else if(gameParameters->health[0]->picture == NULL)
	{
		char errorMessage[maxLengthOfErrorMessage] = "SDL_LoadBMP(heart.bmp)";
		displayError(errorMessage);
		freeMemory(gameParameters);
		return false;
	}

	return true;
}

void setValuesForColorParameters(WindowParameters* windowParameters, ColorParameters* colorParameters)
{
	colorParameters->boardColor = SDL_MapRGB(windowParameters->screenSurface->format, 0x00, 0x00, 0x00); // black
	colorParameters->textBackgroundColor = SDL_MapRGB(windowParameters->screenSurface->format, 0x11, 0x11, 0xCC); // blue
	colorParameters->textColor = SDL_MapRGB(windowParameters->screenSurface->format, 0xFF, 0x00, 0x00); // red
	colorParameters->healthColor = SDL_MapRGB(windowParameters->screenSurface->format, 0xFF, 0x00, 0x00); // red
	colorParameters->healthTextColor = SDL_MapRGB(windowParameters->screenSurface->format, 0xFF, 0xFF, 0xFF); // white
}

void setValuesForLevel(Level* level, int width, int height, int playerX, int playerY, int enemyX, int enemyY,
					   int numberOfWaves, int numberOfMissilesInWaves, double waveDuration, double patternInterval, bool enemyMovement)
{
	level->boardWidth = width;
	level->boardHeight = height;
	level->playerStartingX = playerX;
	level->playerStartingY = playerY;
	level->enemyStartingX = enemyX;
	level->enemyStartingY = enemyY;
	level->numberOfWaves = numberOfWaves;
	level->numberOfMissilesInWaves = numberOfMissilesInWaves;
	level->waveDuration = waveDuration;
	level->missilesInterval = waveDuration/numberOfMissilesInWaves;
	level->patternInterval = patternInterval;
	level->enemyMoving = enemyMovement;
}

void setValuesForLevels(Level** levels)
{
	setValuesForLevel(levels[0], widthOfFirstBoard, heightOfFirstBoard, playerDefaultXOnFirstBoard,
					  playerDefaultYOnFirstBoard, enemyDefaultXOnFirstBoard, enemyDefaultYOnFirstBoard,
					  levelOneNumberOfWaves, levelOneNumberOfMissilesInWaves, levelOneWaveDuration, levelOnePatternInterval,
					  notMoving);
	setValuesForLevel(levels[1], widthOfSecondBoard, heightOfSecondBoard, playerDefaultXOnSecondBoard,
					  playerDefaultYOnSecondBoard, enemyDefaultXOnSecondBoard, enemyDefaultYOnSecondBoard,
					  levelTwoNumberOfWaves, levelTwoNumberOfMissilesInWaves, levelTwoWaveDuration, levelTwoPatternInterval,
					  notMoving);
	setValuesForLevel(levels[2], widthOfThirdBoard, heightOfThirdBoard, playerDefaultXOnThirdBoard,
					  playerDefaultYOnThirdBoard, enemyDefaultXOnThirdBoard, enemyDefaultYOnThirdBoard,
					  levelThreeNumberOfWaves, levelThreeNumberOfMissilesInWaves, levelThreeWaveDuration, levelThreePatternInterval,
					  moving);

}

void changePositionOfRenderedRectangle(SDL_Rect* renderedRectangle, int x, int y)
{
	renderedRectangle->x = x;
	renderedRectangle->y = y;
	renderedRectangle->w = screenWidth;
	renderedRectangle->h = screenHeight;
}

// draw a single pixel
void drawPixel(SDL_Surface* surface, int x, int y, Uint32 color)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
}

// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void drawLine(SDL_Surface* screenSurface, int x, int y, int l, int dx, int dy, Uint32 color)
{
	for(int i = 0; i < l; i++)
	{
		drawPixel(screenSurface, x, y, color);
		x += dx;
		y += dy;
	}
}

// draw a rectangle of size l by k
void drawRectangle(SDL_Surface* screenSurface, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor)
{
	int i;

	drawLine(screenSurface, x, y, k, 0, 1, outlineColor);
	drawLine(screenSurface, x + l - 1, y, k, 0, 1, outlineColor);
	drawLine(screenSurface, x, y, l, 1, 0, outlineColor);
	drawLine(screenSurface, x, y + k - 1, l, 1, 0, outlineColor);

	for(i = y + 1; i < y + k - 1; i++)
		drawLine(screenSurface, x + 1, i, l - 2, 1, 0, fillColor);
}

// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void drawString(SDL_Surface *screenSurface, int x, int y, const char *text,
                SDL_Surface *charset)
{
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text)
	{
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screenSurface, &d);
		x += 8;
		text++;
	}
}

void drawMenu(SDL_Surface* charset, WindowParameters* windowParameters, ColorParameters* colorParameters, InformationParameters* informationParameters, Player* player, Enemy* enemy, int* menuOption)
{
	char text[maxLengthOfText];

	drawRectangle(windowParameters->screenSurface, 0, 0, screenWidth, screenHeight, colorParameters->boardColor, colorParameters->boardColor);
	drawRectangle(windowParameters->screenSurface, menuOptionsX, menuOption1Y, menuOptionsWidth, menuOptionsHeight, colorParameters->textBackgroundColor, colorParameters->textBackgroundColor);
	drawRectangle(windowParameters->screenSurface, menuOptionsX, menuOption2Y, menuOptionsWidth, menuOptionsHeight, colorParameters->textBackgroundColor, colorParameters->textBackgroundColor);
	drawRectangle(windowParameters->screenSurface, menuOptionsX, menuOption3Y, menuOptionsWidth, menuOptionsHeight, colorParameters->textBackgroundColor, colorParameters->textBackgroundColor);
	drawRectangle(windowParameters->screenSurface, menuOptionsX, menuOption4Y, menuOptionsWidth, menuOptionsHeight, colorParameters->textBackgroundColor, colorParameters->textBackgroundColor);

	sprintf(text, "Level 1");
	drawString(windowParameters->screenSurface, menuOptionsX + 4, menuOption1Y + 10, text, charset);
	sprintf(text, "Level 2");
	drawString(windowParameters->screenSurface, menuOptionsX + 4, menuOption2Y + 10, text, charset);
	sprintf(text, "Level 3");
	drawString(windowParameters->screenSurface, menuOptionsX + 4, menuOption3Y + 10, text, charset);
	sprintf(text, "Quit");
	drawString(windowParameters->screenSurface, menuOptionsX + 4, menuOption4Y + 10, text, charset);
	
	drawRectangle(windowParameters->screenSurface, menuOptionsX, (1 + *menuOption) * menuOption1Y + menuOptionsHeight , menuOptionsWidth, 10, colorParameters->textColor, colorParameters->textColor);
}

void displayMenu(GameParameters* gameParameters, int* menuOption)
{
	changePositionOfRenderedRectangle(gameParameters->windowParameters->renderedRectangle, 0, 0);
	SDL_FillRect(gameParameters->windowParameters->screenSurface, NULL, gameParameters->colorParameters->boardColor);
	drawMenu(gameParameters->charset, gameParameters->windowParameters, gameParameters->colorParameters, gameParameters->informationParameters, gameParameters->player, gameParameters->enemy, menuOption);
	SDL_UpdateTexture(gameParameters->windowParameters->screenTexture, NULL, (gameParameters->windowParameters->screenSurface)->pixels, (gameParameters->windowParameters->screenSurface)->pitch);
	SDL_RenderCopy(gameParameters->windowParameters->renderer, gameParameters->windowParameters->screenTexture, gameParameters->windowParameters->renderedRectangle, NULL);
	SDL_RenderPresent(gameParameters->windowParameters->renderer);
}

int handleMenu(SDL_Event event, int* menuOption, GameParameters* gameParameters)
{
	int quit = 0;
	while(quit == 0)
	{		
		while(SDL_PollEvent(&event))
		{
			displayMenu(gameParameters, menuOption);
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE || (event.key.keysym.sym == SDLK_RETURN && *menuOption == 3))
						quit = 1;
					else if(event.key.keysym.sym == SDLK_RETURN)
						quit = 2;
					else if(event.key.keysym.sym == SDLK_UP)
					{
						if(*menuOption > levelOne)
							(*menuOption)--;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						if(*menuOption < 3)
							(*menuOption)++;
					}
					break;

				case SDL_QUIT:
						quit = 1;
					break;
			}
		}
		
	}

	return quit;
}


void setDefaultValuesForTimeParameters(TimeParameters* timeParameters)
{
	timeParameters->t1 = SDL_GetTicks();
	timeParameters->frames = 0;
	timeParameters->fps = 0;
	timeParameters->fpsTimer = 0;
	timeParameters->worldTime = 0;
	timeParameters->worldTimeInt = 0;
	timeParameters->lastWave = 0;
	timeParameters->lastMissile = 0;
	timeParameters->frameDelay = 1000.0/60.0;
}

void setDefaultValuesForPlayerMissiles(Missile** missiles, int x, int y)
{
	for(int i = 0; i < numberOfPlayerMissiles; i++)
	{
		missiles[i]->x = x;
		missiles[i]->y = y;
		missiles[i]->size = playerMissileSize;
		missiles[i]->existence = false;
		missiles[i]->speed = playerMissileSpeed;
	}
}

void setDefaultValuesForPlayer(Player* player, int x, int y)
{
	player->x = x;
	player->y = y;
	player->size = playerSize;
	player->currentMissile = 0;
	player->combo = 1;
	player->lastHitAt = 0;
	player->lastShotAt = 0;
	player->invincibility = false;
	player->foundBonus = false;
	player->health = maxPlayerHealth;
	player->lastHealth = maxPlayerHealth;
	setDefaultValuesForPlayerMissiles(player->missiles, player->x, player->y);
}

void setDefaultValuesForEnemy(Enemy* enemy, int x, int y)
{
	enemy->x = x;
	enemy->y = y;
	enemy->size = enemySize;
	enemy->combo = 1;
	enemy->lastHitAt = 0;
	enemy->speed = enemySpeed;
	enemy->health = maxEnemyHealth;
	enemy->lastHealth = maxEnemyHealth;
	enemy->movement = false;
}

void setDefaultValuesForMissile(Missile* missile, Enemy* enemy, double angle)
{
	missile->x = enemy->x;
	missile->y = enemy->y;
	missile->size = normalMissileSize;
	missile->angle = angle;
	missile->angleSin = sin(missile->angle);
	missile->angleCos = cos(missile->angle);
	missile->existence = false;
	missile->speed = normalMissileSpeed;
}

void setDefaultValuesForMissiles(Missile*** missiles, Enemy* enemy, Level* level)
{
	for(int i = 0; i < maxNumberOfMissileWaves; i++)
	{
		for(int j = 0; j < maxNumberOfMissilesInWave; j++)
			setDefaultValuesForMissile(missiles[i][j], enemy, (2 * M_PI / level->numberOfMissilesInWaves) * j);
	}
}

void setDefaultValuesForBigMissiles(Missile** bigMissiles, Enemy* enemy)
{
	for(int i = 0; i < maxNumberOfBigMissiles; i++)
	{
		bigMissiles[i]->x = enemy->x;
		bigMissiles[i]->y = enemy->y;
		bigMissiles[i]->size = bigMissileSize;
		bigMissiles[i]->existence = false;
		bigMissiles[i]->speed = bigMissileSpeed;
	}
}

void setDefaultValuesForExplodingMissiles(ExplodingMissile** explodingMissile, Enemy* enemy)
{
	for(int i = 0; i < maxNumberOfExplodingMissiles; i++)
	{
		explodingMissile[i]->x = enemy->x;
		explodingMissile[i]->y = enemy->y;
		explodingMissile[i]->size = normalMissileSize;
		explodingMissile[i]->speed = normalMissileSpeed;
		explodingMissile[i]->existence = false;

		for(int j = 0; j < numberOfMissilesFromExplosion; j++)
			setDefaultValuesForMissile(explodingMissile[i]->normalMissiles[j], enemy, (2 * M_PI / numberOfMissilesFromExplosion) * j);
	}
}

void setDefaultValuesForExplodingField(ExplodingField* explodingField, int size, int delay)
{
	explodingField->size = size;
	explodingField->activationDelay = delay;
	explodingField->existence = false;
}

void setDefaultValuesForExplodingFields(ExplodingField** explodingFields, int size, int delay, int numberOfExplodingFields)
{
	for(int i = 0; i < numberOfExplodingFields; i++)
	{
		setDefaultValuesForExplodingField(explodingFields[i], size, delay);
	}
}

void setDefaultValuesAboutWaves(MissilesParameters* missilesParameters)
{
	missilesParameters->currentCircularWave = 0;
	missilesParameters->currentSpiralWave = 0;
	missilesParameters->currentMissileInSpiralWave = 0;
	missilesParameters->currentExplodingFieldCircle = 0;
	missilesParameters->currentExplodingFieldSquare = 0;
	missilesParameters->currentExplodingMissile = 0;
	missilesParameters->currentBigMissile = 0;
	missilesParameters->currentHealth = 0;
}

void startNewGame(WindowParameters* windowParameters, TimeParameters* timeParameters, MissilesParameters* missilesParameters, Player* player,
				  Enemy* enemy, Missile*** missiles, Missile** bigMissiles, ExplodingMissile** explodingMissiles, ExplodingField** explodingFieldsSquare, ExplodingField** explodingFieldsCircle, ExplodingField** health, Level* level)
{
	setDefaultValuesForTimeParameters(timeParameters);
	setDefaultValuesForPlayer(player, level->playerStartingX, level->playerStartingY);
	setDefaultValuesForEnemy(enemy, level->enemyStartingX, level->enemyStartingY);
	setDefaultValuesForMissiles(missiles, enemy, level);
	setDefaultValuesForBigMissiles(bigMissiles, enemy);
	setDefaultValuesForExplodingMissiles(explodingMissiles, enemy);
	setDefaultValuesForExplodingFields(explodingFieldsSquare, explodingFieldSquareSize, explodingFieldSquareDelay, maxNumberOfExplodingFields);
	setDefaultValuesForExplodingFields(explodingFieldsCircle, explodingFieldCircleSize, explodingFieldCircleDelay, maxNumberOfExplodingFields);
	setDefaultValuesForExplodingFields(health, healthSize, healthDelay, maxNumberOfHealthFields);
	setDefaultValuesAboutWaves(missilesParameters);
	changePositionOfRenderedRectangle(windowParameters->renderedRectangle, player->x - screenWidth/2, player->y - screenHeight/2);
}

void calculateNewValuesForTimeParameters(TimeParameters* timeParameters)
{
	timeParameters->t2 = SDL_GetTicks();
	timeParameters->delta = (timeParameters->t2 - timeParameters->t1) * 0.001;
	timeParameters->t1 = timeParameters->t2;
	timeParameters->worldTime += timeParameters->delta;
	timeParameters->fpsTimer += timeParameters->delta;

	if(timeParameters->fpsTimer > 0.5)
	{
		timeParameters->fps = timeParameters->frames * 2;
		timeParameters->frames = 0;
		timeParameters->fpsTimer -= 0.5;
	}

	timeParameters->worldTimeInt = (int)timeParameters->worldTime;
}

void setValuesForInformationParameters(InformationParameters* informationParameters, WindowParameters* windowParameters)
{
	informationParameters->x = windowParameters->renderedRectangle->x + 4;
	informationParameters->y = windowParameters->renderedRectangle->y + 4;
	informationParameters->width = screenWidth - 8;
	informationParameters->height = 36;
	informationParameters->playerHealthWidth = screenWidth/2 - 8;
	informationParameters->playerHealthHeight = 36;
	informationParameters->playerHealthX = windowParameters->renderedRectangle->x + 4;
	informationParameters->playerHealthY = windowParameters->renderedRectangle->y + screenHeight - informationParameters->playerHealthHeight - 4;
	informationParameters->enemyHealthWidth = screenWidth/2 - 8;
	informationParameters->enemyHealthHeight = 36;
	informationParameters->enemyHealthX = windowParameters->renderedRectangle->x + screenWidth/2 + 4;
	informationParameters->enemyHealthY = windowParameters->renderedRectangle->y + screenHeight - informationParameters->playerHealthHeight - 4;
}

void calculateNewValuesForHealthBars(InformationParameters* informationParameters, Player* player, Enemy* enemy)
{
	informationParameters->playerHealthWidth = (int)((screenWidth/2 - 8) * player->health / maxPlayerHealth);
	informationParameters->enemyHealthWidth = (int)((screenWidth/2 - 8) * enemy->health / maxEnemyHealth);
}

void delayFrames(TimeParameters* timeParameters)
{
	if(timeParameters->delta < timeParameters->frameDelay)
		SDL_Delay(timeParameters->frameDelay - timeParameters->delta);
}

// draw a surface sprite on a surface screenSurface in point (x, y)
// (x, y) is the center of sprite on screenSurface
void drawSurface(SDL_Surface *screenSurface, SDL_Surface *sprite, int x, int y)
{
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screenSurface, &dest);
}

void drawExplodingField(WindowParameters* windowParameters, ExplodingField* explodingField)
{
	drawSurface(windowParameters->screenSurface, explodingField->picture, explodingField->x, explodingField->y);
}

void drawExplodingFields(WindowParameters* windowParameters, ExplodingField** explodingFieldsCircle, ExplodingField** explodingFieldsSquare)
{
	for(int i = 0; i < maxNumberOfExplodingFields; i++)
	{
		if(explodingFieldsCircle[i]->existence == true)
			drawExplodingField(windowParameters, explodingFieldsCircle[i]);	
	}

	for(int i = 0; i < maxNumberOfExplodingFields; i++)
	{
		if(explodingFieldsSquare[i]->existence == true)
			drawExplodingField(windowParameters, explodingFieldsSquare[i]);	
	}
}

void drawHealth(WindowParameters* windowParameters, ExplodingField** health)
{
	for(int i = 0; i < maxNumberOfHealthFields; i++)
		if(health[i]->existence == true)
		{
			drawExplodingField(windowParameters, health[i]);
		}
}

void drawGroupOfMissiles(WindowParameters* windowParameters, Missile** missiles, int numberOfMissilesInGroup)
{
	for(int i = 0; i < numberOfMissilesInGroup; i++)
	{
		if(missiles[i]->existence == true)
			drawSurface(windowParameters->screenSurface, missiles[i]->picture, missiles[i]->x, missiles[i]->y);
	}
}

void drawMissiles(WindowParameters* windowParameters, Missile*** missiles)
{
	for(int i = 0; i < maxNumberOfMissileWaves; i++)
		drawGroupOfMissiles(windowParameters, missiles[i], maxNumberOfMissilesInWave);
}

void drawExplodingMissile(WindowParameters* windowParameters, ExplodingMissile* explodingMissile)
{
	drawSurface(windowParameters->screenSurface, explodingMissile->picture, explodingMissile->x, explodingMissile->y);
}

void drawExplodingMissiles(WindowParameters* windowParameters, ExplodingMissile** explodingMissiles)
{
	for(int i = 0; i < maxNumberOfExplodingMissiles; i++)
	{
		if(explodingMissiles[i]->existence == true)
			drawExplodingMissile(windowParameters, explodingMissiles[i]);
		
		drawGroupOfMissiles(windowParameters, explodingMissiles[i]->normalMissiles, numberOfMissilesFromExplosion);
	}
}

void drawInformationBoard(SDL_Surface* charset, WindowParameters* windowParameters, ColorParameters* colorParameters, TimeParameters* timeParameters, InformationParameters* informationParameters)
{
	char text[maxLengthOfText];
	drawRectangle(windowParameters->screenSurface, informationParameters->x, informationParameters->y, informationParameters->width, informationParameters->height, colorParameters->textColor, colorParameters->textBackgroundColor);
	
	sprintf(text, "Elapsed time = %.1lf s  %.0lf frames / s", timeParameters->worldTime, timeParameters->fps);
	drawString(windowParameters->screenSurface, informationParameters->x + 4, windowParameters->renderedRectangle->y + 10, text, charset);
	sprintf(text, "Esc - exit, n - new game, %s - up, %s - down, %s - left, %s - right, space - fire", upKey, downKey, leftKey, rightKey);
	drawString(windowParameters->screenSurface, informationParameters->x + 4, windowParameters->renderedRectangle->y + 26, text, charset);
}

void drawHealthBars(SDL_Surface* charset, WindowParameters* windowParameters, ColorParameters* colorParameters, InformationParameters* informationParameters, Player* player, Enemy* enemy)
{
	drawRectangle(windowParameters->screenSurface, informationParameters->playerHealthX, informationParameters->playerHealthY, informationParameters->playerHealthWidth, informationParameters->playerHealthHeight, colorParameters->healthColor, colorParameters->boardColor);
	char text[maxLengthOfText];
	drawRectangle(windowParameters->screenSurface, informationParameters->playerHealthX, informationParameters->playerHealthY, informationParameters->playerHealthWidth, informationParameters->playerHealthHeight, colorParameters->healthColor, colorParameters->healthColor);
	sprintf(text, "Player health: %.1f", player->health);
	drawString(windowParameters->screenSurface, informationParameters->playerHealthX + 4, informationParameters->playerHealthY + 10, text, charset);
	drawRectangle(windowParameters->screenSurface, informationParameters->enemyHealthX, informationParameters->enemyHealthY, informationParameters->enemyHealthWidth, informationParameters->enemyHealthHeight, colorParameters->healthColor, colorParameters->healthColor);
	sprintf(text, "Enemy health: %.1f", enemy->health);
	drawString(windowParameters->screenSurface, informationParameters->enemyHealthX + 4, informationParameters->enemyHealthY + 10, text, charset);
}

void drawScore(int* score, Player* player, SDL_Surface* charset, WindowParameters* windowParameters, ColorParameters* colorParameters)
{
	char text[maxLengthOfText];
	sprintf(text, "%i", *score);
	drawString(windowParameters->screenSurface, player->x + player->size/2, player->y - player->size/2 , text, charset);
}

void drawBoard(GameParameters* gameParameters, int* score)
{
	SDL_FillRect(gameParameters->windowParameters->screenSurface, NULL, gameParameters->colorParameters->boardColor);
	drawExplodingFields(gameParameters->windowParameters, gameParameters->explodingFieldsCircle, gameParameters->explodingFieldsSquare);
	drawGroupOfMissiles(gameParameters->windowParameters, gameParameters->player->missiles, numberOfPlayerMissiles);
	drawMissiles(gameParameters->windowParameters, gameParameters->missiles);
	drawGroupOfMissiles(gameParameters->windowParameters, gameParameters->bigMissiles, maxNumberOfBigMissiles);
	drawExplodingMissiles(gameParameters->windowParameters, gameParameters->explodingMissiles);
	drawHealth(gameParameters->windowParameters, gameParameters->health);
	drawSurface(gameParameters->windowParameters->screenSurface, gameParameters->player->picture, gameParameters->player->x, gameParameters->player->y);
	drawSurface(gameParameters->windowParameters->screenSurface, gameParameters->enemy->picture, gameParameters->enemy->x, gameParameters->enemy->y);
	drawInformationBoard(gameParameters->charset, gameParameters->windowParameters, gameParameters->colorParameters, gameParameters->timeParameters, gameParameters->informationParameters);
	drawHealthBars(gameParameters->charset, gameParameters->windowParameters, gameParameters->colorParameters, gameParameters->informationParameters, gameParameters->player, gameParameters->enemy);
	drawScore(score, gameParameters->player, gameParameters->charset, gameParameters->windowParameters, gameParameters->colorParameters);
	SDL_UpdateTexture(gameParameters->windowParameters->screenTexture, NULL, (gameParameters->windowParameters->screenSurface)->pixels, (gameParameters->windowParameters->screenSurface)->pitch);
	SDL_RenderCopy(gameParameters->windowParameters->renderer, gameParameters->windowParameters->screenTexture, gameParameters->windowParameters->renderedRectangle, NULL);
	SDL_RenderPresent(gameParameters->windowParameters->renderer);
}

void firePlayerMissile(Player* player, int lastKey, TimeParameters* timeParameters)
{
	player->missiles[player->currentMissile]->existence = true;
	player->missiles[player->currentMissile]->launchedAt = timeParameters->worldTime;
	player->missiles[player->currentMissile]->launchingX = player->x;
	player->missiles[player->currentMissile]->launchingY = player->y;
	player->missiles[player->currentMissile]->angleSin = (lastKey == up || lastKey == down) ? 0 : (lastKey == left) ? -1 : 1;
	player->missiles[player->currentMissile]->angleCos = (lastKey == left || lastKey == right) ? 0 : (lastKey == up) ? -1 : 1;
}

void handleEvent(int* quit, SDL_Event event, GameParameters* gameParameters, int* currentLevel)
{
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					*quit = 1;
				else if(event.key.keysym.sym == SDLK_UP)
				{
					gameParameters->lastKey = up;
					if(gameParameters->player->y - 2 * playerMoveSize > gameParameters->informationParameters->height)
						gameParameters->player->y -= playerMoveSize;
					if(gameParameters->player->y >= screenHeight/2 && gameParameters->player->y < gameParameters->levels[*currentLevel]->boardHeight - screenHeight/2)
						gameParameters->windowParameters->renderedRectangle->y -= playerMoveSize;
				}
				else if(event.key.keysym.sym == SDLK_DOWN)
				{
					gameParameters->lastKey = down;
					if(gameParameters->player->y + 2 * playerMoveSize < gameParameters->levels[*currentLevel]->boardHeight)
						gameParameters->player->y += playerMoveSize;
					if(gameParameters->player->y > screenHeight/2 && gameParameters->player->y <= gameParameters->levels[*currentLevel]->boardHeight - screenHeight/2)
						gameParameters->windowParameters->renderedRectangle->y += playerMoveSize;
				}
				else if(event.key.keysym.sym == SDLK_LEFT)
				{
					gameParameters->lastKey = left;
					if(gameParameters->player->x - 2 * playerMoveSize > 0)
						gameParameters->player->x -= playerMoveSize;
					if(gameParameters->player->x >= screenWidth/2 && gameParameters->player->x < gameParameters->levels[*currentLevel]->boardWidth - screenWidth/2)
						gameParameters->windowParameters->renderedRectangle->x -= playerMoveSize;
				}
				else if(event.key.keysym.sym == SDLK_RIGHT)
				{
					gameParameters->lastKey = right;
					if(gameParameters->player->x + 2 * playerMoveSize < gameParameters->levels[*currentLevel]->boardWidth)
						gameParameters->player->x += playerMoveSize;
					if(gameParameters->player->x > screenWidth/2 && gameParameters->player->x < gameParameters->levels[*currentLevel]->boardWidth - screenWidth/2)
						gameParameters->windowParameters->renderedRectangle->x += playerMoveSize;
				}
				else if(event.key.keysym.sym == SDLK_n)
				{
					*currentLevel = levelOne;
					startNewGame(gameParameters->windowParameters, gameParameters->timeParameters, gameParameters->missilesParameters,
								 gameParameters->player, gameParameters->enemy, gameParameters->missiles, gameParameters->bigMissiles, gameParameters->explodingMissiles,
								 gameParameters->explodingFieldsSquare, gameParameters->explodingFieldsCircle, gameParameters->health, gameParameters->levels[*currentLevel]);
				}
				else if(event.key.keysym.sym == SDLK_SPACE && gameParameters->timeParameters->worldTime - gameParameters->player->lastShotAt > 0.1)
				{
					firePlayerMissile(gameParameters->player, gameParameters->lastKey, gameParameters->timeParameters);
					gameParameters->player->currentMissile = (gameParameters->player->currentMissile+1)%numberOfPlayerMissiles;
					gameParameters->player->lastShotAt = gameParameters->timeParameters->worldTime;
				}
				break;

			case SDL_QUIT:
				{
					*quit = 1;
					*currentLevel = numberOfLevels;
				}
				break;
		}
	}
}

void fireSpiralOfMissiles(Level* level, Missile*** missiles,  Enemy* enemy, TimeParameters* timeParameters, MissilesParameters* missilesParameters)
{
	if(missilesParameters->currentSpiralWave == missilesParameters->currentCircularWave)
		(missilesParameters->currentSpiralWave+1) % level->numberOfWaves;

	if(timeParameters->lastWave + level->waveDuration < timeParameters->worldTime)
	{
		timeParameters->lastWave = timeParameters->worldTime;
		missilesParameters->currentSpiralWave = (missilesParameters->currentSpiralWave + 1) % level->numberOfWaves;
	}
	else if(timeParameters->lastMissile + level->missilesInterval < timeParameters->worldTime)
	{
		timeParameters->lastMissile = timeParameters->worldTime;

		missiles[missilesParameters->currentSpiralWave][missilesParameters->currentMissileInSpiralWave]->existence = true;
		missiles[missilesParameters->currentSpiralWave][missilesParameters->currentMissileInSpiralWave]->launchedAt = timeParameters->worldTime;
		missiles[missilesParameters->currentSpiralWave][missilesParameters->currentMissileInSpiralWave]->launchingX = enemy->x;
		missiles[missilesParameters->currentSpiralWave][missilesParameters->currentMissileInSpiralWave]->launchingY = enemy->y;

		missilesParameters->currentMissileInSpiralWave = (missilesParameters->currentMissileInSpiralWave + 1) % level->numberOfMissilesInWaves;
	}
}

void fireCircleOfMissiles(Level* level, Missile*** missiles, Enemy* enemy, TimeParameters* timeParameters, MissilesParameters* missilesParameters)
{
	if(missilesParameters->currentSpiralWave == missilesParameters->currentCircularWave)
		(missilesParameters->currentCircularWave+1) % level->numberOfWaves;

	if(timeParameters->lastWave + level->waveDuration < timeParameters->worldTime)
	{
		timeParameters->lastWave = timeParameters->worldTime;
		for(int i = 0; i < level->numberOfMissilesInWaves; i++)
		{
			missiles[missilesParameters->currentCircularWave][i]->existence = true;
			missiles[missilesParameters->currentCircularWave][i]->launchedAt = timeParameters->worldTime;
			missiles[missilesParameters->currentCircularWave][i]->launchingX = enemy->x;
			missiles[missilesParameters->currentCircularWave][i]->launchingY = enemy->y;
		}

		missilesParameters->currentCircularWave = (missilesParameters->currentCircularWave + 1) % level->numberOfWaves;
	}
}

void fireExplodingField(Level* level, ExplodingField** explodingField, TimeParameters* timeParameters, WindowParameters* windowParameters,
						int* currentExplodingField, int numberOfExplodingFields)
{
	explodingField[*currentExplodingField]->x = rand() % level->boardWidth;
	explodingField[*currentExplodingField]->y = rand() % level->boardHeight;
	explodingField[*currentExplodingField]->launchedAt = timeParameters->worldTime;
	explodingField[*currentExplodingField]->existence = true;

	*currentExplodingField = (*currentExplodingField+1) % numberOfExplodingFields;
}

void fireExplodingMissile(ExplodingMissile* explodingMissile, Enemy* enemy, TimeParameters* timeParameters, Level* level, int* currentExplodingMissile)
{
	explodingMissile->existence = true;
	explodingMissile->launchingX = enemy->x;
	explodingMissile->launchingY = enemy->y;
	explodingMissile->destinationX = rand() % level->boardWidth;
	explodingMissile->destinationY = rand() % level->boardHeight;
	explodingMissile->angleSin = (explodingMissile->destinationX - explodingMissile->launchingX)/(sqrt(pow(explodingMissile->destinationX - explodingMissile->launchingX, 2) + pow(explodingMissile->destinationY - explodingMissile->launchingY, 2)));
	explodingMissile->angleCos = (explodingMissile->destinationY - explodingMissile->launchingY)/(sqrt(pow(explodingMissile->destinationX - explodingMissile->launchingX, 2) + pow(explodingMissile->destinationY - explodingMissile->launchingY, 2)));
	explodingMissile->launchedAt = timeParameters->worldTime;

	*currentExplodingMissile = (*currentExplodingMissile+1) % maxNumberOfExplodingMissiles;
}

void fireBigMissile(Missile* bigMissile, Enemy* enemy, TimeParameters* timeParameters, Level* level, int* currentBigMissile)
{
	int angle = rand();
	bigMissile->existence = true;
	bigMissile->launchingX = enemy->x;
	bigMissile->launchingY = enemy->y;
	bigMissile->angleSin = sin(angle);
	bigMissile->angleCos = cos(angle);
	bigMissile->launchedAt = timeParameters->worldTime;

	*currentBigMissile = (*currentBigMissile+1) % maxNumberOfBigMissiles;
}

void moveEnemy(Enemy* enemy, TimeParameters* timeParameters, Level* level)
{
	enemy->oldX = enemy->x;
	enemy->oldY = enemy->y;
	enemy->newX = rand() % level->boardWidth;
	enemy->newY = rand() % level->boardHeight;

	enemy->angleSin = (enemy->newX - enemy->x)/(sqrt(pow(enemy->x - enemy->newX, 2) + pow(enemy->y - enemy->newY, 2)));
	enemy->angleCos = (enemy->newY - enemy->y)/(sqrt(pow(enemy->x - enemy->newX, 2) + pow(enemy->y - enemy->newY, 2)));

	enemy->startedMovingAt = timeParameters->worldTime;

	enemy->movement = true;
}

bool checkContact(int x1, int y1, int size1, int x2, int y2, int size2, int shape)
{
	if(shape = square)
	{
		if(abs(x1 - x2) < (size1 + size2)/2 && abs(y1 - y2) < (size1 + size2)/2)
			return true;
		else
			return false;
	}
	else
	{
		if(sqrt(pow(x1-x2, 2) + pow(y1-y2, 2)) < (size1 + size2) / 2)
			return true;
		else
			return false;
	}
}

bool checkIfExplodingFieldHitPlayer(ExplodingField* explodingField, Player* player, int shape)
{
	if(checkContact(explodingField->x, explodingField->y, explodingField->size, player->x, player->y, player->size, shape) == true)
		return true;
	else
		return false;
}	

void destroyExplodingFields(ExplodingField** explodingFields, TimeParameters* timeParameters, Player* player, int shape)
{
	for(int i = 0; i < maxNumberOfExplodingFields; i++)
	{
		if(explodingFields[i]->activationDelay <= timeParameters->worldTime - explodingFields[i]->launchedAt && explodingFields[i]->existence == true)
		{
			if(checkIfExplodingFieldHitPlayer(explodingFields[i], player, shape) == true && player->invincibility == false)
			{
				player->health -= hitByExplodingField;
				player->invincibility = true;
				player->lastHitAt = timeParameters->worldTime;
			}	
			explodingFields[i]->existence = false;
		}
	}
}

void destroyHealth(ExplodingField** health, TimeParameters* timeParameters, Player* player, int shape)
{
	for(int i = 0; i < maxNumberOfHealthFields; i++)
	{
		if(health[i]->activationDelay <= timeParameters->worldTime - health[i]->launchedAt)
			health[i]->existence = false;
		else if(health[i]->existence == true &&
				health[i]->activationDelay > timeParameters->worldTime - health[i]->launchedAt &&
				checkContact(health[i]->x, health[i]->y, health[i]->size, player->x, player->y, player->size, shape) == true)
		{
			health[i]->existence = false;
			player->health = (player->health + bonusHealth > maxPlayerHealth) ? maxPlayerHealth : player->health + bonusHealth;
			player->foundBonus = true;
		}
			
	}
}

void fireCircleOfNormalMissiles(Missile** missiles, TimeParameters* timeParameters, int x, int y)
{
	for(int i = 0; i < numberOfMissilesFromExplosion; i++)
	{
		missiles[i]->existence = true;
		missiles[i]->launchingX = x;
		missiles[i]->launchingY = y;
		missiles[i]->launchedAt = timeParameters->worldTime;
	}
}

void destroyExplodingMissile(ExplodingMissile* explodingMissile, TimeParameters* timeParameters)
{
	if(abs(explodingMissile->x - explodingMissile->destinationX) <= 10 && abs(explodingMissile->y - explodingMissile->destinationY) <= 10 && explodingMissile->existence == true) // this is a margin for rounding
	{
		fireCircleOfNormalMissiles(explodingMissile->normalMissiles, timeParameters, explodingMissile->x, explodingMissile->y);
		explodingMissile->existence = false;
	}
}

void destroyExplodingMissiles(ExplodingMissile** explodingMissiles, TimeParameters* timeParameters)
{
	for(int i = 0; i < maxNumberOfExplodingMissiles; i++)
		destroyExplodingMissile(explodingMissiles[i], timeParameters);
}

void executeLevel(Level* level, Missile*** missiles, Enemy* enemy, TimeParameters* timeParameters, MissilesParameters* missilesParameters,
				  ExplodingField** explodingFieldsCircle, ExplodingField** explodingFieldsSquare, ExplodingMissile** explodingMissiles,
				  Missile** bigMissiles, ExplodingField** health, WindowParameters* windowParameters, Player* player)
{
	int randomNumberForExplodingFieldSquare = rand() % 100;
	int randomNumberForExplodingFieldCircle = rand() % 100;
	int randomNumberForExplodingMissile = rand() % 100;
	int randomNumberForBigMissile = rand() % 100;
	int randomNumberForHealth = rand() % 100;

	if(((timeParameters->worldTimeInt)/(level->patternInterval))%2 == 0)
		fireSpiralOfMissiles(level, missiles, enemy, timeParameters, missilesParameters);
	else
		fireCircleOfMissiles(level, missiles, enemy, timeParameters, missilesParameters);

	if(randomNumberForExplodingFieldCircle/100.0 < probabilityOfExplodingFieldCircle &&
	   explodingFieldsCircle[missilesParameters->currentExplodingFieldCircle]->existence == false)
		fireExplodingField(level, explodingFieldsCircle, timeParameters, windowParameters, &(missilesParameters->currentExplodingFieldCircle), maxNumberOfExplodingFields);
		
	if(randomNumberForExplodingFieldSquare/100.0 < probabilityOfExplodingFieldSquare &&
	   explodingFieldsSquare[missilesParameters->currentExplodingFieldSquare]->existence == false)
		fireExplodingField(level, explodingFieldsSquare, timeParameters, windowParameters, &(missilesParameters->currentExplodingFieldSquare), maxNumberOfExplodingFields);

	if(randomNumberForExplodingMissile/100.0 < probabilityOfExplodingMissile &&
	   explodingMissiles[missilesParameters->currentExplodingMissile]->existence == false)
		fireExplodingMissile(explodingMissiles[missilesParameters->currentExplodingMissile], enemy, timeParameters, level, &(missilesParameters->currentExplodingMissile));

	if(randomNumberForBigMissile/100.0 < probabilityOfBigMissile &&
	   bigMissiles[missilesParameters->currentBigMissile]->existence == false)
		fireBigMissile(bigMissiles[missilesParameters->currentBigMissile], enemy, timeParameters, level, &(missilesParameters->currentBigMissile));
	
	for(int i = 0; i < maxNumberOfBigMissiles; i++)
	{
		if(bigMissiles[i]->x < 0 || bigMissiles[i]->x > level->boardWidth || bigMissiles[i]->y < 0 || bigMissiles[i]->y > level->boardHeight)
		{
			bigMissiles[i]->x = enemy->x;
			bigMissiles[i]->y = enemy->y;
			bigMissiles[i]->existence = false;
		}
	}

	if(randomNumberForHealth/100.0 < probabilityOfHealth &&
	   health[missilesParameters->currentHealth]->existence == false)
		fireExplodingField(level, health, timeParameters, windowParameters, &(missilesParameters->currentHealth), maxNumberOfHealthFields);

		
	destroyExplodingFields(explodingFieldsCircle, timeParameters, player, circle);
	destroyExplodingFields(explodingFieldsSquare, timeParameters, player, square);
	destroyExplodingMissiles(explodingMissiles, timeParameters);
	destroyHealth(health, timeParameters, player, square);

	if(level->enemyMoving == moving && enemy->movement == false)
		moveEnemy(enemy, timeParameters, level);

}


void calculateNewPositionsForGroupOfMissiles(Missile** missiles, TimeParameters* timeParameters, int numberOfMissilesInGroup)
{
	for(int i = 0; i < numberOfMissilesInGroup; i++)
	{
		if(missiles[i]->existence == true)
		{
			missiles[i]->x = missiles[i]->launchingX + missiles[i]->angleSin * missiles[i]->speed * (timeParameters->worldTime - missiles[i]->launchedAt);
			missiles[i]->y = missiles[i]->launchingY + missiles[i]->angleCos  * missiles[i]->speed * (timeParameters->worldTime - missiles[i]->launchedAt);
		}
	}
}

void calculateNewPositionsForMissiles(Missile*** missiles, TimeParameters* timeParameters)
{
	for(int i = 0; i < maxNumberOfMissileWaves; i++)
		calculateNewPositionsForGroupOfMissiles(missiles[i], timeParameters, maxNumberOfMissilesInWave);
}

void calculateNewPositionsForExplodingMissiles(ExplodingMissile** explodingMissiles, TimeParameters* timeParameters)
{
	for(int i = 0; i < maxNumberOfExplodingMissiles; i++)
	{
		calculateNewPositionsForGroupOfMissiles(explodingMissiles[i]->normalMissiles, timeParameters, numberOfMissilesFromExplosion);

		if(explodingMissiles[i]->existence == true)
		{
			explodingMissiles[i]->x = explodingMissiles[i]->launchingX + explodingMissiles[i]->angleSin * normalMissileSpeed * (timeParameters->worldTime - explodingMissiles[i]->launchedAt);
			explodingMissiles[i]->y = explodingMissiles[i]->launchingY + explodingMissiles[i]->angleCos * normalMissileSpeed * (timeParameters->worldTime - explodingMissiles[i]->launchedAt);
		}
	}
}

void calculateNewPositionForEnemy(Enemy* enemy, TimeParameters* timeParameters)
{
	enemy->x = enemy->oldX + enemy->angleSin * enemy->speed * (timeParameters->worldTime - enemy->startedMovingAt);
	enemy->y = enemy->oldY + enemy->angleCos * enemy->speed * (timeParameters->worldTime - enemy->startedMovingAt);

	if(abs(enemy->x - enemy->newX) <= 10 && abs(enemy->y - enemy->newY) <= 10) // this is a margin for rounding
		enemy->movement = false;
}

void calculateNewPositionsForObjects(GameParameters* gameParameters)
{
	calculateNewPositionsForMissiles(gameParameters->missiles, gameParameters->timeParameters);
	calculateNewPositionsForExplodingMissiles(gameParameters->explodingMissiles, gameParameters->timeParameters);
	calculateNewPositionsForGroupOfMissiles(gameParameters->player->missiles, gameParameters->timeParameters, numberOfPlayerMissiles);
	calculateNewPositionsForGroupOfMissiles(gameParameters->bigMissiles, gameParameters->timeParameters, maxNumberOfBigMissiles);
	if(gameParameters->enemy->movement == moving)
			calculateNewPositionForEnemy(gameParameters->enemy, gameParameters->timeParameters);
}

void checkIfMissileHitPlayer(Missile* missile, TimeParameters* timeParameters, Player* player, int shape, int hitPower)
{
	if(checkContact(missile->x, missile->y, missile->size, player->x, player->y, player->size, shape) == true && missile->existence == true && player->invincibility == false)
	{
		player->health -= hitPower;
		player->invincibility = true;
		player->lastHitAt = timeParameters->worldTime;
		missile->existence = false;
	}
}

void checkIfMissilesHitPlayer(Player* player, Missile*** missiles, Missile** bigMissiles, TimeParameters* timeParameters)
{
	for(int i = 0; i < maxNumberOfMissileWaves; i++)
	{
		for(int j = 0; j < maxNumberOfMissilesInWave; j++)
			checkIfMissileHitPlayer(missiles[i][j], timeParameters, player, square, hitByNormalMissile);
	}

	for(int i = 0; i < maxNumberOfBigMissiles; i++)
		checkIfMissileHitPlayer(bigMissiles[i], timeParameters, player, square, hitByBigMissile);
}

void checkIfExplodingMissileHitPlayer(ExplodingMissile* explodingMissile, TimeParameters* timeParameters, Player* player, int shape)
{
	if(checkContact(explodingMissile->x, explodingMissile->y, explodingMissile->size, player->x, player->y, player->size, shape) == true && explodingMissile->existence == true && player->invincibility == false)
	{
		player->health -= hitByExplodingMissile;
		player->invincibility = true;
		player->lastHitAt = timeParameters->worldTime;
		explodingMissile->existence = false;
	}
}

void checkIfExplodingMissilesHitPlayer(ExplodingMissile** explodingMissiles, TimeParameters* timeParameters, Player* player)
{
	for(int i = 0; i < maxNumberOfExplodingMissiles; i++)
	{
		checkIfExplodingMissileHitPlayer(explodingMissiles[i], timeParameters, player, square);
		for(int j = 0; j < numberOfMissilesFromExplosion; j++)
			checkIfMissileHitPlayer(explodingMissiles[i]->normalMissiles[j], timeParameters, player, square, hitByNormalMissile);
	}
}

void checkIfEnemyWasHit(Missile** missiles, Enemy* enemy, TimeParameters* timeParameters)
{
	for(int i = 0; i < numberOfPlayerMissiles; i++)
	{
		if(checkContact(missiles[i]->x, missiles[i]->y, missiles[i]->size, enemy->x, enemy->y, enemy->size, square) == true && missiles[i]->existence == true)
		{
			enemy->health--;
			enemy->lastHitAt = timeParameters->worldTime;
			missiles[i]->existence = false;
		}
			
	}
}

void checkIfPlayerOrEnemyWereHit(GameParameters* gameParameters)
{
	checkIfMissilesHitPlayer(gameParameters->player, gameParameters->missiles, gameParameters->bigMissiles, gameParameters->timeParameters);
	checkIfExplodingMissilesHitPlayer(gameParameters->explodingMissiles, gameParameters->timeParameters, gameParameters->player);
	checkIfEnemyWasHit(gameParameters->player->missiles, gameParameters->enemy, gameParameters->timeParameters);
}


void calculateScore(int* score, Enemy* enemy, Player* player, TimeParameters* timeParameters)
{
	if(player->foundBonus == true)
	{
		player->foundBonus = false;
		*score += bonusPoints;
		player->lastHealth = player->health;
	}
	else
	{
		*score += (player->health - player->lastHealth) * (player->combo) + (enemy->lastHealth - enemy->health) * (enemy->combo);

		// calculate combo recived by player
		if(player->lastHealth != player->health && player->lastHitAt != 0 && timeParameters->worldTime - player->lastHitAt < maxDifferenceBetweenHits)
		{
			player->combo++;
			player->lastHealth = player->health;
		}

		if(timeParameters->worldTime - player->lastHitAt > maxDifferenceBetweenHits)
			player->combo = 1;
		
		// calculate combo recived by enemy
		if(enemy->lastHealth != enemy->health && enemy->lastHitAt != 0 && timeParameters->worldTime - enemy->lastHitAt < maxDifferenceBetweenHits)
		{
			enemy->combo++;
			enemy->lastHealth = enemy->health;
		}

		if(timeParameters->worldTime - enemy->lastHitAt > maxDifferenceBetweenHits)
			enemy->combo = 1;
	}
}

void removeInvincibility(Player* player, TimeParameters* timeParameters)
{
	if(player->lastHitAt + invincibilityTime < timeParameters->worldTime)
		player->invincibility = false;
}

void displayInformation(GameParameters* gameParameters, int option)
{
	char text[maxLengthOfText];

	changePositionOfRenderedRectangle(gameParameters->windowParameters->renderedRectangle, 0, 0);
	SDL_FillRect(gameParameters->windowParameters->screenSurface, NULL, gameParameters->colorParameters->boardColor);
	drawRectangle(gameParameters->windowParameters->screenSurface, 0, 0, screenWidth, screenHeight, gameParameters->colorParameters->boardColor, gameParameters->colorParameters->boardColor);
	switch (option)
	{
	case gameOver:
		sprintf(text, "Game Over!");
		break;
	
	case levelOne:
		sprintf(text, "Level One!");
		break;

	case levelTwo:
		sprintf(text, "Level Two!");
		break;

	case levelThree:
		sprintf(text, "Level Three!");
		break;
	}
	
	
	drawString(gameParameters->windowParameters->screenSurface, screenWidth/2 - strlen(text) * 8 / 2, screenHeight/2, text, gameParameters->charset);
	SDL_UpdateTexture(gameParameters->windowParameters->screenTexture, NULL, (gameParameters->windowParameters->screenSurface)->pixels, (gameParameters->windowParameters->screenSurface)->pitch);
	SDL_RenderCopy(gameParameters->windowParameters->renderer, gameParameters->windowParameters->screenTexture, gameParameters->windowParameters->renderedRectangle, NULL);
	SDL_RenderPresent(gameParameters->windowParameters->renderer);
}

bool simulateGame(SDL_Event* event, GameParameters* gameParameters, int* currentLevel)
{
	int quit = 0;
	int score = 0;

	while(quit == 0 && gameParameters->player->health > 0 && gameParameters->enemy->health > 0)
	{
		calculateNewValuesForTimeParameters(gameParameters->timeParameters);
		setValuesForInformationParameters(gameParameters->informationParameters, gameParameters->windowParameters);
		calculateNewValuesForHealthBars(gameParameters->informationParameters, gameParameters->player, gameParameters->enemy);
		delayFrames(gameParameters->timeParameters);
		drawBoard(gameParameters, &score);
		handleEvent(&quit, *event, gameParameters, currentLevel);
		executeLevel(gameParameters->levels[*currentLevel], gameParameters->missiles, gameParameters->enemy,
					 gameParameters->timeParameters, gameParameters->missilesParameters, gameParameters->explodingFieldsCircle,
					 gameParameters->explodingFieldsSquare, gameParameters->explodingMissiles, gameParameters->bigMissiles, gameParameters->health, gameParameters->windowParameters, gameParameters->player);
		calculateNewPositionsForObjects(gameParameters);
		checkIfPlayerOrEnemyWereHit(gameParameters);
		calculateScore(&score, gameParameters->enemy, gameParameters->player, gameParameters->timeParameters);
		removeInvincibility(gameParameters->player, gameParameters->timeParameters);
		gameParameters->timeParameters->frames++;
	}

	if(gameParameters->player->health <= 0)
	{
		quit = 1;
		displayInformation(gameParameters, gameOver);
	}
			

	if(quit != 0)
		return false;
	else
		return true;
}

//======================================================================================================================//


#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char** argv)
{
	char charsetBMP[maxLengthOfPath] = "./cs8x8.bmp";
    char playerBMP[maxLengthOfPath] = "./player.bmp";
	char enemyBMP[maxLengthOfPath] = "./enemy1.bmp";
	char missileBMP[maxLengthOfPath] = "./normal_missile.bmp";
	char bigMissileBMP[maxLengthOfPath] = "./big_missile.bmp";
	char explodingMissileBMP[maxLengthOfPath] = "./exploding_missile.bmp";
	char explodingFieldSquareBMP[maxLengthOfPath] = "./exploding_field_square.bmp";
	char explodingFieldCircleBMP[maxLengthOfPath] = "./exploding_field_circle.bmp";
	char playerMissileBMP[maxLengthOfPath] = "./player_missile.bmp";
	char healthBMP[maxLengthOfPath] = "./heart.bmp";
	int menuOption = levelOne;
	GameParameters* gameParameters = allocateMemoryForGameParameters();
	SDL_Event* event = (SDL_Event*)malloc(sizeof(SDL_Event));

	srand(time(NULL));

	if(startSDL(gameParameters->windowParameters) == false)
        return 1;
    else
    {
        configureWindow(gameParameters->windowParameters);

		gameParameters->charset = SDL_LoadBMP(charsetBMP);
		gameParameters->player->picture = SDL_LoadBMP(playerBMP);
		gameParameters->enemy->picture = SDL_LoadBMP(enemyBMP);
		loadPictureForMissiles(gameParameters->missiles, missileBMP, maxNumberOfMissileWaves, maxNumberOfMissilesInWave);
		loadPictureForGroupOfMissiles(gameParameters->bigMissiles, bigMissileBMP, maxNumberOfBigMissiles);
		loadPictureForGroupOfMissiles(gameParameters->player->missiles, playerMissileBMP, numberOfPlayerMissiles);
		loadPictureForExplodingMissiles(gameParameters->explodingMissiles, explodingMissileBMP, missileBMP);
		loadPictureForExplodingFields(gameParameters->explodingFieldsCircle, explodingFieldCircleBMP, maxNumberOfExplodingFields);
		loadPictureForExplodingFields(gameParameters->explodingFieldsSquare, explodingFieldSquareBMP, maxNumberOfExplodingFields);
		loadPictureForExplodingFields(gameParameters->health, healthBMP, maxNumberOfHealthFields);
		
		if(checkIfPicturesLoadedSuccessfully(gameParameters) == false)
			return 1;
		else
		{
			SDL_SetColorKey(gameParameters->charset, true, 0x000000);
			setValuesForColorParameters(gameParameters->windowParameters, gameParameters->colorParameters);
			setValuesForLevels(gameParameters->levels);
			while(handleMenu(*event, &menuOption, gameParameters) != 1)
			{
				for(int currentLevel = menuOption; currentLevel < numberOfLevels; currentLevel++)
				{
					displayInformation(gameParameters, currentLevel);
					SDL_Delay(1000);
					startNewGame(gameParameters->windowParameters, gameParameters->timeParameters, gameParameters->missilesParameters,
							gameParameters->player, gameParameters->enemy, gameParameters->missiles, gameParameters->bigMissiles, gameParameters->explodingMissiles,
							gameParameters->explodingFieldsSquare, gameParameters->explodingFieldsCircle, gameParameters->health, gameParameters->levels[currentLevel]);
					if(simulateGame(event, gameParameters, &currentLevel) == false)
						currentLevel = numberOfLevels;
				}
			}
			
		}
    }

	freeMemory(gameParameters);
	return 0;
}