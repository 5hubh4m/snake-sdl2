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

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

Texture gDotTexture;
Texture gHeadTexture;
Texture gTreatTexture1;
Texture gTreatTexture2;
Texture gTreatTexture3;
Texture gTitleTexture;
Texture gMenuTexture;
Texture gBulletTexture;
Texture gGameOverTexture;
Texture gHighScoreTexture;
Texture gScoreTexture;
Texture gTextTexture;

Texture::Texture() {
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

Texture::~Texture() {
  free();
}

bool Texture::loadFromFile(std::string path) {
  free();
  SDL_Texture* newTexture=NULL;
  SDL_Surface* loadedSurface=IMG_Load(path.c_str());
  if(loadedSurface==NULL) {
    std::cout<<"Unable to load image "<<path.c_str()<<"! SDL_image Error: "<<IMG_GetError()<<std::endl;
  }
  else {
    newTexture=SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if(newTexture==NULL) {
      std::cout<<"Unable to create texture from "<<path.c_str()<<"! SDL Error: "<<SDL_GetError()<<std::endl;
    }
    else {
      mWidth=loadedSurface->w;
      mHeight=loadedSurface->h;
    }
    SDL_FreeSurface(loadedSurface);
  }
  mTexture=newTexture;
  return mTexture!=NULL;
}

bool Texture::loadFromRenderedText(char* textureText, SDL_Color textColor, int size) {
  TTF_Font* gFont=TTF_OpenFont(FONT_FILE, size);
  if(gFont==NULL) {
    std::cout<<"Unable to Open Font! TTF Error: "<<TTF_GetError()<<std::endl;
    return false;
  }
  else {
    free();
    SDL_Surface* textSurface=TTF_RenderText_Solid(gFont, textureText, textColor);
    if(textSurface!=NULL) {
      mTexture=SDL_CreateTextureFromSurface(gRenderer, textSurface);
      if(mTexture==NULL) {
        std::cout<<"Unable to create texture from rendered text! SDL Error: "<<SDL_GetError()<<std::endl;
      }
      else {
        mWidth = textSurface->w;
        mHeight = textSurface->h;
      }
      SDL_FreeSurface(textSurface);
    }
    else {
      std::cout<<"Unable to render text surface! SDL_ttf Error: "<<TTF_GetError()<<std::endl;
    }
    return mTexture!=NULL;
  }
}

void Texture::free() {
  if(mTexture!= NULL) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending) {
  SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(Uint8 alpha) {
  SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
  SDL_Rect renderQuad={x, y, mWidth, mHeight};
  if(clip!=NULL) {
    renderQuad.w=clip->w;
    renderQuad.h=clip->h;
  }
  SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth() {
  return mWidth;
}

int Texture::getHeight() {
  return mHeight;
}
