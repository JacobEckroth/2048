#include "Board.h"
#include "Box.h"
#include "SDL.h"
#include <iostream>
#include <cstring>
#include <string>
#include "SDL_ttf.h"

#include <ctype.h>
#include <fstream>
SDL_Renderer* Board::renderer = nullptr;
float Board::boardWidth = 0;
float Board::boardHeight = 0;
int Board::boardX = 0;
int Board::boardY = 0;
float Board::boardTopOffset = 0;
float Board::boardLeftOffset = 0;



float Box::boxWidth = 0;
float Box::boxHeight = 0;
float Box::boxTopOffset = 0;
float Box::boxLeftOffset = 0;
TTF_Font* Board::font = nullptr;


Board::Board() {


}

Board::~Board() {
	for (int i = 0; i < boxRows; i++) {
		delete [] boxes[i];
		delete[] moveBoxes[i];
	}
	delete[] moveBoxes;
	delete[] boxes;
}

void Board::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	highScore = 0;
	botMode = false;
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initialized!\n";
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window Created!\n";
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
			std::cout << "Renderer created!\n";
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		}
		if (TTF_Init() < 0) {
			std::cout << "Text initialized error" << std::endl;
			atexit(TTF_Quit);	//does at the exit of the program

		}
		font = TTF_OpenFont("./resources/fonts/Karla-Bold.ttf", 72);
		isRunning = true;
	}
	
	else {
		isRunning = false;
	}
	scoreMaxHeight = .05 * height;
	boxRows = newBoxRows;
	boxCols = newBoxCols;
	scoreMaxWidth = .35 * width;
	scoreRect.x = width * .1;
	scoreRect.y = height*  .025;
	highScoreRect.x = width * .55;
	highScoreRect.y = height * .025;

	boardWidth = .8 * width;
	boardHeight = .8 * height;
	boardBackground.w = boardWidth;
	boardBackground.h = boardHeight;
	boardBackground.y = boardY = (height - boardHeight) / 2;
	boardBackground.x= boardX = (width - boardWidth) / 2;

	Box::boxHeight = (float(boardHeight) / boxRows) * .8;
	Box::boxWidth = (float(boardWidth) / boxCols) * .8;
	Box::boxTopOffset = (float(boardHeight) / (boxRows + 1)) * .2;
	Box::boxLeftOffset = (float(boardWidth) / (boxCols + 1)) * .2;

	boardTopOffset = boardBackground.y;
	boardLeftOffset = boardBackground.x;
	


	boxes = new Box * [boxRows];
	moveBoxes = new MoveBox * [boxRows];
	for (int i = 0; i < boxRows; i++) {
		boxes[i] = new Box[boxCols];
		moveBoxes[i] = new MoveBox[boxCols];
	}
	for (int i = 0; i < boxRows; i++) {
		for (int j = 0; j < boxCols; j++) {
			boxes[i][j].updatePosition(i, j);
		}
	}

	createButtons(width, height);

	
	keyPressed = false;
	score = 0;
	getPreviousHighScore();
	addBoxes();
	addABox();
	animateNewBoxes();






}

void Board::createButtons(int width, int height) {
	int buttonHeight = height * .1 * .8;
	int topMargin = height * .91;
	int buttonWidth = buttonHeight;
	int sideMargins = (width - buttonWidth * 2) / 3;
	addColButton.init("resources/images/AddButton.png", sideMargins, topMargin, buttonWidth, buttonHeight);
	removeColButton.init("resources/images/removeButton.png", sideMargins * 2 + buttonWidth, topMargin, buttonWidth, buttonHeight);

	sideMargins = width * .1 * .1;
	topMargin = (height - buttonHeight * 2) / 3;
	addRowButton.init("resources/images/addButton.png", sideMargins, topMargin, buttonWidth, buttonHeight);
	removeRowButton.init("resources/images/removeButton.png", sideMargins, topMargin* 2 + buttonHeight, buttonWidth, buttonHeight);
	
}



