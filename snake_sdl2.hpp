#ifndef SNAKE_SDL2_HPP
#define SNAKE_SDL2_HPP

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640
#define DEF_LEN_CLASSIC 5
#define DEF_LEN 20
#define DOT_SIZE 10

#define MAX_ROW 47
#define MAX_COL 63

#define SCORE_TEXT_SIZE 40
#define SMALL_TEXT_SIZE 20

#define MAX_SCORE 10
#define MAX_TREATS 3

#define FILE_NAME "snake.db"
#define DOT_FILE "res/dot.png"
#define HEAD_FILE "res/head.png"
#define TITLE_FILE "res/title.png"
#define GAMEOVER_FILE "res/gameover.png"
#define HIGHSCORE_FILE "res/highscore.png"
#define MENU_FILE "res/menu.png"
#define BULLET_FILE "res/bullet.png"
#define TREAT_1 "res/treat1.png"
#define TREAT_2 "res/treat2.png"
#define TREAT_3 "res/treat3.png"
#define FONT_FILE "res/font.ttf"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define TIMEOUT_SHORT 20
#define TIMEOUT 100
#define TIMEOUT_LONG 1000
#define GAME_SPEED 4

struct Point{
	int Y, X;
};

struct Cell {
	Point p;
	int dir;
};

struct Node {
	Point p;
	int dir, mark;
};

struct Treat {
	Point p;
	int points;
	int type;
};

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

extern TTF_Font* gFont;

int show_menu(void);
void show_gameover(int);
Point rand_point(std::deque<Cell>*);
int rand_score(void);
int rand_treat(void);
int getX(int);
int getY(int);
int diff(int, int);
void int2str(int, char*);
int classic_game(void);
void fun_snake_game(void);
void wait(void);
bool init();
void close();

#endif
