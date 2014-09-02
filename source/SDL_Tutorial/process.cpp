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
#include "Level.h"
#include "HelpMode.h"
#include "MenuMode.h"

typedef enum gameMode {MENU = 0, HELP = 1, OPTIONS = 2, LEVEL = 3, KILL = 4};

	
SDL_Window *window = NULL;

bool levelKeyPressed[512] = {false};

int main(int argc, char **argv) {

	CommonTools *utils = new CommonTools();
	window = utils->setupSDL(utils->getWidth(), utils->getHeight());

	if (window == NULL)	{
		utils->logSDLError(std::cout, "SDL Failed to initialise");
		return 1;
	}
	if (TTF_Init() != 0) {
		utils->logSDLError(std::cout, "TTF Failed to initialise");
		return 2;
	}

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )     {
		utils->logSDLError(std::cout, "Audio Failed to initialise");
        return 3;    
    }
	
	bool end = false;
	
	// represents the chosen mode from gameMode enum
	bool modeSelected[5] = {false};
	modeSelected[MENU] = true;
	
	Level *invade = new Level(window);
	HelpMode *helpScreen = new HelpMode(window);
	MenuMode *menuScreen = new MenuMode(window);
	menuScreen->startMusic();

	while (!end){
		if(modeSelected[MENU]) {
			
			int menuSelected = menuScreen->processMenuMode();
			modeSelected[MENU] = false;
			modeSelected[menuSelected] = true;
		}
		if(modeSelected[HELP]) {
			int menuSelected = helpScreen->processHelpMode();
			modeSelected[HELP] = false;
			modeSelected[menuSelected] = true;
		}
		if(modeSelected[LEVEL]) {		
			menuScreen->killMusic();
			int menuSelected = invade->processLevelMode();
			modeSelected[LEVEL] = false;
			modeSelected[menuSelected] = true;
			menuScreen->startMusic();
		}
		if(modeSelected[KILL]) {
			end = true;
		}	
	}

	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();

	return 0;
}