void Board::renderButtons() {
	if (boxRows + 1 <= 25) {
		addRowButton.render();
	}
	if (boxCols + 1 <= 25) {
		addColButton.render();
	}
	if (boxRows - 1 > 1) {
		removeRowButton.render();
	}
	if (boxCols - 1 > 1) {
		removeColButton.render();
	}
}
void Board::addBoxes() {
	int amountOfBoxesToAdd = 0;
	if (boxRows + boxCols <= 8) {
		amountOfBoxesToAdd = 1;
	}
	else if (boxRows + boxCols > 8 && boxRows + boxCols <= 14) {
		amountOfBoxesToAdd = 2;
	}
	else if (boxRows + boxCols > 14 && boxRows + boxCols <= 18) {
		amountOfBoxesToAdd = 3;
	}
	else if(boxRows + boxCols > 18 && boxRows + boxCols < 22){
		amountOfBoxesToAdd = 4;
	}
	else if(boxRows + boxCols >= 22 && boxRows + boxCols < 28){
		amountOfBoxesToAdd = 5;
	}
	else if (boxRows + boxCols >= 28 && boxRows + boxCols < 36) {
		amountOfBoxesToAdd = 6;
	}
	else if (boxRows + boxCols >= 36 && boxRows + boxCols < 44) {
		amountOfBoxesToAdd = 7;
	}
	else {
		amountOfBoxesToAdd = 8;
	}
	for (int i = 0; i < amountOfBoxesToAdd; i++) {
		addABox();
	}
}

void Board::update() {
	if (botMode) {
		int moveChoice = rand() % 4;
		switch (moveChoice) {
		case 0:
			movePiecesUp();
			break;
		case 1:
			movePiecesDown();
			break;
		case 2:
			movePiecesRight();
			break;
		case 3:
			movePiecesLeft();
			break;
		}
	}
}

void Board::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		updateHighScore();
		isRunning = false;
		
		
		break;
	case SDL_KEYDOWN:
		handleKeyDown(event.key);
		break;
	case SDL_KEYUP:
		handleKeyUp(event.key);
		break;
	case SDL_MOUSEBUTTONDOWN:
		testIfClickInButton();
		break;
	}
}

void Board::testIfClickInButton() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (addRowButton.clickInButton(x,y)) {
		if (boxRows + 1 <= 25) {
			addRow();
		}
	}
	else if (addColButton.clickInButton(x, y)) {
		if (boxCols + 1 <= 25) {
			addCol();
		}
	}
	else if (removeColButton.clickInButton(x, y)) {
		if (boxCols - 1 > 1) {
			removeCol();
		}
	}
	else if (removeRowButton.clickInButton(x, y)) {
		if (boxRows - 1 > 1) {
			removeRow();
		}
	}
}

void Board::handleKeyDown(SDL_KeyboardEvent& key){

	switch (key.keysym.scancode) {
	case 41:		//esc key
		
		updateHighScore();
		isRunning = false;
		
		break;
	case 79:
		
		if (keyPressed == -1) {
			movePiecesRight();
			keyPressed = 79;
		}
		break;
	case 80:
		if (keyPressed == -1) {
			movePiecesLeft();
			keyPressed = 80;
		}
		break;
	case 81:
		if (keyPressed == -1) {
			movePiecesDown();
			keyPressed = 81;
		}
		break;
	case 82:
		if (keyPressed == -1) {
			movePiecesUp();
			keyPressed = 82;
		}
		break;
	case 26:
		if (keyPressed == -1) {
			movePiecesUp();
			keyPressed = 26;
		}
		break;
	case 4:
		if (keyPressed == -1) {
			movePiecesLeft();
			keyPressed = 4;
		}
		break;
	case 22:
		if (keyPressed == -1) {
			movePiecesDown();
			keyPressed = 22;
		}
		break;
	case 7:
		if (keyPressed == -1) {
			movePiecesRight();
			keyPressed = 7;
		}
		break;
	case 53:
		if (keyPressed == -1) {
			toggleBotMode();
			keyPressed = 53;
			break;
		}
	default:
		break;
	}
}
void Board::toggleBotMode() {
	botMode == true ? botMode = false : botMode = true;
}

void Board::handleKeyUp(SDL_KeyboardEvent& key) {

	if (keyPressed = key.keysym.scancode) {
		keyPressed = -1;
	}
}


void Board::addRow() {
	Board::newBoxRows++;

	newGame();
}

