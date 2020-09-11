#pragma once
#undef main
#include "SDL.h"
#include "Box.h"
#include "SDL_ttf.h"
#include "MoveBox.h"
#include "Button.h"
#include <vector>
#include "GrowBox.h"
class Board
{


public:
	Board();
	~Board();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void update();
	void handleEvents();
	void clean();
	void render();
	void renderBoxes(int,int);
	bool running();

	void addABox();
	void handleKeyDown(SDL_KeyboardEvent& key);
	void handleKeyUp(SDL_KeyboardEvent& key);

	void endOfTurn();
	void movePiecesRight();
	void movePiecesLeft();
	void movePiecesDown();
	void getPreviousHighScore();
	void movePiecesUp();
	void resetMerges();
	void checkMergeRight(int, int);
	void checkMergeLeft(int, int);
	void checkMergeUp(int, int);
	void checkMergeDown(int, int);
	void newGame();
	void renderBlankBoard();
	void animateBoxes();
	void clearMoveBoxes();

	void testIfClickInButton();
	void resetBoard();

	void showScore();
	
	void updateHighScore();
	void showHighScore();
	//void animateBox(SDL_Rect, SDL_Rect,int,int);
	void animateNewBox(int, int);
	void addBoxes();
	void addRow();
	void addCol();
	void removeRow();
	void removeCol();
	void toggleBotMode();
	void createButtons(int, int);
	void renderButtons();
	void animateNewBoxes();




	static SDL_Renderer* renderer;
	static float boardWidth;
	static float boardHeight;
	static float boardTopOffset;
	static float boardLeftOffset;
	static int boardX;
	static int boardY;
	static int boxRows;
	static int boxCols;
	static int newBoxRows;
	static int newBoxCols;
	static TTF_Font* font;

private:
	std::vector<GrowBox> growBoxes;
	MoveBox** moveBoxes;
	bool botMode;
	int score;
	bool isRunning;
	SDL_Window* window;
	Box** boxes;
	int keyPressed;
	int highScore;
	float scoreMaxHeight;
	float scoreMaxWidth;
	
	SDL_Rect highScoreRect;
	SDL_Rect scoreRect;
	
	SDL_Rect boardBackground;

	Button addRowButton;
	Button addColButton;
	Button removeRowButton;
	Button removeColButton;

};

