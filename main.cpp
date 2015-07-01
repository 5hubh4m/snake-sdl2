#include "snake_header.hpp"

int main(void) {
	Score S;
	int ch, a;
	srand((unsigned int)time(NULL));
	if(!init()) {
		std::cout<<"Failed to initialize!"<<std::endl;
	}
	S.fileread();
	if(!gDotTexture.loadFromFile(DOT_FILE)
		|| !gHeadTexture.loadFromFile(HEAD_FILE)
		|| !gTitleTexture.loadFromFile(TITLE_FILE)
		|| !gMenuTexture.loadFromFile(MENU_FILE)
		|| !gBulletTexture.loadFromFile(BULLET_FILE)
		|| !gTreatTexture1.loadFromFile(TREAT_1)
		|| !gTreatTexture2.loadFromFile(TREAT_2)
		|| !gTreatTexture3.loadFromFile(TREAT_3)
		|| !gGameOverTexture.loadFromFile(GAMEOVER_FILE)
		|| !gHighScoreTexture.loadFromFile(HIGHSCORE_FILE)) {
		std::cout<<"Error!"<<std::endl;
		return 1;
	}
	SDL_RenderClear(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	while(1) {
		switch(ch=show_menu()) {
			case 1:
				a=classic_game();
				if(a>0) S.addscore(a);
				break;
			case 2:
				fun_snake_game();
				break;
			case 3:
				S.showscores();
				break;
			case 4:
				close();
				S.filewrite();
				return 0;
		}
	}
}
