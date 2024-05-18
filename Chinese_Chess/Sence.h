#pragma once

#include <easyx.h>

#define BoardRow 10
#define BoardCol 9

#define PieceSize 80


#define EMPTY -1

enum _PIECES {
	R_King, R_Guard, R_Minister, R_Horse, R_Chariot, R_Cannon, R_Soldier,
	B_King, B_Guard, B_Minister, B_Horse, B_Chariot, B_Cannon, B_Soldier,
};


class Sence
{
public:
	void Init();
	void PlayGame();


private :
	void InitBoard();
	void displayText();
	void Draw();
	void test(); // ����̨������и��ӵ�����
	void getMousePosition(ExMessage& msg, int& row, int& col);
	bool RuleAllow(int type, int curRow, int curCol, int tarRow, int tarCol);
	bool KingAllow(int type, int curRow, int curCol, int tarRow, int tarCol);
	bool GuardAllow(int type, int curRow, int curCol, int tarRow, int tarCol);
	bool MinisterAllow(int type, int curRow, int curCol, int tarRow, int tarCol);
	bool HorseAllow(int type, int curRow, int curCol, int tarRow, int tarCol);
	bool ChariotAllow(int type, int curRow, int curCol, int tarRow, int tarCol);
	bool CannonAllow(int type, int curRow, int curCol, int tarRow, int tarCol);
	bool SoldierAllow(int type, int curRow, int curCol, int tarRow, int tarCol);
	bool isRed(int type);
	bool isBlack(int type);
	bool TurnToRed(int Turns) { return Turns % 2 == 0; }
	bool TurnToBlack(int Turns) { return Turns % 2 == 1; }



private:

	int Board[BoardRow][BoardCol];
	int Turns;	// % 2 Ϊ 0 Ϊ�췽��% 2 Ϊ 1 Ϊ�ڷ�
};

