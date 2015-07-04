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

Snake::Snake(int len) {
  int i;
  Cell temp;
  temp.p.Y=DOT_SIZE/2;
  temp.dir=RIGHT;
  for(i=0; i<len; i++) {
    temp.p.X=i*DOT_SIZE+(DOT_SIZE/2);
    cells.push_front(temp);
  }
  Collide=0;
}

void Snake::movesnake(void) {
  for(std::deque<Cell>::iterator itc=cells.begin(); itc!=cells.end(); ++itc) {
    for(std::deque<Node>::iterator itn=nodes.begin(); itn!=nodes.end(); ++itn) {
      if(itc->p.X==itn->p.X && itc->p.Y==itn->p.Y) {
        itc->dir=itn->dir;
        itn->mark+=1;
        if(itn->mark==cells.size()) {
          nodes.erase(itn);
        }
        break;
      }
    }
    switch(itc->dir) {
      case UP:
        itc->p.Y=getY(itc->p.Y-1);
        break;
      case DOWN:
        itc->p.Y=getY(itc->p.Y+1);
        break;
      case RIGHT:
        itc->p.X=getX(itc->p.X+1);
        break;
      case LEFT:
        itc->p.X=getX(itc->p.X-1);
        break;
    }
  }
  if(collide()) {
    Collide=1;
  }
}

void Snake::render(void) {
  int Y, X;
  Y=cells.front().p.Y;
  X=cells.front().p.X;
  for(std::deque<Cell>::iterator it=cells.begin(); it!=cells.end(); ++it){
    if(it==cells.begin()) {
      gHeadTexture.render(it->p.X-(DOT_SIZE/2), it->p.Y-(DOT_SIZE/2));
    }
    else {
      gDotTexture.render(it->p.X-(DOT_SIZE/2), it->p.Y-(DOT_SIZE/2));
    }
  }
}

int Snake::collide(void) {
  for(std::deque<Cell>::iterator it=cells.begin(); it!=cells.end(); ++it){
      if(diff(cells.front().p.Y, it->p.Y)<(DOT_SIZE/3) && diff(cells.front().p.X, it->p.X)<(DOT_SIZE/3) && it!=cells.begin()) {
          return 1;
      }
  }
  return 0;
}

void Snake::createnode(int dir) {
  Node t;
  t.p.X=cells.front().p.X;
  t.p.Y=cells.front().p.Y;
  t.dir=dir;
  t.mark=0;
  nodes.push_back(t);
}

int Snake::getfdir(void) {
  return cells.front().dir;
}

int Snake::getcoll(void) {
  return Collide;
}

Game_Snake::Game_Snake():Snake(2) {
  T.p.Y=SCREEN_HEIGHT/2;
  T.p.X=SCREEN_WIDTH/2;
  T.points=1;
  T.type=rand_treat();
  score=0;
  gspeed=1;
}

void Game_Snake::addtreat(void) {
  T.p=rand_point(&cells);
  T.points=rand_score();
  T.type=rand_treat();
}

void Game_Snake::movesnake(void) {
  Cell temp;
  for(std::deque<Cell>::iterator itc=cells.begin(); itc!=cells.end(); ++itc) {
    for(std::deque<Node>::iterator itn=nodes.begin(); itn!=nodes.end(); ++itn) {
      if(itc->p.X==itn->p.X && itc->p.Y==itn->p.Y) {
        itc->dir=itn->dir;
        itn->mark+=1;
        if(itn->mark==cells.size()) {
          nodes.erase(itn);
        }
        break;
      }
    }
    switch(itc->dir) {
      case UP:
        itc->p.Y=getY(itc->p.Y-1);
        break;
      case DOWN:
        itc->p.Y=getY(itc->p.Y+1);
        break;
      case RIGHT:
        itc->p.X=getX(itc->p.X+1);
        break;
      case LEFT:
        itc->p.X=getX(itc->p.X-1);
        break;
    }
  }
  if(diff(cells.front().p.X, T.p.X)<4*(DOT_SIZE/3) && diff(cells.front().p.Y, T.p.Y)<4*(DOT_SIZE/3)) {
    switch(cells.back().dir) {
      case UP:
        temp.p.Y=getY(cells.back().p.Y+DOT_SIZE);
        temp.p.X=cells.back().p.X;
        break;
      case DOWN:
        temp.p.Y=getY(cells.back().p.Y-DOT_SIZE);
        temp.p.X=cells.back().p.X;
        break;
      case RIGHT:
        temp.p.X=getX(cells.back().p.X-DOT_SIZE);
        temp.p.Y=cells.back().p.Y;
        break;
      case LEFT:
        temp.p.X=getX(cells.back().p.X+DOT_SIZE);
        temp.p.Y=cells.back().p.Y;
        break;
    }
    temp.dir=cells.back().dir;
    cells.push_back(temp);
    score+=T.points;
    addtreat();
  }
  if(collide()) {
    Collide=1;
  }
}

void Game_Snake::render(void) {
  int Y, X;
  Y=cells.front().p.Y;
  X=cells.front().p.X;
  for(std::deque<Cell>::iterator it=cells.begin(); it!=cells.end(); ++it){
    if(it==cells.begin()) {
      gHeadTexture.render(it->p.X-(DOT_SIZE/2), it->p.Y-(DOT_SIZE/2));
    }
    else {
      gDotTexture.render(it->p.X-(DOT_SIZE/2), it->p.Y-(DOT_SIZE/2));
    }
  }
  switch(T.type) {
    case 1: gTreatTexture1.render(T.p.X-3*(DOT_SIZE/2), T.p.Y-3*(DOT_SIZE/2)); break;
    case 2: gTreatTexture2.render(T.p.X-3*(DOT_SIZE/2), T.p.Y-3*(DOT_SIZE/2)); break;
    case 3: gTreatTexture3.render(T.p.X-3*(DOT_SIZE/2), T.p.Y-3*(DOT_SIZE/2)); break;
  }
}

int Game_Snake::getscore(void) {
    return score;
}
