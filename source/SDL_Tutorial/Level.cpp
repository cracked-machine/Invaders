#include "Level.h"


Level::Level(SDL_Window *pWindow)
{
	
	
	fleetRows = 10;
	fleetCols = 15;
	invaderspeed = 2.999;
	bombspeed = 1.999;
	missilespeed = 4.999;
	bombOdds = 19999;
	gamelevel = 1;
	score = 0;
	blinkText = true;

	fontname = "../res/arial.ttf";
	

	ship = NULL;
	utils = new CommonTools();
	window = pWindow;
}


Level::~Level(void)
{
}

void Level::cleanupMem() {


	SDL_RenderClear(levelRenderer);
	SDL_RenderPresent(levelRenderer);
	vAmmo.clear();
	vInvaderFleet.clear();
	vBombs.clear();
	ship = NULL;
	SDL_DestroyTexture(winner);
	SDL_DestroyTexture(gameover);
	SDL_DestroyTexture(exship);
	SDL_DestroyTexture(background);
	//SDL_DestroyTexture(levelImage);
	//SDL_DestroyTexture(warning);
	//SDL_DestroyTexture(scoreImage);
				
	SDL_DestroyRenderer(levelRenderer);
	Mix_FreeMusic(mixer);
}

void Level::doLevelBanner() {

	SDL_RenderClear(levelRenderer);
	SDL_Color cLevelBanner = { 255, 255, 255 };
	std::stringstream tLevelBanner;
	tLevelBanner << "LEVEL " << gamelevel;
	

	for (int t=0; t< 250; t++) {
		levelBanner = utils->renderText(tLevelBanner.str(), fontname, cLevelBanner, 800, levelRenderer);
		utils->renderTexture(levelBanner, levelRenderer, (int)(utils->getWidth() - t*20), -70);
		SDL_RenderPresent(levelRenderer);
		SDL_DestroyTexture(levelBanner);
		SDL_RenderClear(levelRenderer);
	}

	
}

