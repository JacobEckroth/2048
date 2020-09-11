#include "Button.h"
#include "Board.h"
#include "SDL_image.h"
#include "SDL.h"
#include <iostream>
#include <string>
Button::Button() {

}
Button::~Button() {
	SDL_DestroyTexture(texture);
}
void Button::init(std::string name, int xPos, int yPos, int width, int height) {
	srcRect.w = srcRect.h = 100;
	srcRect.x = srcRect.y = 0;
	destRect.w = width;
	destRect.h = height;
	destRect.x = xPos;
	destRect.y = yPos;
	fileName = name;
	const char* c = fileName.c_str();
	
	SDL_Surface* tempSurface = IMG_Load(c);
	if (!tempSurface) {
		std::cerr << "error loading image file" << std::endl;
	}
	std::cout << IMG_GetError();
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Board::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	texture = tex;
}


void Button::render() {
	
	SDL_SetRenderDrawColor(Board::renderer, 255, 255, 255, 255);
	SDL_RenderCopy(Board::renderer, texture, &srcRect, &destRect);
}

bool Button::clickInButton(int x, int y) {
	if (x >= destRect.x && x <= destRect.x + destRect.w) {
		if (y >= destRect.y && y <= destRect.y + destRect.h) {
			return true;
		}
	}
	return false;

}
	