void Board::addCol() {
	Board::newBoxCols++;

	newGame();
}

void Board::removeRow() {
	Board::newBoxRows--;

	newGame();
}

void Board::removeCol() {
	Board::newBoxCols--;
	
	newGame();

}
void Board::newGame() {
	
	updateHighScore();
	resetBoard();
}




void Board::clean() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);	//error here

	SDL_Quit();
	std::cout << "Succesfully cleaned!\n";
}

void Board::render() {
	SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 187, 173, 160,255);
	SDL_RenderFillRect(renderer, &boardBackground);
	renderButtons();
	renderBoxes(-1,-1);
	
	showScore();
	
	
	SDL_RenderPresent(Board::renderer);
}

bool Board::running() {
	return isRunning;
}

void Board::renderBoxes(int moveToRow, int moveToCol) {
	int* rows = new int[growBoxes.size()];
	int* cols = new int[growBoxes.size()];
	bool rendered = false;

	if (moveToRow == -10 && moveToCol == -10) {
		for (int i = 0; i < growBoxes.size(); i++) {
			rows[i] = growBoxes[i].getRow();
			cols[i] = growBoxes[i].getCol();
		}
	}
	for (int i = 0; i < boxRows; i++) {
		for (int j = 0; j < boxCols; j++) {
			for (int k = 0; k< growBoxes.size(); k++) {
				rendered = false;
				if (rows[k] == i && cols[k] == j) {
					boxes[i][j].renderBlank();
					rendered = true;
					break;
				}
			}
			if (rendered) {
				continue;
			}else if (moveToRow == -5 && moveToCol == -5) {	//send -5 to make the obard blank
				boxes[i][j].renderBlank();
			}else if (i != moveToRow || j != moveToCol) {
				boxes[i][j].render();
			}
			else {
				boxes[i][j].renderBlank();
			}
		}
	}
	delete[] rows;
	delete[] cols;
}

void Board::resetBoard() {
	for (int i = 0; i < boxRows; i++) {
		delete[] boxes[i];
		delete[] moveBoxes[i];
	}
	delete[] moveBoxes;
	delete[] boxes;
	highScore = 0;

	boxRows = newBoxRows;
	boxCols = newBoxCols;
	Box::boxHeight = (float(boardHeight) / boxRows) * .8;
	Box::boxWidth = (float(boardWidth) / boxCols) * .8;
	Box::boxTopOffset = (float(boardHeight) / (boxRows + 1)) * .2;
	Box::boxLeftOffset = (float(boardWidth) / (boxCols + 1)) * .2;

	boardTopOffset = boardBackground.y;
	boardLeftOffset = boardBackground.x;



	boxes = new Box * [boxRows];
	moveBoxes = new MoveBox * [boxRows];
	for (int i = 0; i < boxRows; i++) {
		boxes[i] = new Box[boxCols];
		moveBoxes[i] = new MoveBox[boxCols];
	}
	for (int i = 0; i < boxRows; i++) {
		for (int j = 0; j < boxCols; j++) {
			boxes[i][j].updatePosition(i, j);
		}
	}
	keyPressed = false;
	score = 0;
	getPreviousHighScore();
	addBoxes();
	addABox();
	animateNewBoxes();
}



void Board::getPreviousHighScore() {
	std::fstream highScoreFile;
	std::string filename = "";
	filename += "resources/scores/";
	filename += std::to_string(boxRows);
	filename += "x";
	filename += std::to_string(boxCols);
	filename += "_score.txt";
	highScoreFile.open(filename, std::ios::in);
	if (!highScoreFile.is_open()) {
		highScore = 0;
	}
	else {
		std::string line;
		getline(highScoreFile, line);
		bool invalid = false;
		for (int i = 0; i < line.length(); i++) {
			if (!std::isdigit(line[i])) {
				highScore = 0;
				invalid = true;
			}
		}
		if (!invalid) {
			highScore = std::stoi(line);
		}
	}
}


