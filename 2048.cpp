#include "Gotoxy.h"
#include "SDLsystem.h"
#include "Output.h"
#include <string>
#include <fstream>
#include <conio.h>
#include <vector>

struct block{
	SDL_Surface* s;
	int value;
	bool New;
	block() {
		s = NULL;
		value = 0;
		New = false;
	}
	block(int v, bool n) {
		s = NULL;
		value = v;
		New = n;
	}
};

block a[4][4];
int blankSpace = 16;
bool over = false;
bool change = false;
block defaultBlock = block(0, false);
bool quit = false;
int level = 1;
bool win = false;
int point = 0;
bool autoSave = true;
bool haveSave = false;
std::vector<int> newBlock = {}; // {row, col, val, check}
std::ifstream input("save.txt");
std::ifstream config("config.txt");
std::ifstream stats("stats.txt");
const std::vector<int> pointGain = {4,8,16,32,64,128,256,512,1024,2048};

void viewBoard();

void setting();

void saveGame() {
	std::ofstream output("save.txt");
	output << haveSave;
	output << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			output << a[i][j].value << ' ';
		}
		output << std::endl;
	}
	output << win << std::endl << point;
}

void loadSave() {
	input.clear();
	input.seekg(0);
	bool k;
	input >> k;
	blankSpace = 0;
	int max = 1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			input >> a[i][j].value;
			if (a[i][j].value == 0) blankSpace++;
			else if (a[i][j].value > max) max = a[i][j].value;
		}
	}
	level = max;
	input >> win >> point;
}

void updateBoardUp() {
	bool c[4][4];
	bool check = true;
	while (check) {
		check = false;
		for (int col = 0; col < 4; col++) {
			for (int row = 1; row < 4; row++) {
				if (a[row - 1][col].value == 0) {
					if (a[row][col].value != 0) {
						change = true;
						c[row][col] = true;
						check = true;
					}
					else c[row][col] = false;
				}
				else if (a[row][col].value == a[row - 1][col].value && !(a[row][col].New || a[row - 1][col].New)) {
					change = true;
					c[row][col] = true;
					check = true;
				}
				else c[row][col] = false;
			}
		}
		if (check) {
			int movedPixel = 0;
			while (movedPixel <= 100) {
				SDL_FillRect(windowSurface, 0, 0x00);
				for (int col = 0; col < 4; col++) {
					for (int row = 0; row < 4; row++) {
						if (row == 0) c[row][col] = false;
						if (a[row][col].value != 0) {
							a[row][col].s = surface[a[row][col].value];
							int k = (c[row][col] ? movedPixel : 0);
							SDL_Rect r = { 100 * col,100 * row - k,90,90 };
							SDL_BlitScaled(a[row][col].s, NULL, windowSurface, &r);
						}
					}
				}
				SDL_Delay(1);
				movedPixel += 5;
				SDL_UpdateWindowSurface(window);
			}
			for (int col = 0; col < 4; col++) {
				for (int row = 1; row < 4; row++) {
					if (c[row][col]) {
						if (a[row - 1][col].value == 0) {
							a[row - 1][col] = a[row][col];
							a[row][col] = defaultBlock;
						}
						else {
							a[row - 1][col] = block(a[row - 1][col].value + 1, true);
							if (a[row - 1][col].value > level) level++;
							point += pointGain[a[row][col].value];
							a[row][col] = defaultBlock;
							blankSpace++;
						}
					}
				}
			}
		}
	}
}

void updateBoardLeft() {
	bool c[4][4];
	bool check = true;
	while (check) {
		check = false;
		for (int row = 0; row < 4; row++) {
			for (int col = 1; col < 4; col++) {
				if (a[row][col - 1].value == 0) {
					if (a[row][col].value != 0) {
						change = true;
						c[row][col] = true;
						check = true;
					}
					else c[row][col] = false;
				}
				else if (a[row][col].value == a[row][col - 1].value && !(a[row][col].New || a[row][col - 1].New)) {
					change = true;
					c[row][col] = true;
					check = true;
				}
				else c[row][col] = false;
			}
		}
		if (check) {
			int movedPixel = 0;
			while (movedPixel <= 100) {
				SDL_FillRect(windowSurface, 0, 0x00);
				for (int row = 0; row < 4; row++) {
					for (int col = 0; col < 4; col++) {
						if (col == 0) c[row][col] = false;
						if (a[row][col].value != 0) {
							a[row][col].s = surface[a[row][col].value];
							int k = (c[row][col] ? movedPixel : 0);
							SDL_Rect r = { 100 * col - k,100 * row,90,90 };
							SDL_BlitScaled(a[row][col].s, NULL, windowSurface, &r);
						}
					}
				}
				SDL_Delay(1);
				movedPixel += 5;
				SDL_UpdateWindowSurface(window);
			}
			for (int row = 0; row < 4; row++) {
				for (int col = 1; col < 4; col++) {
					if (c[row][col]) {
						if (a[row][col - 1].value == 0) {
							a[row][col - 1] = a[row][col];
							a[row][col] = defaultBlock;
						}
						else {
							a[row][col - 1] = block(a[row][col - 1].value + 1, true);
							if (a[row][col - 1].value > level) level++;
							point += pointGain[a[row][col].value];
							a[row][col] = defaultBlock;
							blankSpace++;
						}
					}
				}
			}
		}
	}
}

