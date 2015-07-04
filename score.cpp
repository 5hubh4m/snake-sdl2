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

Score::Score() {
  head=-1;
  for(int i=0; i<14; i++) {
    scores[i]=-1;
  }
}

void Score::filewrite() {
  std::ofstream file;
  if(head>-1) {
    if(file.is_open()) {
      file.close();
    }
    file.open(FILE_NAME, std::ios::trunc | std::ios::binary);
    file.write((char *)this, sizeof(Score));
    file.close();
  }
}

void Score::fileread() {
  std::ifstream file;
  file.open(FILE_NAME, std::ios::in | std::ios::binary);
  if(file.is_open()) {
    file.read((char *)this, sizeof(Score));
  }
  file.close();
  sortscore();
}

void Score::addscore(int s) {
  sortscore();
  if(head<13) {
    scores[++head]=s;
  }
  else {
    if(s>scores[0]) {
      scores[0]=s;
    }
  }
  sortscore();
}

void Score::sortscore(void) {
  int temp;
  if(head>0) {
    for(int i=0; i<=head; i++) {
      for(int j=0; j<=head-1; j++) {
        if(scores[j]>scores[j+1]) {
          temp=scores[j+1];
          scores[j+1]=scores[j];
          scores[j]=temp;
        }
      }
    }
  }
}

void Score::showscores(void) {
  sortscore();
  SDL_RenderClear(gRenderer);
  SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_Color color={(Uint8)(rand()%0xFF), (Uint8)(rand()%0xFF), (Uint8)(rand()%0xFF), 0xFF};
  gHighScoreTexture.render(120, 0);
  char str[10], s1[3], s2[4];
  int t=(23-head-1)/2;
  if(head>-1) {
    for(int i=head, j=0; i>=0; i--,  j++) {
      int2str(j+1, s1);
      int2str(scores[i], s2);
      strcpy(str, s1);
      strcat(str, ". ");
      strcat(str, s2);
      gTextTexture.loadFromRenderedText(str, color, SMALL_TEXT_SIZE);
      gTextTexture.render((SCREEN_WIDTH/2)-(5*6), 50+((t+j)*20));
    }
  }
  SDL_RenderPresent(gRenderer);
  wait();
}

int Score::gethead(void) {
  return head;
}