void Board::updateHighScore() {
	
	std::fstream highScoreFile;
	std::string filename = "";
	filename += "resources/scores/";
	filename += std::to_string(boxRows);
	filename += "x";
	filename += std::to_string(boxCols);
	filename += "_score.txt";
	highScoreFile.open(filename, std::ios::in);
	std::string line;
	int currentHighScore;
	if (highScoreFile.is_open()) {
		
		getline(highScoreFile, line);
		bool invalid = false;
		for (int i = 0; i < line.length(); i++) {
			if (!std::isdigit(line[i])) {
				currentHighScore = 0;
				invalid = true;
			}
		}
		if (!invalid) {
			currentHighScore = std::stoi(line);
		}
		else {
			highScoreFile.close();
			highScoreFile.open(filename, std::ios::out | std::ios::trunc);
	
			highScoreFile << score;
			highScoreFile.close();
			return;
		}
	
		if (currentHighScore > score) {
			
			highScoreFile.close();
			return;
		}
		else {
			highScoreFile.close();
			highScoreFile.open(filename, std::ios::out | std::ios::trunc);
			
			highScoreFile << score;
			highScoreFile.close();
			return;
		}
	}
	else {	
		highScoreFile.close();

		highScoreFile.open(filename, std::ios::out);
		highScoreFile << score;
		highScoreFile.close();
	}

		
}


void Board::addABox() {	//TODO make the box smart small and grow into the space when you add one.
	bool boardFull = true;
	for (int i = 0; i < boxRows; i++) {
		for (int j = 0; j < boxCols; j++) {
			if (boxes[i][j].empty()) {
				boardFull = false;
			}
		}
	}
	if (boardFull) {
		SDL_Delay(1000);
		newGame();
		
		return;
	}
	bool boxPicked = false;
	int boxRow, boxCol;

	while (!boxPicked) {
	
		boxRow = rand() % boxRows;
		boxCol = rand() % boxCols;
		if (boxes[boxRow][boxCol].empty()) {
			boxPicked = true;
			break;
		}
		
	}
	int randomNumber = (rand() % 2 + 1) * 2;
	boxes[boxRow][boxCol].updateValue(randomNumber);

	animateNewBox(boxRow, boxCol);

}

void Board::animateNewBox(int boxRow, int boxCol) {
	SDL_Rect animateRect;
	SDL_Rect goToRect = boxes[boxRow][boxCol].getRect();
	
	animateRect.x = goToRect.x + Box::boxWidth/ 2;
	animateRect.y = goToRect.y + Box::boxHeight/ 2;
	animateRect.w = 0;
	animateRect.h = 0;
	int r, g, b;
	r = boxes[boxRow][boxCol].getR();
	g = boxes[boxRow][boxCol].getG();
	b = boxes[boxRow][boxCol].getB();
	GrowBox newBox;
	newBox.init(animateRect, goToRect, r, g, b,boxRow,boxCol);
	growBoxes.push_back(newBox);
	/*while (std::abs(animateRect.w - goToRect.w) >= increaseBy || std::abs(animateRect.h - goToRect.h) >= increaseBy) {
		if (animateRect.w < goToRect.w) {
			animateRect.w += increaseBy;
			animateRect.x -= increaseBy / 2;
		}
		if (animateRect.h < goToRect.h) {
			animateRect.h += increaseBy;
			animateRect.y -= increaseBy / 2;
		}
		SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
		SDL_RenderFillRect(renderer, &boardBackground);
		renderBoxes(boxRow, boxCol);
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderFillRect(renderer, &animateRect);
		showScore();
		renderButtons();
		SDL_RenderPresent(Board::renderer);
		if (!botMode) {
			SDL_Delay(64 / (boxRows + boxCols));
		}
	}*/




}


void Board::animateNewBoxes() {
	bool done = false;
	while (!done) {
		done = true;
		SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
		SDL_RenderFillRect(renderer, &boardBackground);
		renderBoxes(-10, -10);

		for (int i = 0; i < growBoxes.size(); i++) {
			if (growBoxes[i].needsToGrow()) {
				done = false;
				growBoxes[i].updateSize();
				
			}
		}
		for (int i = 0; i < growBoxes.size(); i++) {
			growBoxes[i].render();
		}
		renderButtons();
		showScore();
		SDL_RenderPresent(Board::renderer);
		if (!botMode) {
			
			SDL_Delay(1);
		}
	}
	for (int i = 0; i < growBoxes.size(); i++) {
		growBoxes[i].reset();
	}
	
	growBoxes.clear();

}