void updateBoardDown() {
	bool c[4][4];
	bool check = true;
	while (check) {
		check = false;
		for (int col = 0; col < 4; col++) {
			for (int row = 2; row >= 0; row--) {
				if (a[row + 1][col].value == 0) {
					if (a[row][col].value != 0) {
						change = true;
						c[row][col] = true;
						check = true;
					}
					else c[row][col] = false;
				}
				else if (a[row][col].value == a[row + 1][col].value && !(a[row][col].New || a[row + 1][col].New)) {
					change = true;
					c[row][col] = true;
					check = true;
				}
				else c[row][col] = false;
			}
		}
		if (check) {
			int movedPixel = 0;
			while (movedPixel <= 100) {
				SDL_FillRect(windowSurface, 0, 0x00);
				for (int col = 0; col < 4; col++) {
					for (int row = 3; row >= 0; row--) {
						if (row == 3) c[row][col] = false;
						if (a[row][col].value != 0) {
							a[row][col].s = surface[a[row][col].value];
							int k = (c[row][col] ? movedPixel : 0);
							SDL_Rect r = { 100 * col,100 * row + k,90,90 };
							SDL_BlitScaled(a[row][col].s, NULL, windowSurface, &r);
						}
					}
				}
				SDL_Delay(1);
				movedPixel += 5;
				SDL_UpdateWindowSurface(window);
			}
			for (int col = 0; col < 4; col++) {
				for (int row = 2; row >= 0; row--) {
					if (c[row][col]) {
						if (a[row + 1][col].value == 0) {
							a[row + 1][col] = a[row][col];
							a[row][col] = defaultBlock;
						}
						else {
							a[row + 1][col] = block(a[row + 1][col].value + 1, true);
							if (a[row + 1][col].value > level) level++;
							point += pointGain[a[row][col].value];
							a[row][col] = defaultBlock;
							blankSpace++;
						}
					}
				}
			}
		}
	}
}

void updateBoardRight() {
	bool c[4][4];
	bool check = true;
	while (check) {
		check = false;
		for (int row = 0; row < 4; row++) {
			for (int col = 2; col >= 0; col--) {
				if (a[row][col + 1].value == 0) {
					if (a[row][col].value != 0) {
						change = true;
						c[row][col] = true;
						check = true;
					}
					else c[row][col] = false;
				}
				else if (a[row][col].value == a[row][col + 1].value && !(a[row][col].New || a[row][col + 1].New)) {
					change = true;
					c[row][col] = true;
					check = true;
				}
				else c[row][col] = false;
			}
		}
		if (check) {
			int movedPixel = 0;
			while (movedPixel <= 100) {
				SDL_FillRect(windowSurface, 0, 0x00);
				for (int row = 0; row < 4; row++) {
					for (int col = 3; col >= 0; col--) {
						if (col == 3) c[row][col] = false;
						if (a[row][col].value != 0) {
							a[row][col].s = surface[a[row][col].value];
							int k = (c[row][col] ? movedPixel : 0);
							SDL_Rect r = { 100 * col + k,100 * row,90,90 };
							SDL_BlitScaled(a[row][col].s, NULL, windowSurface, &r);
						}
					}
				}
				SDL_Delay(1);
				movedPixel += 5;
				SDL_UpdateWindowSurface(window);
			}
			for (int row = 0; row < 4; row++) {
				for (int col = 2; col >= 0; col--) {
					if (c[row][col]) {
						if (a[row][col + 1].value == 0) {
							a[row][col + 1] = a[row][col];
							a[row][col] = defaultBlock;
						}
						else {
							a[row][col + 1] = block(a[row][col + 1].value + 1, true);
							if (a[row][col + 1].value > level) level++;
							point += pointGain[a[row][col].value];
							a[row][col] = defaultBlock;
							blankSpace++;
						}
					}
				}
			}
		}
	}
}

