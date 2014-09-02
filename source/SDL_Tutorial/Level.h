#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Sprite.h"
#include "CommonTools.h"

class Level
{

public:
	Level(SDL_Window *pWindow);
	~Level(void);

	void cleanupMem();
	void doLevelBanner();
	int processLevelMode();
	void createAlienFleet(SDL_Renderer *pRenderer);
	int processBombs(SDL_Renderer *pRenderer);
	int processInvaders(SDL_Renderer *pRenderer);
	int processInvaders2(SDL_Renderer *pRenderer);
	void processMissile(SDL_Renderer *pRenderer, Mix_Chunk *pBonus, Mix_Chunk *pExplosion);

private:
	#define ENABLE_BOMBS 1;

	Sprite *ship;
	std::vector<Sprite*> vInvaderFleet;
	std::vector<Sprite*> vAmmo;
	std::vector<Sprite*> vBombs;

	SDL_Window *window;
	SDL_Renderer *levelRenderer;
	CommonTools *utils;

	// various game stats
	double fleetCols;
	double fleetRows;
	double invaderspeed;
	double bombspeed;
	double missilespeed;
	int bombOdds;
	int gamelevel;
	long score;

	bool dead;
	bool won;
	bool quit;
	bool invaderHit;
	bool blinkText;
	int blinkCounter;

	
	typedef enum gameMode {MENU = 0, HELP = 1, OPTIONS = 2, LEVEL = 3, KILL = 4};
	
	std::string fontname;

	SDL_Texture *winner;
	SDL_Texture *gameover;
	SDL_Texture *exship;
	SDL_Texture *background;

	Mix_Chunk *laser;
	Mix_Chunk *explosion;
	Mix_Chunk *bonus;
	Mix_Music *mixer;

	SDL_Texture *levelImage;
	SDL_Texture *scoreImage;
	SDL_Texture *warning;
	SDL_Texture *levelBanner;
};