void Board::resetMerges() {
	for (int i = 0; i < boxRows; i++) {
		for (int j = 0; j < boxCols; j++) {
			boxes[i][j].updateMerged(false);
		}
	}
}

void Board::movePiecesRight() {
	for (int j = boxCols - 1; j >= 0; j--) {
		for (int i = 0; i < boxRows; i++) {
			SDL_Rect prev = boxes[i][j].getRect();
			bool done = false;
			int currentCol = j;
			while (!done) {
				if (boxes[i][j].empty()) {
					done = true;
				}
				else if (currentCol+1 >= boxCols) {
					done = true;
				}
				else {
					if (!(boxes[i][currentCol + 1].empty())) {	//if it's not empty stop here.
						done = true;
					}
					else {
						currentCol++;
					}
				}
				
				
			}
			int tempVal = boxes[i][j].getValue();
			SDL_Rect current = boxes[i][currentCol].getRect();
			if (!(boxes[i][j].empty())) {
				moveBoxes[i][j].init(prev, current, boxes[i][j].getText(), (boxes[i][j].getTextRect()), boxes[i][j].getR(), boxes[i][j].getG(), boxes[i][j].getB());
			}
			boxes[i][j].emptyBox();
			boxes[i][currentCol].updateValue(tempVal);
	
			
		
			checkMergeRight(i, currentCol);


		}
	}
	endOfTurn();
	

}
void Board::endOfTurn() {

	animateBoxes();
	clearMoveBoxes();

	addBoxes();
	animateNewBoxes();
	
	resetMerges();
	
	
}


void Board::clearMoveBoxes() {
	for (int i = 0; i < boxRows; i++) {
		for (int j = 0; j < boxCols; j++) {
			moveBoxes[i][j].reset();
		}
	}
}


void Board::checkMergeRight(int row, int col) {
	if (boxes[row][col].empty()) {
		return;
	}
	else if (col + 1 >= boxCols) {
		return;
	}
	else if (boxes[row][col + 1].getValue() == boxes[row][col].getValue()) {
		if (boxes[row][col + 1].wasJustMerged()) {
			return;
		}
		else {
	
			score += boxes[row][col].getValue() * 2;
			boxes[row][col + 1].updateValue(boxes[row][col].getValue() * 2);
			boxes[row][col + 1].updateMerged(true);
			boxes[row][col].emptyBox();
			
		}
	} 


}



void Board::movePiecesLeft() {
	for (int j = 0; j < boxCols; ++j) {
		for (int i = 0; i < boxRows; i++) {
			SDL_Rect prev = boxes[i][j].getRect();
			bool done = false;
			int currentCol = j;
			while (!done) {
				if (boxes[i][j].empty()) {
					done = true;
				}
				else if (currentCol - 1 < 0) {
					done = true;
				}
				else {
					if (!(boxes[i][currentCol - 1].empty())) {	//if it's not empty stop here.
						done = true;
					}
					else {
						currentCol--;
					}
				}


			}
			int tempVal = boxes[i][j].getValue();
			SDL_Rect current = boxes[i][currentCol].getRect();
			if (!(boxes[i][j].empty())) {
				moveBoxes[i][j].init(prev, current, boxes[i][j].getText(), (boxes[i][j].getTextRect()), boxes[i][j].getR(), boxes[i][j].getG(), boxes[i][j].getB());
			}
			boxes[i][j].emptyBox();
			boxes[i][currentCol].updateValue(tempVal);
			
			//animateBox(current, prev, i, currentCol);
			checkMergeLeft(i, currentCol);


		}
	}
	endOfTurn();
}

void Board::checkMergeLeft(int row, int col) {
	if (boxes[row][col].empty()) {
		return;
	}
	else if (col -1 < 0) {
		return;
	}
	else if (boxes[row][col - 1].getValue() == boxes[row][col].getValue()) {
		if (boxes[row][col - 1].wasJustMerged()) {
			return;
		}
		else {
			score += boxes[row][col].getValue() * 2;
			boxes[row][col - 1].updateValue(boxes[row][col].getValue() * 2);
			boxes[row][col - 1].updateMerged(true);
			boxes[row][col].emptyBox();

		}
	}


}






