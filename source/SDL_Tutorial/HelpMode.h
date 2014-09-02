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



class HelpMode
{
public:
	HelpMode(SDL_Window *pWindow);
	~HelpMode(void);
	void cleanupMem();
	int processHelpMode();
	
private:
	SDL_Window *window;
	SDL_Renderer *helpRenderer;
	CommonTools *utils;

	std::string fontname;
	typedef enum gameMode {MENU = 0, HELP = 1, OPTIONS = 2, LEVEL = 3, KILL = 4};

	SDL_Texture *background;
	SDL_Texture *title;
	SDL_Texture *text1;
	SDL_Texture *text2;
	SDL_Texture *text3;
	SDL_Texture *text4;
	SDL_Texture *credits;
	Mix_Music *helpMixer;

};