int Level::processLevelMode() {
	
	
	SDL_Color levelColor = { 255, 255, 255 };
	bool levelKeyPressed[512] = {false};
	levelRenderer = utils->setupRenderer(window);

	//doLevelBanner();

	winner = utils->loadTexture("../res/won.png", levelRenderer);
	gameover = utils->loadTexture("../res/gameover.png", levelRenderer);
	exship = utils->loadTexture("../res/exship.png", levelRenderer);
	background = utils->loadTexture("../res/background.png", levelRenderer);

	Mix_Chunk *laser = Mix_LoadWAV("../res/laser.wav");
	Mix_Chunk *explosion = Mix_LoadWAV("../res/explosion.wav");
	Mix_Chunk *bonus = Mix_LoadWAV("../res/bonus.wav");
	mixer = Mix_LoadMUS("../res/balls.wav");
	Mix_VolumeMusic(5);
	Mix_Volume(-1, 1);
	Mix_PlayMusic( mixer, -1 );

	// Create the cursor sprite and set start pos
	ship = new Sprite("../res/spaceship.png", levelRenderer);
	SDL_Rectf hPos = ship->getDstf();
	hPos.x = utils->getWidth() / 2;
	hPos.y = utils->getHeight() - (hPos.h /4 );
	ship->setDstf(hPos);

	std::stringstream leveltext;
	leveltext << "Level " << gamelevel;
	//levelImage = utils->renderText(leveltext.str(), fontname, levelColor, 30, levelRenderer);

	bool invasion = false;

	/// create alien fleet
	createAlienFleet(levelRenderer);
	int red = 0;
	SDL_Event levelEvent;
	SDL_FlushEvent(SDL_KEYDOWN);
	while (true){

		
		//update the score on screen
		std::stringstream sScore;
		sScore << "Score:" << score;
		scoreImage = utils->renderText(sScore.str(), fontname, levelColor, 50, levelRenderer);

		
		
		//scoreImage = NULL;

		//check for input events
		while (SDL_PollEvent(&levelEvent))
		{	
			if (levelEvent.type == SDL_QUIT) {
				cleanupMem();
				return KILL;
			}
			if (levelEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				cleanupMem();
				return MENU;
			}
			
			if (levelEvent.type == SDL_KEYDOWN) {
				levelKeyPressed[levelEvent.key.keysym.scancode] = true;					
			}
			if (levelEvent.type == SDL_KEYUP) 
				levelKeyPressed[levelEvent.key.keysym.scancode] = false;
		}


		utils->renderTexture(background, levelRenderer, 0, 0, (int)utils->getWidth(), (int)utils->getHeight());

		
		// If the level has been run increment the level.
		if (vInvaderFleet.empty()) { 

			won = true;
			
			
			
			
			// increase difficulty
			invaderspeed += 0.2;
			bombspeed += 0.2;
			bombOdds -= 50;
		
			//reset the game area
			vAmmo.clear();
			createAlienFleet(levelRenderer);

			gamelevel += 1;

			doLevelBanner();
			
			std::stringstream leveltext;
			leveltext << "Level " << gamelevel;
			//levelImage = utils->renderText(leveltext.str(), fontname, levelColor, 30, levelRenderer);
		}

		


		
		
		// start of the render chain
		SDL_RenderClear(levelRenderer);
		
		utils->renderTexture(background, levelRenderer, 0, 0, (int)utils->getWidth(), (int)utils->getHeight());
		

		if(processBombs(levelRenderer) != 0) { ///returns true if bomb hits player
			
			SDL_RenderClear(levelRenderer);
			int gW, gH;
			SDL_QueryTexture(gameover, NULL, NULL, &gW, &gH);
			utils->renderTexture(gameover, levelRenderer, (int)(utils->getWidth() - gW) / 2, (int)(utils->getHeight() / 3));
			SDL_RenderPresent(levelRenderer);
			
			SDL_Delay(3000);
			SDL_RenderClear(levelRenderer);
			SDL_RenderPresent(levelRenderer);
			
			cleanupMem();
			return MENU;
		}

		if(processInvaders(levelRenderer)) {  // returns true if invader reaches earth

			SDL_RenderClear(levelRenderer);
			int gW, gH;
			SDL_QueryTexture(gameover, NULL, NULL, &gW, &gH);
			utils->renderTexture(gameover, levelRenderer, (int)(utils->getWidth() - gW) / 2, (int)(utils->getHeight() / 3));
			SDL_RenderPresent(levelRenderer);
			
			SDL_Delay(3000);
			SDL_RenderClear(levelRenderer);
			SDL_RenderPresent(levelRenderer);
			
			cleanupMem();
			return MENU;
		}


		//utils->renderTexture(levelImage, levelRenderer, 10, 10);

		/// DO NOT SEPARATE - WILL CAUSE MEMORY LEAK!!!!   ///////////////////////////////
		utils->renderTexture(scoreImage, levelRenderer, 10, (int)(utils->getHeight()-60));		////// 
		SDL_DestroyTexture(scoreImage);												//////
		//////////////////////////////////////////////////////////////////////////////////

		processMissile(levelRenderer, bonus, explosion);


		// get the player sprite position and update if keys are pressed
		SDL_Rectf sPos = ship->getDstf();					
		if (levelKeyPressed[SDL_SCANCODE_LEFT])
			sPos.x = sPos.x - 5.2; ship->setDstf(sPos);

		if (levelKeyPressed[SDL_SCANCODE_RIGHT])  
			sPos.x = sPos.x + 5.2; ship->setDstf(sPos);
				
		if (levelKeyPressed[SDL_SCANCODE_SPACE]) {
			
			
			// create a new missile sprite set the pos to equl current ship pos, add it to the vAmmo vector
			if (vAmmo.size() < 1) {

				if(Mix_PlayChannel(-1, laser, 0) == -1) {
					utils->logSDLError(std::cout, "Unable to play laser.wav");
				
				}
				Sprite *m = new Sprite("../res/Missile.png", levelRenderer);
				SDL_Rectf mPos = m->getDstf();
				mPos.x = ship->getDstf().x;
				mPos.y = ship->getDstf().y - (ship->getDstf().h / 2);
				m->setDstf(mPos);
				vAmmo.push_back(m);	
			}		
		}
		// Draw the ship at current pos
		utils->renderTexture(ship->getTexture(), levelRenderer, (int)((ship->getDstf().x) - ship->getDstf().w / 2), (int)((ship->getDstf().y) - ship->getDstf().h / 2));
		

		red += 12;
		SDL_Color wColor = {red, 0, 0};
		invasion=false;
		warning = utils->renderText("!!! INVASION IMMINENT !!!", "../res/arialbi.ttf", wColor, 100, levelRenderer);
		for (std::vector<Sprite*>::iterator iIt = vInvaderFleet.begin(); iIt != vInvaderFleet.end(); ++iIt ) {
			// get the Sprite object's last stored pos
			SDL_Rectf iPos = (*iIt)->getDstf();			
			if(iPos.y > (utils->getHeight() - iPos.h * 6)) 
				invasion = true;
			
		}
		if(invasion)
		///// DO NOT SEPARATE THESE TWO - WILL CAUSE MEMORY LEAKS ////////
			utils->renderTexture(warning, levelRenderer, 10, 10);	//////
		//////////////////////////////////////////////////////////////////
																	//////
		SDL_RenderPresent(levelRenderer);							//////
																	//////
		//////////////////////////////////////////////////////////////////
		SDL_DestroyTexture(warning);								//////
		//////////////////////////////////////////////////////////////////

		SDL_RenderClear(levelRenderer);
		
	}
}

