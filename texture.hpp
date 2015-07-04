#ifndef TEXTURE_HPP
#define TEXTURE_HPP

class Texture {
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
public:
	Texture();
	~Texture();
	bool loadFromFile(std::string path);
	bool loadFromRenderedText(char* textureText, SDL_Color textColor, int size);
	void free();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	int getWidth();
	int getHeight();
};

extern Texture gDotTexture;
extern Texture gHeadTexture;
extern Texture gTreatTexture1;
extern Texture gTreatTexture2;
extern Texture gTreatTexture3;
extern Texture gTitleTexture;
extern Texture gMenuTexture;
extern Texture gBulletTexture;
extern Texture gGameOverTexture;
extern Texture gHighScoreTexture;
extern Texture gScoreTexture;
extern Texture gTextTexture;

#endif
