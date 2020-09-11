#pragma once
#include "SDL.h"
class GrowBox
{
public:
	bool needsToGrow();
	GrowBox();
	~GrowBox();
	void init(SDL_Rect, SDL_Rect,  int, int, int,int ,int);
	void updateSize();
	static float growBy;
	void render();
	int getRow() {
		return row;
	}
	int getCol() {
		return col;
	}
	void reset();

private:

	bool doneVertical, doneHorizontal;
	bool doneGrowing;
	SDL_Rect toRect;
	SDL_Texture* Valuetext;
	SDL_Rect moveTextRect;
	SDL_Rect growRect;
	int row, col;
	int r;
	int g;
	int b;



};

