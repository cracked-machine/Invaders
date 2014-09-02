#include "MenuMode.h"


MenuMode::MenuMode(SDL_Window *pWindow)
{
	utils = new CommonTools();
	window = pWindow;
	fontname = "../res/arial.ttf";

}


MenuMode::~MenuMode(void)
{
}

void MenuMode::cleanupMem() {
	randomShipA = NULL;
	randomShipB = NULL;
	SDL_RenderClear(menuRenderer);
	SDL_RenderPresent(menuRenderer);
	SDL_DestroyTexture(title);
	SDL_DestroyTexture(option1);
	SDL_DestroyTexture(option2);
	SDL_DestroyTexture(option3);
	SDL_DestroyTexture(credits);
	SDL_DestroyRenderer(menuRenderer);
	
}

void MenuMode::killMusic(){
	Mix_FreeMusic(menuMixer);
}

void MenuMode::startMusic() {
	menuMixer = Mix_LoadMUS("../res/Panst.wav");
	Mix_VolumeMusic(5);
	Mix_PlayMusic( menuMixer, -1 );
}

int MenuMode::processMenuMode() {
	
	
	SDL_Color levelColor = { 255, 255, 255 };
	menuRenderer = utils->setupRenderer(window);
	if(menuRenderer != NULL) {
		SDL_Event menuEvent;

		background = utils->loadTexture("../res/background.png", menuRenderer);
		title = utils->loadTexture("../res/Title.png", menuRenderer);
		option1 = utils->loadTexture("../res/START.png", menuRenderer);
		option2 = utils->loadTexture("../res/HELP.png", menuRenderer);
		option3 = utils->loadTexture("../res/EXIT.png", menuRenderer);
		credits = utils->renderText("Music and coding: @cracked_machine", fontname, levelColor, 10, menuRenderer);
		
		randomShipA = new Sprite("../res/invader.png", menuRenderer);
		randomShipB = new Sprite("../res/Invader.png", menuRenderer);
		int a = 0;
		int b = (int)utils->getWidth();
		bool goRight = true;
		bool goLeft = true;
		int sizeA = rand() % 30;
		int sizeB = rand() % 30+10;

		while (true){
			//check for input events
			while (SDL_PollEvent(&menuEvent))
			{	
				if (menuEvent.type == SDL_QUIT) {
					cleanupMem();
					return KILL;
				
				}
				if (menuEvent.key.keysym.scancode == SDL_SCANCODE_1) {	// start game
					cleanupMem();
					return LEVEL;
				}
				if (menuEvent.key.keysym.scancode == SDL_SCANCODE_2) {	
					cleanupMem();
					return HELP;
				}

				if (menuEvent.key.keysym.scancode == SDL_SCANCODE_3) {	
					cleanupMem();
					return KILL;
				}
			}

	
			//SDL_Texture *title = renderText("INVADERS", fontname, levelColor, 75, renderer);
		
				
		
			utils->renderTexture(background, menuRenderer, 0, 0, utils->getWidth(), utils->getHeight());
			

			// start of the render chain
			SDL_RenderClear(menuRenderer);
			
			utils->renderTexture(background, menuRenderer, 0, 0, utils->getWidth(), utils->getHeight());
			 
			
			if (a < utils->getWidth() && goRight) {
				a++;
				utils->renderTexture(randomShipA->getTexture(), menuRenderer, a, 200, sizeA, sizeA);
			}
			else {
				goRight = false;
				a--;
				utils->renderTexture(randomShipA->getTexture(), menuRenderer, a, 500, sizeA, sizeA);
				if(a < 0 && !goRight) {
					goRight = true;
					sizeA = rand() % 30;
					
				}
			}

			if (b > 0 && goLeft) {
				b-=1;
				utils->renderTexture(randomShipB->getTexture(), menuRenderer, b, 400, sizeB, sizeB);
			}
			else {
				goLeft = false;
				b+=1;
				utils->renderTexture(randomShipB->getTexture(), menuRenderer, b, 300, sizeB, sizeB);
				if(b > utils->getWidth() && !goLeft) {
					goLeft = true;
					sizeB = rand() % 30+10;
					
				}
			}
			int titleWidth = utils->getWidth();-50;
			utils->renderTexture(title, menuRenderer, (utils->getWidth()-titleWidth)/2, 40, titleWidth, 100);
		
			//renderTexture(title, renderer, 10, 70);
			utils->renderTexture(option1, menuRenderer, 30, (utils->getHeight() / 3) + 30);
			utils->renderTexture(option2, menuRenderer, 30, (utils->getHeight() / 3) + 90);
			utils->renderTexture(option3, menuRenderer, 30, (utils->getHeight() / 3) + 150);
			utils->renderTexture(credits, menuRenderer, 30, utils->getHeight() - 15);

		
			// end of the render chain
			SDL_RenderPresent(menuRenderer);
		}
		
	}
	return MENU;
}