void Board::movePiecesUp() {
	for (int i = 0; i < boxRows; i++) {
		for (int j = 0; j < boxCols; j++) {
		
			SDL_Rect prev = boxes[i][j].getRect();
			bool done = false;
			int currentRow = i;
			while (!done) {
				if (boxes[i][j].empty()) {
					done = true;
				}
				else if (currentRow - 1 < 0) {
					done = true;
				}
				else {
					if (!(boxes[currentRow-1][j].empty())) {	//if it's not empty stop here.
						done = true;
					}
					else {
						currentRow--;
					}
				}


			}
			int tempVal = boxes[i][j].getValue();
			SDL_Rect current = boxes[currentRow][j].getRect();
			if (!(boxes[i][j].empty())) {
				moveBoxes[i][j].init(prev, current, boxes[i][j].getText(), (boxes[i][j].getTextRect()), boxes[i][j].getR(), boxes[i][j].getG(), boxes[i][j].getB());
			}
			boxes[i][j].emptyBox();
			boxes[currentRow][j].updateValue(tempVal);
			//animateBox(current, prev, currentRow, j);
			checkMergeUp(currentRow,j);


		}
	}
	endOfTurn();
}

void Board::checkMergeUp(int row, int col) {
	if (boxes[row][col].empty()) {
		return;
	}
	else if (row - 1 < 0) {
		return;
	}
	else if (boxes[row -1][col].getValue() == boxes[row][col].getValue()) {
		if (boxes[row -1][col].wasJustMerged()) {
			
			return;
		}
		else {
			score += boxes[row][col].getValue() * 2;
			boxes[row -1][col].updateValue(boxes[row][col].getValue() * 2);
			boxes[row -1][col].updateMerged(true);
			boxes[row][col].emptyBox();

		}
	}


}




void Board::movePiecesDown() {
	for (int i = boxRows - 1; i >= 0; i--) {
		for (int j = 0; j < boxCols; j++) {
		
			SDL_Rect prev = boxes[i][j].getRect();

			bool done = false;
			int currentRow = i;
			while (!done) {
				if (boxes[i][j].empty()) {
					done = true;
				}
				else if (currentRow + 1 >= boxRows) {
					done = true;
				}
				else {
					if (!(boxes[currentRow + 1][j].empty())) {	//if it's not empty stop here.
						done = true;
					}
					else {
						currentRow++;
					}
				}


			}
			int tempVal = boxes[i][j].getValue();
			SDL_Rect current = boxes[currentRow][j].getRect();
			if (!(boxes[i][j].empty())) {
				moveBoxes[i][j].init(prev, current, boxes[i][j].getText(), (boxes[i][j].getTextRect()), boxes[i][j].getR(), boxes[i][j].getG(), boxes[i][j].getB());
			}
			boxes[i][j].emptyBox();
			boxes[currentRow][j].updateValue(tempVal);
		
			checkMergeDown(currentRow,j);

			

		}
	}
	endOfTurn();
}




void Board::checkMergeDown(int row, int col) {
	if (boxes[row][col].empty()) {
		return;
	}
	else if (row + 1 >= boxRows) {
		return;
	}
	else if (boxes[row + 1][col].getValue() == boxes[row][col].getValue()) {
		if (boxes[row + 1][col].wasJustMerged()) {
			return;
		}
		else {
			score += boxes[row][col].getValue() * 2;
			boxes[row + 1][col].updateValue(boxes[row][col].getValue() * 2);
			boxes[row + 1][col].updateMerged(true);
			boxes[row][col].emptyBox();
			

		}
	}


}