void Level::createAlienFleet(SDL_Renderer *pRenderer) {

	std::string greenalien = "../res/invaderGreen.png";
	std::string redalien = "../res/invaderRed.png";
	std::string bluealien = "../res/invaderBlue.png";

	int heightOffset = 0;
	bool direction = true;
	// Create the vector of invader sprites
	for (int row = 0;row<fleetRows;row++) {
		// initialise sprite with Invader pic
		
		for (int col = 0; col<fleetCols; col++) {
			Sprite *i = new Sprite("../res/invader.png", pRenderer);
			//set position in line
			SDL_Rectf iPos = i->getDstf();
			iPos.x = (((utils->getWidth() - iPos.w*4) / fleetCols) * col) + iPos.w*2 ;
		
			//iPos.y = (utils->getHeight() - 150);
			iPos.y = (((utils->getHeight()-heightOffset) / iPos.h*1.6) * row) + iPos.h;
			i->setDstf(iPos);

	
			i->setYDirection(false); direction = false; 
			
			//add the new Invader sprite to the vector
			vInvaderFleet.push_back(i);
		}
	}
}



int Level::processInvaders(SDL_Renderer *pRenderer) {
	//////////////////////////////////////////////
	// process INVADERS in vInvaderFleet vector

	for (std::vector<Sprite*>::iterator iIt = vInvaderFleet.begin(); iIt != vInvaderFleet.end(); ++iIt ) {
		// get the Sprite object's last stored pos
		SDL_Rectf iPos = (*iIt)->getDstf();

		//change the Sprites direction if we hit the screen boundary
		if (iPos.x <= iPos.w*2) {
			(*iIt)->setYDirection(true);
			// lower the sprite for every pass
			iPos.y += iPos.h;
		}
		if (iPos.x > (utils->getWidth() - iPos.w*2)) {
			(*iIt)->setYDirection(false);
			// lower the sprite for every pass
			iPos.y += iPos.h;
		}
		
		// blink warning text when invaders get close - 8 x invader height
		int blinkInterval = 200;
		if(iPos.y > (utils->getHeight() - iPos.h * 6)) {
			
			
			//utils->renderTexture(warning, pRenderer, utils->getWidth() / 5, 5);
			//SDL_DestroyTexture(warning);
			
			
		}
		// THE INVADERS HAVE LANDED! signal to processLevelMode that game is over
		if(iPos.y > utils->getHeight() - iPos.h * 2)
			return 1;

		//increment or decrement the x depending on the Sprite's Y direction setting. But make sure it doesn't go beyond boundary
		if (((*iIt)->getYDirection()) && (iPos.x - invaderspeed <= (utils->getWidth() - (iPos.w*2))))
			iPos.x += invaderspeed;
		else if (iPos.x + invaderspeed > iPos.w*2) 
			iPos.x -= invaderspeed;

			
#if ENABLE_BOMBS == 1
		// calculate the chance of dropping a bomb every update
		int bombFreq = rand() % bombOdds;
		if (bombFreq < 1) {
			Sprite *bomb = new Sprite("../res/bomb.png", pRenderer);
			SDL_Rectf bPos = bomb->getDstf();
			bPos.x = iPos.x;
			bPos.y = iPos.y;
			bomb->setDstf(bPos);
			vBombs.push_back(bomb);
		}
#endif

		// draw current invader in its current pos
		utils->renderTexture((*iIt)->getTexture(), pRenderer, (int)(iPos.x - (iPos.w / 2)),  (int)(iPos.y - (iPos.h / 2)));
		//update the Sprite with its new pos
		(*iIt)->setDstf(iPos);
	}

	return 0;
}