int randomGenerate() {
	int r = rand() % 100;
	return r > 90 ? 2 : 1;
}

void blockGenerateAnimation(int row, int col,int val) {
	SDL_Rect block{ 100 * col,100 * row,90,90 };
	for (int size = 10; size <= 90; size += 10) {
		SDL_FillRect(windowSurface, &block, SDL_MapRGB(windowSurface->format,0, 0, 0));
		SDL_Rect rect{ 100 * col + (90 - size) / 2,100 * row + (90 - size) / 2 ,size,size };
		SDL_BlitScaled(surface[val], NULL, windowSurface, &rect);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(10);
	}
}

std::vector<int> addRandomGeneratedBlock(int& blank) {
	int r = (rand() % blank) + 1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (a[i][j].value == 0) {
				if (--r == 0) {
					int val = randomGenerate();
					a[i][j] = block(val, false);
					blankSpace--;
					return { i,j,val,1 };
				}
			}
		}
	}
}

bool gameOver() {
	if (blankSpace != 0) return false;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (a[i][j].value == a[i][j + 1].value) return false;
			if (a[j][i].value == a[j + 1][i].value) return false;
		}
	}
	return true;
}

void clearNew() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			a[i][j].New = false;
		}
	}
}

void checking() {
	if (level == 11 && !win) {
		viewBoard();
		//gotoxy(0, 16);
		stringOutput((std::string)"YOU WIN!!");
		stringOutput((std::string)"DO YOU WANT TO CONTINUE? [y/n]");
		bool flag = false;
		while (!flag) {
			char c = _getch();
			switch (c) {
			case 'y':
				win = true;
				flag = true;
				break;
			case 'n':
				win = true;
				over = true;
				flag = true;
				break;
			default:
				break;
			}
		}
		stringOutput((std::string)"Request accepted!");
	}
	if (blankSpace == 0) {
		if (gameOver()) {
			//gotoxy(0, 16);
			stringOutput((std::string)"GAME OVER");
			std::string s = "Score: " + std::to_string(point);
			stringOutput(s);
			over = true;
		}
	}
	else {
		if (change) {
			newBlock = addRandomGeneratedBlock(blankSpace);
		}
	}
	clearNew();
	if (autoSave) saveGame();
}

void updateBoard() {
	change = false;
	if (blankSpace == 0) {
		if (gameOver()) {
			//gotoxy(0, 16);
			stringOutput((std::string)"GAME OVER");
			std::string s = "Score: " + std::to_string(point);
			stringOutput(s);
			over = true;
			return;
		}
	}

	while (SDL_PollEvent(&e) != 0 && !over) {
		
		if (e.type == SDL_QUIT) {
			quit = true;
			if (autoSave) saveGame();
			gamewindow::close();
			break;
		}
		else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {

			case SDLK_UP:
			case SDLK_w:
				updateBoardUp();
				checking();
				break;

			case SDLK_LEFT:
			case SDLK_a:
				updateBoardLeft();
				checking();
				break;
			case SDLK_DOWN:
			case SDLK_s:
				updateBoardDown();
				checking();
				break;

			case SDLK_RIGHT:
			case SDLK_d:
				updateBoardRight();
				checking();
				break;
			case SDLK_p:
				saveGame();
				break;
			default:
				break;

			}
			if (over) gamewindow::close();
		}
	}
	
}

void viewBlock(int row, int col) {
	/*gotoxy(6 * j + 2, 3 * i + 1);
	for (int k = 0; k < 5; k++) std::cout << '_';

	gotoxy(6 * j + 2, 3 * i + 4);
	for (int k = 0; k < 5; k++) std::cout << '_';

	for (int k = 1; k <= 3; k++) {
		gotoxy(6 * j + 1, 3 * i + k + 1);
		std::cout << '|';
		gotoxy(6 * j + 7, 3 * i + k + 1);
		std::cout << '|';
	}
	gotoxy(6 * j + 4, 3 * i + 3);
	std::cout << a[i][j].value; */
	

	a[row][col].s = surface[a[row][col].value];
	SDL_Rect rect;
	rect.x = 100 * col;
	rect.y = 100 * row;
	rect.w = 90;
	rect.h = 90;

	SDL_BlitScaled(a[row][col].s, NULL, windowSurface, &rect);

}

void viewBoard() {
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = 640;
	r.h = 480;

	SDL_FillRect(windowSurface, 0, 0x00);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (a[i][j].value != 0) 
				viewBlock(i, j);
		}
	}
	if (newBlock.size() > 0 && newBlock[3]) {
		blockGenerateAnimation(newBlock[0], newBlock[1], newBlock[2]);
		newBlock[3] = 0;
	}
	SDL_UpdateWindowSurface(window);

	
}

