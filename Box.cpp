#include "Box.h"
#include "Board.h"
#include "SDL_ttf.h"
Box::Box(int row, int col) {
	this->row = row;
	this->col = col;
	isEmpty = true;
	value = 0;
	r = 205;
	g = 193;
	b = 180;
	updatePosition(row, col);
	justMerged = false;
	valueString = std::to_string(value);
	color3 = { 0,0,0 };
	Valuetext = NULL;
}
Box::Box() {
	isEmpty = true;
	value = 0;
	r = 205;
	g = 193;
	b = 180;
	justMerged = false;
	valueString = std::to_string(value);
	color3 = { 0,0,0 };
	Valuetext = NULL;
}

bool Box::empty() {
	return isEmpty;
}

void Box::render() {
	SDL_SetRenderDrawColor(Board::renderer, r, g, b, 255);
	SDL_RenderFillRect(Board::renderer, &boxRect);
	if (value) {
		SDL_RenderCopy(Board::renderer, Valuetext, NULL, &textRect);
	}


}

void Box::renderBlank() {
	SDL_SetRenderDrawColor(Board::renderer, 205, 193, 180,255);
	SDL_RenderFillRect(Board::renderer, &boxRect);
}

void Box::updatePosition(int newRow, int newCol) {
	row = newRow;
	col = newCol;
	boxRect.w = boxWidth;
	boxRect.h = boxHeight;
	boxRect.x = Board::boardLeftOffset + (boxLeftOffset * (col + 1) + boxWidth * col);
	boxRect.y = Board::boardTopOffset + (boxTopOffset * (row + 1) + boxHeight * row);

	textRect.w = .8 * boxWidth;
	textRect.h = .8 * boxHeight;
	textRect.x = boxRect.x + .1 * boxWidth;
	textRect.y = boxRect.y + .1 * boxWidth;


}

void Box::emptyBox() {
	isEmpty = true;
	r = 205;
	g = 193;
	b = 180;
	updateValue(0);
	valueString = std::to_string(value);
	

}
#include <iostream>
void Box::updateValue(int newVal) {
	if (newVal != 0) {
		isEmpty = false;
	}
	else {
		isEmpty = true;
	}
	value = newVal;
	valueString = std::to_string(value);
	fixColor();
	const char* c = valueString.c_str();
	text = TTF_RenderText_Solid(Board::font, c, color3);
	if (Valuetext) {
	
		SDL_DestroyTexture(Valuetext);
		
	}

	textRect.w = text->w;
	textRect.h = text->h;
	if (textRect.w > boxWidth * .8) {
		float scalar = (.8 * boxWidth) / textRect.w;
		textRect.w *= scalar;
		textRect.h *= scalar;
	}
	if (textRect.h > boxHeight * .8) {
		float scalar = (.8 * boxHeight) / textRect.h;
		textRect.w *= scalar;
		textRect.h *= scalar;

	}
	
	textRect.x = boxRect.x + (std::abs(boxRect.w - textRect.w)) / 2;
	textRect.y = boxRect.y + (std::abs(boxRect.h - textRect.h)) / 2;
	

	Valuetext = SDL_CreateTextureFromSurface(Board::renderer, text);
	SDL_FreeSurface(text);
}


SDL_Texture* Box::getText() {
	const char* c = valueString.c_str();
	SDL_Surface* text = TTF_RenderText_Solid(Board::font, c, color3);
	SDL_Texture* Valuetext = SDL_CreateTextureFromSurface(Board::renderer, text);
	SDL_FreeSurface(text);
	return Valuetext;

}
void Box::fixColor() {
	switch (value) {
	case 0:
		r = 205;
		g = 193;
		b = 180;
		break;
	case 2:
		r = 238;
		g = 228;
		b = 218;
		break;
	case 4:
		
		r = 237;
		g = 224;
		b = 200;
		break;
	case 8:
		r = 242;
		g = 177;
		b = 121;
		break;
	case 16:
	
		r = 245;
		g = 149;
		b = 99;
		break;
	case 32:
		r = 246;
		g = 124;
		b = 95;
		break;
	case 64:
		r = 246;
		g = 94;
		b = 59;
		break;
	case 128:
	
		r = 237;
		g = 207;
		b = 114;
		break;
	case 256:
		r = 132;
		g = 218;
		b = 172;
		break;
	case 512:
		r = 84;
		g = 222;
		b = 232;
		break;
	case 1024:
		r = 107;
		g = 145;
		b = 215;
		break;
	case 2048:
		r = 136;
		g = 127;
		b = 238;
		break;
	case 4096:
		r = 183;
		g = 115; 
		b = 236;
		break;
	case 8192:
		r = 236;
		g = 79;
		b = 81;
		break;
	case 16384:
		r = 97;
		g = 225;
		b = 148;
		break;
	case 32768:
		r = 134;
		g = 238;
		b = 238;
		break;
	case 65536:
		r = 34;
		g = 76;
		b = 150;
		break;
	default:
		r = 255;
		g = 255;
		b = 255;
		break;
	}
}

Box::~Box() {
	if (Valuetext) {
		SDL_DestroyTexture(Valuetext);
		
	}
}