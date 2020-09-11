#pragma once
#include "SDL.h"
class MoveBox
{
public:
	bool needsToMove();
	MoveBox();
	~MoveBox();
	void init(SDL_Rect, SDL_Rect, SDL_Texture*, SDL_Rect, int,int,int);
	void updatePosition();
	static int velocity;
	void render();
	bool empty() {
		return isEmpty;
	}
	void reset();
	
private:


	bool doneMoving;
	SDL_Rect toRect;
	SDL_Texture* Valuetext;
	SDL_Rect moveTextRect;
	SDL_Rect moveRect;
	int r;
	int g;
	int b;
	int xvel;
	int yvel;
	bool isEmpty;



};