int Level::processBombs(SDL_Renderer *pRenderer) {
	////////////////////////////////////
	// process BOMBS in vBombs vector
	
	for (std::vector<Sprite*>::iterator bIt = vBombs.begin(); bIt != vBombs.end(); ) {
		// get the Sprite object's last stored pos
		SDL_Rectf bPos = (*bIt)->getDstf();

		
		if(utils->HasShipIntersectionF(&ship->getDstf(), &bPos) ) {
			/// OH NOES YOU GOT BLOWN UP!!!!
			
			return 1;

		}

		// move the current bomb down the screen
		bPos.y = bPos.y + bombspeed;

				
		utils->renderTexture((*bIt)->getTexture(), pRenderer, (int)(bPos.x - (bPos.w / 2)), (int)(bPos.y - (bPos.h / 2)));
		(*bIt)->setDstf(bPos);

		if (bPos.y > utils->getHeight()) {
			bIt = vBombs.erase(bIt);
				
		}
		else {
			++bIt;
		}
		
	}
	return 0;
}

void Level::processMissile(SDL_Renderer *pRenderer, Mix_Chunk *pBonus, Mix_Chunk *pExplosion) {
	/////////////////////////////////////
	// process MISSILES in vAmmo vector

	for (std::vector<Sprite*>::iterator mIt = vAmmo.begin(); mIt != vAmmo.end(); ) {
		SDL_Rectf mPos = (*mIt)->getDstf();
	
		invaderHit = false;
		for (std::vector<Sprite*>::iterator iIt = vInvaderFleet.begin(); iIt != vInvaderFleet.end(); ) {
			SDL_Rectf iPos = (*iIt)->getDstf();
			
			if(utils->HasIntersectionF(&mPos, &iPos)) {

				invaderHit =true;
				iIt = vInvaderFleet.erase(iIt);				// COLLISION!! BYE BYE INVADER!!
				
				score += 10;
				
				if(Mix_PlayChannel(-1, pExplosion, 0) == -1) 
					utils->logSDLError(std::cout, "Unable to play explosion.wav");
			}				
			else
				++iIt;
		}

		if(mPos.y < 0) {
			mIt = vAmmo.erase(mIt);
			
		}
		else {
			// draw current missile in current pos
			utils->renderTexture((*mIt)->getTexture(), pRenderer, (int)(mPos.x - (mPos.w / 2)), (int)(mPos.y - (mPos.h / 2)));
			// continue to move current missile up the screen
			mPos.y = mPos.y - missilespeed;
			//write pos change to sprite
			(*mIt)->setDstf(mPos);

			++mIt;
		}
		



	}

}
