#include <queue>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <cmath>

#ifdef _WIN32

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#elif defined __unix__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#elif defined __APPLE__

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#endif

#include "snake_sdl2.hpp"
#include "texture.hpp"
#include "snake.hpp"
#include "score.hpp"

int show_menu(void) {
	int c=1;
	SDL_Event e;
	while(1) {
		while(SDL_WaitEvent(&e)) {
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			gTitleTexture.render(120, 0);
			gMenuTexture.render(220, 280);
			if(e.type==SDL_QUIT) {
				exit(0);
			}
			else if(e.type==SDL_KEYDOWN) {
				switch(e.key.keysym.sym) {
					case SDLK_UP: c-=1; break;
					case SDLK_DOWN: c+=1; break;
					case SDLK_RETURN: return c; break;
				}
				if(c>4) c=1;
				if(c<1) c=4;
			}
			switch(c) {
				case 1: gBulletTexture.render(220+25, 280+72); break;
				case 2: gBulletTexture.render(220+25, 280+87); break;
				case 3: gBulletTexture.render(220+25, 280+102); break;
				case 4: gBulletTexture.render(220+25, 280+117); break;
			}
			SDL_RenderPresent(gRenderer);
		}
	}
}

void show_gameover(int a) {
	srand((unsigned int)time(NULL));
	SDL_RenderClear(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	gGameOverTexture.render(0, 80);
	char s[3];
	int2str(a, s);
	gScoreTexture.loadFromRenderedText(s, (SDL_Color){(Uint8)((rand()%128)+128), (Uint8)((rand()%128)+128), (Uint8)((rand()%128)+128), 0xFF}, SCORE_TEXT_SIZE);
	gScoreTexture.render(330, 80+245);
	SDL_RenderPresent(gRenderer);
	wait();
}

int classic_game(void) {
	srand((unsigned int)time(NULL));
	Uint8 r=((rand()%128)+128);
	Uint8 g=((rand()%128)+128);
	Uint8 b=((rand()%128)+128);
	Game_Snake S;
	int d=-1;
	SDL_Event e;
	Uint32 ticks;
	while(1) {
		ticks=SDL_GetTicks();
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, r, g, b, 0xFF);
		while(SDL_WaitEventTimeout(&e, 4)) {
			if(e.type==SDL_QUIT) {
				exit(0);
			}
			else if(e.type==SDL_KEYDOWN && e.key.repeat==0) {
				switch(e.key.keysym.sym) {
					case SDLK_ESCAPE:
						SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
						show_gameover(S.getscore());
						return S.getscore();
					case SDLK_UP:
						d=UP;
						break;
					case SDLK_DOWN:
						d=DOWN;
						break;
					case SDLK_RIGHT:
						d=RIGHT;
						break;
					case SDLK_LEFT:
						d=LEFT;
						break;
				}
				if(diff(S.getfdir(), d)!=2 && d!=S.getfdir()) {
					S.createnode(d);
				}
			}
		}
		S.movesnake();
		S.render();
		if(S.getcoll()) {
			SDL_Delay(TIMEOUT_LONG);
			SDL_RenderClear(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			wait();
			show_gameover(S.getscore());
			return S.getscore();
		}
		SDL_RenderPresent(gRenderer);
		while(SDL_GetTicks()-ticks<16) {
			SDL_Delay(1);
		}
	}
}

void fun_snake_game(void) {
	srand((unsigned int)time(NULL));
	Uint8 r=(rand()%0xFF);
	Uint8 g=(rand()%0xFF);
	Uint8 b=(rand()%0xFF);
	Snake S;
	int d=-1;
	SDL_Event e;
	Uint32 ticks;
	while(1) {
		ticks=SDL_GetTicks();
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, r, g, b, 0xFF);
		while(SDL_PollEvent(&e)) {
			if(e.type==SDL_QUIT) {
				exit(0);
			}
			else if(e.type==SDL_KEYDOWN && e.key.repeat==0) {
				switch(e.key.keysym.sym) {
					case SDLK_ESCAPE:
						SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
						return;
					case SDLK_UP:
						d=UP;
						break;
					case SDLK_DOWN:
						d=DOWN;
						break;
					case SDLK_RIGHT:
						d=RIGHT;
						break;
					case SDLK_LEFT:
						d=LEFT;
						break;
				}
				if(diff(S.getfdir(), d)!=2 && d!=S.getfdir()) {
					S.createnode(d);
				}
			}
		}
		S.movesnake();
		S.render();
    if(S.getcoll()) {
        SDL_Delay(TIMEOUT_LONG);
        SDL_RenderClear(gRenderer);
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        wait();
        return;
    }
		SDL_RenderPresent(gRenderer);
		while(SDL_GetTicks()-ticks<16) {
			SDL_Delay(1);
		}
	}
}

