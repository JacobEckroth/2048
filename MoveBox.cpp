#include "MoveBox.h"
#include <cmath>
#include "Board.h"
int MoveBox::velocity = 20;

bool MoveBox::needsToMove() {
	return !doneMoving;
}



MoveBox::MoveBox() {
	isEmpty = true;
}


MoveBox::~MoveBox() {

}


void MoveBox::init(SDL_Rect fromRect, SDL_Rect toRect, SDL_Texture* Valuetext, SDL_Rect textRect, int r, int g, int b) {
	

	this->moveRect = fromRect;
	this->toRect = toRect;
	this->Valuetext = Valuetext;

	this->moveTextRect = textRect;
	this->r = r;
	this->g = g;
	this->b = b;
	doneMoving = false;
	if (moveRect.x < toRect.x) {
		xvel = 1;
	}
	else if (moveRect.x > toRect.x) {
		xvel = -1;
	}
	else {
		xvel = 0;
	}
	if (moveRect.y < toRect.y) {
		yvel = 1;
	}
	else if (moveRect.y > toRect.y) {
		yvel = -1;
	}
	else {
		yvel = 0;
	}
	isEmpty = false;




}

void MoveBox::reset() {
	if (!isEmpty) {
		SDL_DestroyTexture(Valuetext);
	}
	isEmpty = true;
	
}


void MoveBox::updatePosition() {
	if (std::abs(toRect.x - moveRect.x) >= velocity || std::abs(toRect.y - moveRect.y) >= velocity) {
		moveRect.x += velocity * xvel;
		moveRect.y += velocity * yvel;
		moveTextRect.x += velocity * xvel;
		moveTextRect.y += velocity * yvel;
	}
	else {
		float Xdifference = toRect.x - moveRect.x;
		float Ydifference = toRect.y - moveRect.y;
		moveRect.x += Xdifference;
		moveRect.y += Ydifference;
		moveTextRect.x += Xdifference;
		moveTextRect.y += Ydifference;
		
		doneMoving = true;
	
	}
}

void MoveBox::render() {
	SDL_SetRenderDrawColor(Board::renderer, r, g, b, 255);
	SDL_RenderFillRect(Board::renderer, &moveRect);
	
	SDL_RenderCopy(Board::renderer, Valuetext, NULL, &moveTextRect);


}
