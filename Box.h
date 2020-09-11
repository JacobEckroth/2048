#pragma once
#include "SDL.h"
#include <string>
#include <string.h>
class Box
{
public:
	Box(int, int);
	Box();
	~Box();
	void render();
	static float boxWidth;
	static float boxHeight;
	static float boxTopOffset;
	static float boxLeftOffset;
	void updatePosition(int, int);
	void emptyBox();
	bool empty();
	void updateValue(int);
	void fixColor();
	int getValue() {
		return value;
	}
	int getR() {
		return r;
	}
	int getG() {
		return g;
	}
	int getB() {
		return b;
	}
	bool wasJustMerged() {
		return justMerged;
	}
	void updateMerged(bool newMerged) {
		justMerged = newMerged;
	}
	SDL_Rect getRect() {
		return boxRect;
	}
	void renderBlank(); 

	SDL_Texture* getText();
	SDL_Rect getTextRect() {
		return textRect;
	}

private:
	int row;
	int col;
	bool isEmpty;
	int value;
	std::string valueString;
	int r;
	int g;
	int b;
	SDL_Rect boxRect;
	bool justMerged;
	SDL_Surface* text;
	SDL_Color color3;
	SDL_Texture* Valuetext;
	SDL_Rect textRect;
};

