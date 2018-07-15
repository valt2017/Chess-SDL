#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <map>

// Ported to SDL from https://www.youtube.com/user/FamTrinli chess tutorial in SFML

//start board
const int BoardSize = 8;
int board[BoardSize][BoardSize] =
{ -1,-2,-3,-4,-5,-3,-2,-1,
-6,-6,-6,-6,-6,-6,-6,-6,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
6, 6, 6, 6, 6, 6, 6, 6,
1, 2, 3, 4, 5, 3, 2, 1 };

//figures coordinates
const int BoardOffset = 2;
std::map<int, SDL_Rect> Figures;
const int FigureSize = 56;


int main(int argc, char ** argv) {
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	std::map<const char, SDL_Texture *> Textures;
	std::map<const char, std::string> Bitmaps;
	const int posX = 100, posY = 100;
	const int sizeX = 454, sizeY = 454;
	// Error checks
	std::cout << "SDL init start" << std::endl;
	
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("SDL & Stockfish", posX, posY, sizeX, sizeY, 0);
	if (win == NULL) {
		std::cout << "SDL_CreateWindow error"<<std::endl;
	}
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::cout << "SDL_CreateRenderer error" << std::endl;
	}
	// Load bitmaps
	Bitmaps[0] = "img/board0.bmp";
	Bitmaps[1] = "img/figures.bmp";
	// Create textures from bitmaps
	for (auto bitmap : Bitmaps) {
		SDL_Surface * bitmapSurface = SDL_LoadBMP(bitmap.second.c_str());
		//transparent color -> red
		bitmapSurface->format->Amask = 0xFF000000;
		bitmapSurface->format->Ashift = 24;
		SDL_SetColorKey(bitmapSurface, SDL_TRUE, SDL_MapRGB(bitmapSurface->format, 255, 0, 0));
		// create texture with modified color (transparent)
		Textures[bitmap.first] = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
		if (Textures[bitmap.first] == NULL) {
			std::cout << bitmap.first <<" SDL_CreateTextureFromSurface error" << std::endl;
		}
		SDL_FreeSurface(bitmapSurface);
	}
	// Coordinates for figures from bitmap
	for (int i = 1; i <= 6; i++) {
		Figures[-i] = SDL_Rect{ (i - 1)*FigureSize, 0, FigureSize, FigureSize};
		Figures[i] = SDL_Rect{ (i - 1)*FigureSize, FigureSize, FigureSize, FigureSize};
	}
	while (1) {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
			/* Look for a keypress */
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_LEFT:
					break;
				case SDLK_RIGHT:
					break;
				case SDLK_UP:
					break;
				case SDLK_DOWN:
					break;
				case SDLK_RETURN:
					break;
				case SDLK_SPACE:
					break;
				default:
					std::cout << "Key not supported" << std::endl;
					break;
				}
			}
		}
		SDL_RenderClear(renderer);
		// board texture
		auto texture = Textures.find(0);
		if (texture != Textures.end()) {
			SDL_Rect DestR;
			DestR.x = 0;
			DestR.y = 0;
			DestR.h = sizeY;
			DestR.w = sizeX;
			SDL_RenderCopy(renderer, texture->second, NULL, &DestR);
		}
		// figures texture
		texture = Textures.find(1);
		if (texture != Textures.end()) {
			for (int i = 0; i < BoardSize; i++)
				for (int j = 0; j < BoardSize;j++) {
					auto it = Figures.find(board[j][i]);
					if (it != Figures.end()){
						SDL_Rect DestR;
						DestR.x = BoardOffset + i*FigureSize;
						DestR.y = BoardOffset + j*FigureSize;
						DestR.h = FigureSize;
						DestR.w = FigureSize;
						SDL_RenderCopy(renderer, texture->second, &it->second, &DestR);
					}
				}

		}
		SDL_RenderPresent(renderer);
	}
	for (auto texture : Textures) {
		SDL_DestroyTexture(texture.second);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}