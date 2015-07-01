class Texture {
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
public:
	Texture() {
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
	~Texture() {
		free();
	}
	bool loadFromFile(std::string path) {
		free();
		SDL_Texture* newTexture=NULL;
		SDL_Surface* loadedSurface=IMG_Load(path.c_str());
		if(loadedSurface==NULL) {
			std::cout<<"Unable to load image "<<path.c_str()<<"! SDL_image Error: "<<IMG_GetError()<<std::endl;
		}
		else {
			//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
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
	bool loadFromRenderedText(char* textureText, SDL_Color textColor, int size) {
		TTF_Init();
		TTF_Font* gFont=TTF_OpenFont(FONT_FILE, size);
        if(gFont==NULL) {
            std::cout<<"Unable to Open Font! TTF Error: "<<SDL_GetError()<<std::endl;
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
	void free() {
		if(mTexture!= NULL) {
			SDL_DestroyTexture(mTexture);
			mTexture = NULL;
			mWidth = 0;
			mHeight = 0;
		}
	}
	void setColor(Uint8 red, Uint8 green, Uint8 blue) {
		SDL_SetTextureColorMod(mTexture, red, green, blue);
	}
	void setBlendMode(SDL_BlendMode blending) {
		SDL_SetTextureBlendMode(mTexture, blending);
	}
	void setAlpha(Uint8 alpha) {
		SDL_SetTextureAlphaMod(mTexture, alpha);
	}
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) {
		SDL_Rect renderQuad={x, y, mWidth, mHeight};
		if(clip!=NULL) {
			renderQuad.w=clip->w;
			renderQuad.h=clip->h;
		}
		SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
	}
	int getWidth();
	int getHeight();
}gDotTexture,
gHeadTexture,
gTreatTexture1, 
gTreatTexture2, 
gTreatTexture3, 
gTitleTexture, 
gMenuTexture, 
gBulletTexture,
gGameOverTexture,
gHighScoreTexture,
gScoreTexture,
gTextTexture;

class Snake {
protected:
	std::deque<Node> nodes;
	std::deque<Cell> cells;
    int Collide;
public:
	Snake() {
		int i;
		Cell temp;
		temp.p.Y=DOT_SIZE/2;
		temp.dir=RIGHT;
		for(i=0; i<DEF_LEN; i++) {
			temp.p.X=i*DOT_SIZE+(DOT_SIZE/2);
			cells.push_front(temp);
		}
        Collide=0;
	}
	void movesnake(void) {
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
	void render(void) {
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
    int collide(void) {
        for(std::deque<Cell>::iterator it=cells.begin(); it!=cells.end(); ++it){
            if(diff(cells.front().p.Y, it->p.Y)<(DOT_SIZE/3) && diff(cells.front().p.X, it->p.X)<(DOT_SIZE/3) && it!=cells.begin()) {
                return 1;
            }
        }
        return 0;
    }
	void createnode(int dir) {
		Node t;
		t.p.X=cells.front().p.X;
		t.p.Y=cells.front().p.Y;
		t.dir=dir;
		t.mark=0;
		nodes.push_back(t);
	}
	int getfdir(void) {
		return cells.front().dir;
	}
    
    int getcoll(void) {
        return Collide;
    }
};

class Game_Snake {
protected:
    std::deque<Node> nodes;
    std::deque<Cell> cells;
    int score, Collide, gspeed;
    Treat T;
public:
    Game_Snake() {
        int i;
        Cell temp;
        temp.p.Y=DOT_SIZE/2;
        temp.dir=RIGHT;
        for(i=0; i<DEF_LEN_CLASSIC; i++) {
            temp.p.X=i*DOT_SIZE+(DOT_SIZE/2);
            cells.push_front(temp);
        }
        T.p.Y=SCREEN_HEIGHT/2;
        T.p.X=SCREEN_WIDTH/2;
        T.points=1;
        T.type=rand_treat();
        score=0;
        Collide=0;
        gspeed=1;
    }
    void addtreat(void) {
        T.p=rand_point();
        T.points=rand_score();
        T.type=rand_treat();
    }
    void movesnake(void) {
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
    void render(void) {
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
    int collide(void) {
        for(std::deque<Cell>::iterator it=cells.begin(); it!=cells.end(); ++it){
            if(diff(cells.front().p.Y, it->p.Y)<(DOT_SIZE/3) && diff(cells.front().p.X, it->p.X)<(DOT_SIZE/3) && it!=cells.begin()) {
                return 1;
            }
        }
        return 0;
    }
    void createnode(int dir) {
        Node t;
        t.p.X=cells.front().p.X;
        t.p.Y=cells.front().p.Y;
        t.dir=dir;
        t.mark=0;
        nodes.push_back(t);
    }
    int getfdir(void) {
        return cells.front().dir;
    }
    
    int getcoll(void) {
        return Collide;
    }
    int getscore(void) {
        return score;
    }
};

class Score {
	int scores[14];
	int head;
public:
	Score() {
		head=-1;
		for(int i=0; i<14; i++) {
			scores[i]=-1;
		}
	}
	void filewrite() {
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
	void fileread() {
		std::ifstream file;
		file.open(FILE_NAME, std::ios::in | std::ios::binary);
		if(file.is_open()) {
			file.read((char *)this, sizeof(Score));
		}
		file.close();
		sortscore();
	}
	void addscore(int s) {
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
	void sortscore(void) {
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
	void showscores(void) {
		sortscore();
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
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
				gTextTexture.loadFromRenderedText(str, {(Uint8)(rand()%0xFF), (Uint8)(rand()%0xFF), (Uint8)(rand()%0xFF), 0xFF}, SMALL_TEXT_SIZE);
				gTextTexture.render((SCREEN_WIDTH/2)-(5*6), 50+((t+j)*20));
			}
		}
		SDL_RenderPresent(gRenderer);
		wait();
	}
	int gethead(void) {
		return head;
	}
};