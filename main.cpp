#include "Board.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <ctime>
#include "wtypes.h"
#include <iostream>
int Board::boxRows = 4;
int Board::boxCols = 4;
int Board::newBoxRows = 4;
int Board::newBoxCols = 4;
void calculateWindowSizes(int&, int&);
int main(int argc, char** argv) {
	srand(time(NULL));
	Board* board;
	

	
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	
	Uint32 frameStart;
	int frameTime;
	int windowWidth = 800;
	int windowHeight = 800;
	
	calculateWindowSizes(windowWidth, windowHeight);
	board = new Board();
	board->init("2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, false);
	while (board->running()) {
		frameStart = SDL_GetTicks();
		board->handleEvents();
		board->update();
		board->render();
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}

	}
	board->clean();
		
	delete(board);
		
	_CrtDumpMemoryLeaks();
	return 0;
}


void calculateWindowSizes(int& windowWidth, int& windowHeight) {
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	int vertical = desktop.bottom;

	vertical = int(vertical * .9);
	
	windowWidth = vertical;
	windowHeight = vertical;


}