/*void Board::animateBox(SDL_Rect moveTo, SDL_Rect moveFrom, int moveToRow, int moveToCol) {
	SDL_Rect moveRect;
	moveRect.x = moveFrom.x;
	moveRect.y = moveFrom.y;
	moveRect.w = moveFrom.w;
	moveRect.h = moveFrom.h;
	int xvel;
	int yvel;
	int r;
	int g;
	int b;

	int moveAmount = 15;
	if (moveRect.x > moveTo.x) {
		xvel = -1;
	}
	else if (moveRect.x < moveTo.x) {
		xvel = 1;
	}
	else {
		xvel = 0; 
	}
	if (moveRect.y > moveTo.y) {
		yvel = -1;
	}
	else if (moveRect.y < moveTo.y) {
		yvel = 1;
	}
	else {
		yvel = 0;
	}
	r = boxes[moveToRow][moveToCol].getR();
	g = boxes[moveToRow][moveToCol].getG();
	b = boxes[moveToRow][moveToCol].getB();
	while (std::abs(moveRect.x - moveTo.x) >= moveAmount || std::abs(moveRect.y - moveTo.y) >= moveAmount) {
		
		moveRect.y += yvel * moveAmount;
		moveRect.x += xvel * moveAmount;

		SDL_SetRenderDrawColor(renderer, 255, 250, 248, 239);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
		SDL_RenderFillRect(renderer, &boardBackground);
		renderBoxes(moveToRow,moveToCol);
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderFillRect(renderer, &moveRect);

		SDL_RenderPresent(Board::renderer);
		SDL_Delay(1);

	}



}*/



void Board::renderBlankBoard() {
	SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
	SDL_RenderFillRect(renderer, &boardBackground);
}

void Board::animateBoxes() {
	bool done = false;
	while (!done) {
		done = true;
		renderBlankBoard();
		renderBoxes(-5, -5);
		for (int i = 0; i < boxRows; i++) {
			for (int j = 0; j < boxCols; j++) {
				if (!(moveBoxes[i][j].empty())) {
					moveBoxes[i][j].updatePosition();
					if ((moveBoxes[i][j].needsToMove())) {
						done = false;
					}
					moveBoxes[i][j].render();
				}
			}
		}
		showScore();
		renderButtons();
		SDL_RenderPresent(Board::renderer);
		if (!botMode) {
			SDL_Delay(1);
		}
	}
	for (int i = 0; i < boxRows; i++) {
		for (int j = 0; j < boxCols; j++) {
			moveBoxes[i][j].reset();
		}
	}
}

void Board::showScore() {
	std::string scoreString = "Score: " + std::to_string(score);
	
	
	const char* c = scoreString.c_str();
	SDL_Color color3 = { 0,0,0 };
	
	SDL_Surface* text = TTF_RenderText_Blended(Board::font, c, color3);
	SDL_Texture* Valuetext = SDL_CreateTextureFromSurface(Board::renderer, text);



	scoreRect.w = text->w;
	scoreRect.h = text->h;
	
	float scalar = scoreMaxHeight / scoreRect.h;
	
	scoreRect.h *= scalar;
	scoreRect.w *= scalar;
	if (scoreRect.w > scoreMaxWidth) {
		float scalar = scoreMaxWidth / scoreRect.w;
		scoreRect.w *= scalar;
		scoreRect.h *= scalar;
	}
	

	SDL_FreeSurface(text);
	SDL_RenderCopy(Board::renderer, Valuetext, NULL, &scoreRect);
	SDL_DestroyTexture(Valuetext);

	showHighScore();


	
}

void Board::showHighScore(){
	if (score > highScore) {
		highScore = score;
	}
	std::string scoreString = "Highscore: " + std::to_string(highScore);


	const char* c = scoreString.c_str();
	SDL_Color color3 = { 0,0,0 };

	SDL_Surface* text = TTF_RenderText_Blended(Board::font, c, color3);
	SDL_Texture* Valuetext = SDL_CreateTextureFromSurface(Board::renderer, text);



	highScoreRect.w = text->w;
	highScoreRect.h = text->h;

	float scalar = scoreMaxHeight / highScoreRect.h;

	highScoreRect.h *= scalar;
	highScoreRect.w *= scalar;
	if (highScoreRect.w > scoreMaxWidth) {
		float scalar = scoreMaxWidth / highScoreRect.w;
		highScoreRect.w *= scalar;
		highScoreRect.h *= scalar;
	}


	SDL_FreeSurface(text);
	SDL_RenderCopy(Board::renderer, Valuetext, NULL, &highScoreRect);
	SDL_DestroyTexture(Valuetext);


}