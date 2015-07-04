#ifndef SNAKE_HPP
#define SNAKE_HPP

class Snake {
protected:
	std::deque<Node> nodes;
	std::deque<Cell> cells;
  int Collide;
public:
	Snake(int len=DEF_LEN);
	virtual void movesnake(void);
	virtual void render(void);
  int collide(void);
	void createnode(int dir);
	int getfdir(void);
  int getcoll(void);
};

class Game_Snake:public Snake {
protected:
    int score, gspeed;
    Treat T;
public:
    Game_Snake();
    void addtreat(void);
    virtual void movesnake(void);
    virtual void render(void);
    int getscore(void);
};

#endif