Point rand_point(std::deque<Cell>* cells) {
	Point p;
  bool flag;
	srand((unsigned int)time(NULL));
  do {
    flag=false;
    p.Y=(rand()%(SCREEN_HEIGHT-DOT_SIZE));
    p.X=(rand()%(SCREEN_WIDTH-DOT_SIZE));
    for(std::deque<Cell>::iterator it=cells->begin(); it!=cells->end(); it++) {
      if(p.X==it->p.X && p.Y==it->p.Y) {
        flag=true;
      }
    }
  }while(flag);
	return p;
}

int rand_score(void) {
	srand((unsigned int)time(NULL));
	return (rand()%MAX_SCORE)+1;
}

int rand_treat(void) {
	srand((unsigned int)time(NULL));
	return rand()%MAX_TREATS+1;
}

int getY(int Y) {
	if(Y>=DOT_SIZE/2 && Y<=SCREEN_HEIGHT-(DOT_SIZE/2)) {
		return Y;
	}
	else if(Y<DOT_SIZE/2) {
		return SCREEN_HEIGHT-(DOT_SIZE/2);
	}
	else {
		return DOT_SIZE/2;
	}
}

int getX(int X) {
	if(X>=DOT_SIZE/2 && X<=SCREEN_WIDTH-(DOT_SIZE/2)) {
		return X;
	}
	else if(X<DOT_SIZE/2) {
		return SCREEN_WIDTH-(DOT_SIZE/2);
	}
	else {
		return DOT_SIZE/2;
	}
}

int diff(int a, int b) {
	if((a-b)>0) {
		return a-b;
	}
	else {
		return b-a;
	}
}

void int2str(int a, char* s) {
	char temp;
	int i, j;
	if(a==0) {
		s[0]='0';
		s[1]=0;
	}
	else {
		for(i=0; a>0; a/=10, i++) {
			s[i]='0'+(a%10);
		}
		s[i]='\0';
		for(j=0; j<i/2; j++) {
			temp=s[j];
			s[j]=s[i-j-1];
			s[i-j-1]=temp;
		}
	}
}

void wait() {
	SDL_Event e;
	while(1) {
		while(SDL_PollEvent(&e)) {
			if(e.type==SDL_QUIT) {
				exit(0);
			}
			else if(e.type==SDL_KEYDOWN) {
				return;
			}
		}
	}
}

bool init() {
	bool success=true;
	if(SDL_Init(SDL_INIT_VIDEO)<0) {
		std::cout<<"SDL could not initialize! SDL Error: "<<SDL_GetError()<<std::endl;
		success=false;
	}
	else {
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			std::cout<<"Warning: Linear texture filtering not enabled!";
		}
		gWindow=SDL_CreateWindow("Snake II", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow==NULL) {
			std::cout<<"Window could not be created! SDL Error: "<<SDL_GetError()<<std::endl;
			success=false;
		}
		else {
			gRenderer=SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer==NULL) {
				std::cout<<"Renderer could not be created! SDL Error: "<<SDL_GetError()<<std::endl;
				success=false;
			}
			else {
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)) {
					std::cout<<"SDL_image could not initialize! SDL_image Error: "<<IMG_GetError()<<std::endl;
					success=false;
				}
				if(TTF_Init()==-1) {
					std::cout<<"SDL_ttf could not initialize! SDL_ttf Error: "<<TTF_GetError()<<std::endl;
					success=false;
				}
			}
		}
	}
	return success;
}

void close() {
	gDotTexture.free();
	gHeadTexture.free();
	gTreatTexture1.free();
	gTreatTexture2.free();
	gTreatTexture3.free();
	gTitleTexture.free();
	gMenuTexture.free();
	gBulletTexture.free();
	gGameOverTexture.free();
	gHighScoreTexture.free();
	gScoreTexture.free();
	gTextTexture.free();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow=NULL;
	gRenderer=NULL;
	IMG_Quit();
	SDL_Quit();
}
