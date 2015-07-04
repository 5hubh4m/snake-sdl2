#ifndef SCORE_HPP
#define SCORE_HPP

class Score {
	int scores[14];
	int head;
public:
	Score();
	void filewrite();
	void fileread();
	void addscore(int s);
	void sortscore(void);
	void showscores(void);
	int gethead(void);
};

#endif
