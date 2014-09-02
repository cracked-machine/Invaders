#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "CommonTools.h"
#include "types.h"

class Sprite
{

public:

	

	
public:


	
	Sprite(const std::string &imgFile, SDL_Renderer *ren);
	~Sprite(void);
	
	void setDstf(SDL_Rectf pDstF );
	SDL_Rectf getDstf();

	
	
	bool getYDirection();
	void setYDirection(bool direction);

	

	SDL_Texture* getTexture();
	SDL_Rect getTextureSize();

	bool isDead();

	void logSDLError(std::ostream &os, const std::string &msg);
private:

	bool yDirection;		// true = down, false = up
	bool dead;
	SDL_Rect r;
	SDL_Texture *t;
	
	SDL_Rectf rf;
	
	
};