void clearBoard() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			a[i][j] = defaultBlock;
		}
	}
	blankSpace = 16;
	over = false;
	win = false;
	level = 1;
	haveSave = false;
}

int mainMenu() {
	system("cls");
	stringOutput((std::string)"Welcome to 2048 game");
	if (haveSave) {
		stringOutput((std::string)"[1] Continue last game");
		stringOutput((std::string)"[2] New game");
		stringOutput((std::string)"[3] Setting");
		stringOutput((std::string)"[Esc] Quit");
		bool k = false;
		while (!k) {
			char c = _getch();
			switch (c) {
			case '1':
				return 1;
				break;
			case '2':
				return 2;
				break;
			case '3':
				setting();
				return mainMenu();
				break;
			case 27:
				return 0;
				break;
			default:
				break;
			}
		}
	}
	else {
		stringOutput((std::string)"[1] New game");
		stringOutput((std::string)"[2] Setting");
		stringOutput((std::string)"[Esc] Quit");
		bool k = false;
		while (!k) {
			char c = _getch();
			switch (c) {
			case '1':
				return 2;
				break;
			case '2':
				setting();
				return mainMenu();
				break;
			case 27:
				return 0;
				break;
			default:
				break;
			}
		}
	}
}

void setting() {
	system("cls");
	stringOutput((std::string)"SETTING");
	std::string s = "[1] Auto Save: ";
	s += (autoSave ? 'V' : 'X');
	stringOutput(s);
	stringOutput((std::string)"[Esc] Back to main menu");
	bool flag = false;
	while (!flag) {
		char c = _getch();
		switch (c) {
		case '1':
			if (autoSave) {
				stringOutput(std::string("Do you want to turn off auto save? [y/n]"));
				bool flag2 = false;
				while (!flag2) {
					char c2 = _getch();
					switch (c2) {
					case 'y':
						stringOutput(std::string("Auto save turned off! Press P in game for manual saving!"));
						autoSave = false;
						Sleep(1000);
						flag2 = true;
						break;
					case 'n':
						Sleep(1000);
						flag2 = true;
						break;
					default: 
						break;
					}
				}
			}
			else {
				stringOutput(std::string("Do you want to turn on auto save? [y/n]"));
				bool flag2 = false;
				while (!flag2) {
					char c2 = _getch();
					switch (c2) {
					case 'y':
						stringOutput(std::string("Auto save turned on!"));
						autoSave = true;
						Sleep(1000);
						flag2 = true;
						break;
					case 'n':
						Sleep(1000);
						flag2 = true;
						break;
					default:
						break;
					}
				}
			}
			flag = true;
			setting();
			break;
		case 27:
			flag = true;
			break;
		default:
			break;
		}
	}
	std::ofstream config("config.txt");
	config << autoSave;
}

void statistics() {
	system("cls");
	stringOutput((std::string)"STATISTICS");
	
}

int main(int agrc, char* agrs[]) {
	srand(time(NULL));
	config.clear();
	config.seekg(0);
	config >> autoSave;
	input.clear();
	input.seekg(0);
	input >> haveSave;

	int option = mainMenu();
	
	bool notagain = false;
	while (!notagain) {
		//system("cls");
		switch (option) {
		case 1:
			stringOutput((std::string)"Loading...");
			Sleep(1000);
			gamewindow::Init();
			gamewindow::loadMedia();
			loadSave();
			break;
		case 2:
			stringOutput((std::string)"Loading...");
			Sleep(1000);
			gamewindow::Init();
			gamewindow::loadMedia();
			haveSave = true;
			newBlock = addRandomGeneratedBlock(blankSpace);
			break;
		case 0:
			stringOutput((std::string)"See you next time...");
			return 0;
			break;
		}

		while ((!over) && (!quit)) {
			viewBoard();
			updateBoard();
		}

		if (quit) {
			stringOutput((std::string)"Game quitted. See you again...");
			Sleep(1000);
			notagain = true;
		}
		else {
			gamewindow::close();
			clearBoard();
			saveGame();
			stringOutput((std::string)"ONE MORE TIME? [y/n]");
			bool flag = false;
			while (!flag) {
				char c = _getch();
				switch (c) {
				case 'y':
					option = 2;
					notagain = false;
					flag = true;
					break;
				case 'n':
					stringOutput((std::string)"See you again...");
					Sleep(2000);
					notagain = true;
					flag = true;
					break;
				default:
					break;
				}
			}
		}
	}
	delete[] surface;
	return 0;
}