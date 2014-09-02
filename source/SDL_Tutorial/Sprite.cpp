#include "Sprite.h"



Sprite::Sprite(const std::string &imgFile, SDL_Renderer *ren)
{
	
	this->t = IMG_LoadTexture(ren, imgFile.c_str());
	if (t == nullptr)		
		logSDLError(std::cout, "Sprite::Sprite() Error");
	
	// set default 0,0 position and dimensions to size of texture
	int w, h;
	SDL_QueryTexture(this->t, NULL, NULL, &w, &h );
	SDL_QueryTexture(this->t, NULL, NULL, &r.w, &r.h );
	rf.w = (float)w;
	rf.h = (float)h;
	
}


Sprite::~Sprite(void)
{
	SDL_DestroyTexture(t);
}

SDL_Rect Sprite::getTextureSize() {
	
	return this->r;
}



void Sprite::logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}


SDL_Rectf Sprite::getDstf() {
	return this->rf;
}


void Sprite::setDstf(SDL_Rectf pDst) {
	this->rf = pDst;
}



bool Sprite::getYDirection() {
	return yDirection;
}

void Sprite::setYDirection(bool direction) {
	// true = down, false = up
	this->yDirection = direction;
}


bool Sprite::isDead() {
	return dead;
}

SDL_Texture* Sprite::getTexture() {
	return this->t;
}


