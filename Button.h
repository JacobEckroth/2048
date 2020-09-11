#pragma once
#include "SDL.h"

#include <string>
class Button
{

public:
	Button();
	~Button();
	void init(std::string,int xPos, int yPos, int width, int height);
	void render();
	bool clickInButton(int x, int y);
private:
	SDL_Rect srcRect;
	SDL_Rect destRect;
	std::string fileName;
	SDL_Texture* texture;


};

