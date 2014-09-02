#include "HelpMode.h"


HelpMode::HelpMode(SDL_Window *pWindow)
{
	utils = new CommonTools();
	window = pWindow;
	fontname = "../res/arial.ttf";
}


HelpMode::~HelpMode(void)
{
}

void HelpMode::cleanupMem() {
	SDL_RenderClear(helpRenderer);
	SDL_RenderPresent(helpRenderer);
	SDL_DestroyTexture(title);
	SDL_DestroyTexture(text1);
	SDL_DestroyTexture(text2);
	SDL_DestroyTexture(text3);
	SDL_DestroyTexture(text4);
	SDL_DestroyTexture(credits);
	SDL_DestroyRenderer(helpRenderer);
	
}

int HelpMode::processHelpMode() {


	
	SDL_Color levelColor = { 255, 255, 255 };
	
	SDL_Event helpEvent;

	helpRenderer = utils->setupRenderer(window);
	if(helpRenderer != NULL) 
	{
		background = utils->loadTexture("../res/background.png", helpRenderer);
		title = utils->loadTexture("../res/Title.png", helpRenderer);
		text1 = utils->renderText("You are the lone defender of planet earth facing endless waves of alien invaders.", fontname, levelColor, 20, helpRenderer);
		text2 = utils->renderText("If you are hit by alien lasers or an invader lands, Earth will be lost!", fontname, levelColor, 20, helpRenderer);
		text3 = utils->renderText("With each level the aliens grow bolder...faster and deadlier!", fontname, levelColor, 20, helpRenderer);
		text4 = utils->renderText("Gain bonus points by scoring multiple hits with one shot!", fontname, levelColor, 20, helpRenderer);
		credits = utils->renderText("Music and coding: @cracked_machine", fontname, levelColor, 10, helpRenderer);

		while (true){
			//check for input events
			while (SDL_PollEvent(&helpEvent))
			{	
				if (helpEvent.type == SDL_QUIT) {
					cleanupMem();
					return KILL;
				}
				if (helpEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					cleanupMem();
					return MENU;
				}
			}

			SDL_RenderClear(helpRenderer);

			utils->renderTexture(background, helpRenderer, 0, 0, (int)utils->getWidth(), (int)utils->getHeight());
			int titleWidth = (int)utils->getWidth()-50;
			utils->renderTexture(title, helpRenderer, (int)(utils->getWidth()-titleWidth)/2, 40, titleWidth, 100);

			utils->renderTexture(text1, helpRenderer, (int)(utils->getWidth() / 15), (int)((utils->getHeight() / 3) + 30));
			utils->renderTexture(text2, helpRenderer, (int)(utils->getWidth() / 10), (int)((utils->getHeight() / 3) + 80));
			utils->renderTexture(text3, helpRenderer, (int)(utils->getWidth() / 7), (int)((utils->getHeight() / 3) + 130));
			utils->renderTexture(text4, helpRenderer, (int)(utils->getWidth() / 6),(int)((utils->getHeight() / 3) + 180));

			SDL_RenderPresent(helpRenderer);
		}

		cleanupMem();

		return MENU;
	}
	return MENU;
}