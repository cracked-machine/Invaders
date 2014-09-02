#include "CommonTools.h"


CommonTools::CommonTools(void)
{
	SCREEN_WIDTH  = 1280;
	SCREEN_HEIGHT = 720;
}


CommonTools::~CommonTools(void)
{
}

void CommonTools::setWidth(int pWidth) {
	this->SCREEN_WIDTH = pWidth;
}

void CommonTools::setHeight(int pHeight) {
	this->SCREEN_HEIGHT = pHeight;
}

int CommonTools::getWidth() {
	return this->SCREEN_WIDTH;
}

int CommonTools::getHeight() {
	return this->SCREEN_HEIGHT;
}

bool CommonTools::HasShipIntersectionF(SDL_Rectf *pR1, SDL_Rectf *pR2) {
	SDL_Rectf R1 = *pR1;
	SDL_Rectf R2 = *pR2;
	return !(R2.x > (R1.x + R1.w) - 40  ||
			(R2.x + R2.w) +25 < R1.x ||
			(R2.y > (R1.y + R1.h) ||
			(R2.y + R2.h) < R1.y)
			);
}

bool CommonTools::HasIntersectionF(SDL_Rectf *pR1, SDL_Rectf *pR2) {
	SDL_Rectf R1 = *pR1;
	SDL_Rectf R2 = *pR2;
	return !(R2.x > (R1.x + R1.w)  ||
			(R2.x + R2.w) < R1.x ||
			(R2.y > (R1.y + R1.h) ||
			(R2.y + R2.h) < R1.y)
			);
}

void CommonTools::logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Window* CommonTools::setupSDL(int pSCREEN_WIDTH, int pSCREEN_HEIGHT) {
	SDL_Window *window = NULL;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		
	}

	window = SDL_CreateWindow("INVADERS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pSCREEN_WIDTH,pSCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (window == nullptr){
		logSDLError(std::cout, "CreateWindow");
		
	}
	
	
	return window;
	
}

SDL_Renderer* CommonTools::setupRenderer(SDL_Window *pWindow) {
	SDL_Renderer *renderer = SDL_CreateRenderer(pWindow, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cout, "CreateRenderer");
		return NULL;
	}
	return renderer;
}


SDL_Texture* CommonTools::loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr)		
		logSDLError(std::cout, "LoadTexture");
	return texture;
}

void CommonTools::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

SDL_Texture* CommonTools::renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}	
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

void CommonTools::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}
