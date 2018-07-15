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
std::map<const char, SDL_Texture *> Textures;
std::map<const char, std::string> Bitmaps;
const int posX = 100, posY = 100;
const int sizeX = 454, sizeY = 454;
std::string moves = "";

std::string toChessNote(SDL_Point & position)
{ // www.asciitable.com :-)
	std::string s = "";
	s += char(position.x / FigureSize + 'a');
	s += char(7 - position.y / FigureSize + '1');
	return s;
}

SDL_Point toBoard(char a, char b)
{
	SDL_Point position;
	position.x = int(a) - 97;
	position.y = 7 - int(b) + 49;
	return position;
}

void performMove(std::string str)
{
	SDL_Point oldPos = toBoard(str[0], str[1]);
	SDL_Point newPos = toBoard(str[2], str[3]);
	std::cout << "X:" << oldPos.x << "Y:" << oldPos.y << " " << board[oldPos.y][oldPos.x] << std::endl;
	std::cout << "X:" << newPos.x << "Y:" << newPos.y << " " << board[newPos.y][newPos.x] << std::endl;
	if (board[oldPos.y][oldPos.x] != 0 && board[newPos.y][newPos.x] == 0) {
		board[newPos.y][newPos.x] = board[oldPos.y][oldPos.x];
		board[oldPos.y][oldPos.x] = 0;
	}

/*
	//castling       //if the king didn't move
	if (str == "e1g1") if (position.find("e1") == -1) move("h1f1");
	if (str == "e8g8") if (position.find("e8") == -1) move("h8f8");
	if (str == "e1c1") if (position.find("e1") == -1) move("a1d1");
	if (str == "e8c8") if (position.find("e8") == -1) move("a8d8");
	*/
}


int main(int argc, char ** argv) {
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	// Error checks
	std::cout << "SDL_Init\n";
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("SDL & Stockfish", posX, posY, sizeX, sizeY, 0);
	if (win == NULL) {
		std::cout << "SDL_CreateWindow error\n";
	}
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::cout << "SDL_CreateRenderer error\n";
	}
	// Load bitmaps
	Bitmaps[0] = "img/board0.bmp";
	Bitmaps[1] = "img/figures.bmp";
	// Create textures from bitmaps
	for (auto bitmap : Bitmaps) {
		SDL_Surface * bitmapSurface = SDL_LoadBMP(bitmap.second.c_str());
		//transparent color -> red
		SDL_SetColorKey(bitmapSurface, SDL_TRUE, SDL_MapRGB(bitmapSurface->format, 255, 0, 0));
		// create texture with modified color (transparent)
		Textures[bitmap.first] = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
		if (Textures[bitmap.first] == NULL) {
			std::cout << bitmap.second.c_str() <<" SDL_CreateTextureFromSurface error\n";
		}
		else
			std::cout << bitmap.second.c_str() << " SDL_CreateTextureFromSurface OK\n";
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
			/* Mouse button down */
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_Point mousePos;
				mousePos.x = e.motion.x;
				mousePos.y = e.motion.y;
				if (e.button.button == SDL_BUTTON_LEFT){
					moves = toChessNote(mousePos).c_str();
				}
			}
			/* Mouse button up */
			if (e.type == SDL_MOUSEBUTTONUP) {
				SDL_Point mousePos;
				mousePos.x = e.motion.x;
				mousePos.y = e.motion.y;
				if (e.button.button == SDL_BUTTON_LEFT) {
					std::string newPos = toChessNote(mousePos);
					if (moves != newPos) {
						moves += newPos;
						std::cout << "Move to execute:" << moves.c_str() << std::endl;
						performMove(moves);
					}
					else
						moves = "";
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
	std::cout << "SDL_Quit\n";
	return 0;
}