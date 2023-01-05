#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
SDL_Window* window = NULL;

SDL_Surface** surface = new SDL_Surface * [12];

SDL_Surface* windowSurface = NULL;

SDL_Event e;

class gamewindow {
public:
	static void Init() {
		window = SDL_CreateWindow("2048game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 390, 390, SDL_WINDOW_SHOWN);
		windowSurface = SDL_GetWindowSurface(window);
	}

	static void loadMedia() {
		surface[1] = loadImage("image/2.png");
		surface[2] = loadImage("image/4.png");
		surface[3] = loadImage("image/8.png");
		surface[4] = loadImage("image/16.png");
		surface[5] = loadImage("image/32.png");
		surface[6] = loadImage("image/64.png");
		surface[7] = loadImage("image/128.png");
		surface[8] = loadImage("image/256.png");
		surface[9] = loadImage("image/512.png");
		surface[10] = loadImage("image/1024.png");
		surface[11] = loadImage("image/2048.png");
	}

	static void close() {
		for (int i = 1; i < 12; i++) {
			SDL_FreeSurface(surface[i]);
			surface[i] = NULL;
		}
		//for (int i = 0; i < 4; i++) {
			//for (int j = 0; j < 4; j++) {
				//SDL_FreeSurface(a[i][j].s);
				//a[i][j].s = NULL;
			//}
		//}
		SDL_FreeSurface(windowSurface);
		windowSurface = NULL;
		SDL_DestroyWindow(window);
		SDL_Quit();
		IMG_Quit();

	}

	static SDL_Surface* loadImage(std::string path) {
		SDL_Surface* loadedImage = IMG_Load(path.c_str());
		if (loadedImage == NULL) std::cout << "Unable to load image" << std::endl;
		SDL_Surface* optimizedImage = SDL_ConvertSurface(loadedImage, windowSurface->format, 0);
		return optimizedImage;
	}
};	