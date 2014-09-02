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
#include "types.h"

class CommonTools
{
public:
	CommonTools(void);
	~CommonTools(void);

	SDL_Window* setupSDL(int pSCREEN_WIDTH, int pSCREEN_HEIGHT);
	SDL_Renderer* setupRenderer(SDL_Window *pWindow);
	void logSDLError(std::ostream &os, const std::string &msg);
	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
	SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
		SDL_Color color, int fontSize, SDL_Renderer *renderer);
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

	bool HasIntersectionF(SDL_Rectf *pR1, SDL_Rectf *pR2);
	bool HasShipIntersectionF(SDL_Rectf *pR1, SDL_Rectf *pR2);

	void setWidth(int pWidth); 
	void setHeight(int pHeight);
	int getWidth(); 
	int getHeight(); 

	

	

private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	
};

