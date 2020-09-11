#include "GrowBox.h"
#include <cmath>
#include "Board.h"
#include <iostream>

float GrowBox::growBy = (Board::boxCols > Board::boxCols) ? Board::boxCols * .5 : Board::boxRows * .5;


GrowBox::GrowBox() {

}

GrowBox::~GrowBox() {

}

bool GrowBox::needsToGrow() {
	return !doneGrowing;
}

void GrowBox::render() {
	SDL_SetRenderDrawColor(Board::renderer, r, g, b, 255);
	SDL_RenderFillRect(Board::renderer, &growRect);

}


void GrowBox::updateSize() {

	if (!doneGrowing) {
		if (!doneHorizontal) {
			if (std::abs(growRect.w - toRect.w) > growBy) {
				growRect.w += growBy;
				growRect.x -= growBy / 2;
				
			}
			else {
				growRect.w = toRect.w;
				growRect.x = toRect.x;
				doneHorizontal = true;
				
			}
			
		}
		if (!doneVertical) {
			if (std::abs(growRect.h - toRect.h) > growBy) {
				growRect.h += growBy;
				growRect.y -= growBy / 2;
			}
			else {
				growRect.h = toRect.h;
				growRect.y = toRect.y;
				doneVertical = true;
			}
			
			
		}
		if (doneVertical && doneHorizontal) {
			doneGrowing = true;
		}
	}


}

void GrowBox::reset() {
	SDL_DestroyTexture(Valuetext);
}


void GrowBox::init(SDL_Rect fromRect, SDL_Rect toRect, int r, int g, int b,int row, int col) {
	this->row = row;
	this->col = col;
	
	doneVertical = false;
	doneHorizontal = false;
	this->growRect = fromRect;
	this->toRect = toRect;
	

	

	this->r = r;
	this->g = g;
	this->b = b;
	doneGrowing= false;

}