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

#include "CommonTools.h"

class MenuMode
{
public:
	MenuMode(SDL_Window *pWindow);
	~MenuMode(void);
	void cleanupMem();
	int processMenuMode();
	void killMusic();
	void startMusic();
private:

	SDL_Window *window;
	SDL_Renderer *menuRenderer;
	CommonTools *utils;
	std::string fontname;

	

	typedef enum gameMode {MENU = 0, HELP = 1, OPTIONS = 2, LEVEL = 3, KILL = 4};

	SDL_Texture *background;
	SDL_Texture *title;
	SDL_Texture *option1;
	SDL_Texture *option2;
	SDL_Texture *option3;
	SDL_Texture *credits;

	Mix_Music *menuMixer;
	
	Sprite *randomShipA;
	Sprite *randomShipB;
};

