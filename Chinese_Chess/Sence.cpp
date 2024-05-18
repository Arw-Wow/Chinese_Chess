#include "Sence.h"
// #include "Tools.h"
// #include <easyx.h>
#include <time.h>
#include <iostream>

void Sence::Init()
{
	Turns = 0;

	initgraph(800, 800 + 50);
	setbkcolor(WHITE);
	cleardevice();
	
	InitBoard();

	Draw();

	displayText();

	test();
}

void Sence::PlayGame()
{
	ExMessage chose_msg;
	ExMessage lay_msg;
	while (1) {

		// 获取鼠标选择位置
		int chose_row = -1, chose_col = -1;
		do {			
			getMousePosition(chose_msg, chose_row, chose_col);
		} while (Board[chose_row][chose_col] == EMPTY);	// 直到选到一个有棋的位置

		int type = Board[chose_row][chose_col];

		// 如果选到的不是该回合的棋子，就再选一遍
		if (TurnToRed(Turns) && isBlack(type))
			continue;
		if (TurnToBlack(Turns) && isRed(type))
			continue;


		// 获取鼠标放置位置
		int lay_row = -1, lay_col = -1;
		do {			
			getMousePosition(lay_msg, lay_row, lay_col);
		} while (lay_row == chose_row && lay_col == chose_col);

		int layType = Board[lay_row][lay_col];
	
		// 规则不允许就重新落子
		if (!RuleAllow(type, chose_row, chose_col, lay_row, lay_col))	
			continue;

		// 如果规则允许：如果有棋子就吃掉
		Board[chose_row][chose_col] = EMPTY;
		Board[lay_row][lay_col] = type;



		// std::cout << "已完成一次落子" << std::endl;

		Draw();

		// 交换
		Turns++;

		displayText();

		if (layType == R_King) {
			MessageBox(NULL, "黑方胜利！", "游戏结束", MB_OK);
			exit(-1);
		}
		if (layType == B_King) {
			MessageBox(NULL, "红方胜利！", "游戏结束", MB_OK);
			exit(-1);
		}

		test();
	}
}

void Sence::InitBoard()
{
	for (int i = 0; i < BoardRow; i++) {
		for (int j = 0; j < BoardCol; j++) {
			Board[i][j] = EMPTY;
		}
	}

	Board[0][0] = B_Chariot;
	Board[0][1] = B_Horse;
	Board[0][2] = B_Minister;
	Board[0][3] = B_Guard;
	Board[0][4] = B_King;
	Board[2][1] = B_Cannon;
	Board[3][0] = B_Soldier;
	Board[3][2] = B_Soldier;
	Board[3][4] = B_Soldier;

	Board[9][0] = R_Chariot;
	Board[9][1] = R_Horse;
	Board[9][2] = R_Minister;
	Board[9][3] = R_Guard;
	Board[9][4] = R_King;
	Board[7][1] = R_Cannon;
	Board[6][0] = R_Soldier;
	Board[6][2] = R_Soldier;
	Board[6][4] = R_Soldier;


	for (int i = 0; i < BoardRow; i++) {
		for (int j = 4; j < BoardCol; j++) {
			Board[i][j] = Board[i][8 - j];
		}
	}
}

void Sence::displayText()
{
	// static clock_t begin_time = clock();
	// char totalTime[100] = { 0 };
	// sprintf_s(totalTime, sizeof(totalTime), "对局总用时：%d s", int((clock() - begin_time) / 1000));
	// settextcolor(BLACK);
	// outtextxy(300, 815, totalTime);

	settextcolor(BLACK);
	if (TurnToRed(Turns)) {
		outtextxy(350, 815, "当前轮到：红方");
	}
	else {
		outtextxy(350, 815, "当前轮到：黑方");
	}
}

void Sence::Draw()
{
	// 每一次都重新画，先画棋盘，再画棋子
	// loadimage(0, "res/image/board.png", 800, 800);

	IMAGE* m = new IMAGE;
	loadimage(m, "res/image/board.png");
	putimage(0, 0, m);

	// loadimage(m, "res/image/0.png");
	// putimage(100, 0, m);

	char imageFileName[100] = { 0 };

	for (int i = 0; i < BoardRow; i++) {
		for (int j = 0; j < BoardCol; j++) {
			if (Board[i][j] == -1)	continue;

			sprintf_s(imageFileName, sizeof(imageFileName), "res/image/%d.png", Board[i][j]);
			
			loadimage(m, imageFileName);
			// Tools::putimage_alpha(PieceSize / 2 + j * PieceSize, i * PieceSize, m);
			putimage(PieceSize / 2 + j * PieceSize, i * PieceSize, PieceSize, PieceSize, m, 0, 0);
			// putimage(PieceSize / 2 + j * PieceSize, i * PieceSize, m);
		}
	}



	delete m;
}

void Sence::test()
{
	for (int i = 0; i < BoardRow; i++) {
		for (int j = 0; j < BoardCol; j++) {
			std::cout << Board[i][j] << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Sence::getMousePosition(ExMessage& msg, int& row, int& col)
{
	while (1) {	//如果获取位置失败，就一直获取
		getmessage(&msg, EX_MOUSE);
		if (msg.message == WM_LBUTTONDOWN) {
			for (int i = 0; i < BoardRow; i++) {
				for (int j = 0; j < BoardCol; j++) {
					int x = PieceSize / 2 + j * PieceSize;
					int y = i * PieceSize;
					if (msg.x > x && msg.x < x + PieceSize && msg.y > y && msg.y < y + PieceSize) {
						row = i;
						col = j;
						return;
					}
				}
			}
		}
	}
}

bool Sence::RuleAllow(int type, int curRow, int curCol, int tarRow, int tarCol)
{
	bool ret = false;

	// 控制台追踪
	std::cout << "(" << curRow << "," << curCol << ") -> (" << tarRow << "," << tarCol << ")" << std::endl;

	// 如果是同一方就不允许
	int tarType = Board[tarRow][tarCol];
	if (isRed(type) && isRed(tarType) || isBlack(type) && isBlack(tarType))
		return false;


	switch (type)
	{
	case R_King: case B_King:
		ret = KingAllow(type, curRow, curCol, tarRow, tarCol);
		break;

	case R_Guard: case B_Guard:
		ret = GuardAllow(type, curRow, curCol, tarRow, tarCol);
		break;

	case R_Minister: case B_Minister:
		ret = MinisterAllow(type, curRow, curCol, tarRow, tarCol);
		break;
	
	case R_Horse: case B_Horse:
		ret = HorseAllow(type, curRow, curCol, tarRow, tarCol);
		break;

	case R_Chariot: case B_Chariot:
		ret = ChariotAllow(type, curRow, curCol, tarRow, tarCol);
		break;

	case R_Cannon: case B_Cannon:
		ret = CannonAllow(type, curRow, curCol, tarRow, tarCol);
		break;

	case R_Soldier: case B_Soldier:
		ret = SoldierAllow(type, curRow, curCol, tarRow, tarCol);
		break;
	}

	return ret;
}

bool Sence::KingAllow(int type, int curRow, int curCol, int tarRow, int tarCol)
{
	// return true;
	if (type == R_King) {
		// 超出田字格
		if (tarRow < 7 || tarCol < 3 || tarCol > 5)
			return false;

		// 移动不是上下左右一格
		int row_change = abs(curRow - tarRow);
		int col_change = abs(curCol - tarCol);
		if (!(row_change == 1 && col_change == 0 || row_change == 0 && col_change == 1))
			return false;

		return true;
	}
	else {
		// 超出田字格
		if (tarRow > 2 || tarCol < 3 || tarCol > 5)
			return false;

		// 移动不是上下左右一格
		int row_change = abs(curRow - tarRow);
		int col_change = abs(curCol - tarCol);
		if (!(row_change == 1 && col_change == 0 || row_change == 0 && col_change == 1))
			return false;

		return true;

	}
	return false;
}

bool Sence::GuardAllow(int type, int curRow, int curCol, int tarRow, int tarCol)
{
	if (type == R_Guard) {
		// 超出田字格
		if (tarRow < 7 || tarCol < 3 || tarCol > 5)
			return false;

		int row_change = abs(curRow - tarRow);
		int col_change = abs(curCol - tarCol);

		if (row_change == 1 && col_change == 1)
			return true;

		return false;

	}
	else {
		// 超出田字格
		if (tarRow > 2 || tarCol < 3 || tarCol > 5)
			return false;

		int row_change = abs(curRow - tarRow);
		int col_change = abs(curCol - tarCol);

		if (row_change == 1 && col_change == 1)
			return true;

		return false;
	}
	return false;
}

bool Sence::MinisterAllow(int type, int curRow, int curCol, int tarRow, int tarCol)
{
	if (type == R_Minister) {
		// 到对面
		if (tarRow < 5)
			return false;


		int row_change = abs(curRow - tarRow);
		int col_change = abs(curCol - tarCol);

		if (row_change == 2 && col_change == 2) {
			int drow = (tarRow + curRow) / 2;
			int dcol = (tarCol + curCol) / 2;

			if (Board[drow][dcol] != EMPTY)
				return false;

			return true;
		}

		return false;
	}
	else {
		// 到对面
		if (tarRow > 4)
			return false;


		int row_change = abs(curRow - tarRow);
		int col_change = abs(curCol - tarCol);

		if (row_change == 2 && col_change == 2) {
			int drow = (tarRow + curRow) / 2;
			int dcol = (tarCol + curCol) / 2;

			if (Board[drow][dcol] != EMPTY)
				return false;

			return true;
		}

		return false;

	}
	return false;
}

bool Sence::HorseAllow(int type, int curRow, int curCol, int tarRow, int tarCol)
{
	// 红马 黑马 规则一样

	// 蹩马脚
	if ((tarRow > curRow + 1 && Board[curRow + 1][curCol] != EMPTY) ||
		(tarRow < curRow - 1 && Board[curRow - 1][curCol] != EMPTY) ||
		(tarCol > curCol + 1 && Board[curRow][curCol + 1] != EMPTY) ||
		(tarCol < curCol - 1 && Board[curRow][curCol - 1] != EMPTY)
		)
		return false;

	int row_change = abs(curRow - tarRow);
	int col_change = abs(curCol - tarCol);

	if (row_change == 2 && col_change == 1 || row_change == 1 && col_change == 2)
		return true;

	return false;
}

bool Sence::ChariotAllow(int type, int curRow, int curCol, int tarRow, int tarCol)
{
	// 红车 黑车 规则一样

	int row_change = abs(curRow - tarRow);
	int col_change = abs(curCol - tarCol);

	// 如果走的不是直线
	if (row_change && col_change)
		return false;

	if (row_change) {
		int max = curRow > tarRow ? curRow : tarRow;
		int min = curRow < tarRow ? curRow : tarRow;
		for (int i = min + 1; i < max; i++) {
			if (Board[i][curCol] != EMPTY)
				return false;
		}
		return true;
	}
	else if (col_change) {
		int max = curCol > tarCol ? curCol : tarCol;
		int min = curCol < tarCol ? curCol : tarCol;
		for (int i = min + 1; i < max; i++) {
			if (Board[curRow][i] != EMPTY)
				return false;
		}
		return true;
	}
	return false;
}

bool Sence::CannonAllow(int type, int curRow, int curCol, int tarRow, int tarCol)
{
	// 红炮 黑炮 规则一样

	bool jump = false;

	int row_change = abs(curRow - tarRow);
	int col_change = abs(curCol - tarCol);

	// 如果走的不是直线
	if (row_change && col_change)
		return false;

	if (row_change) {
		int max = curRow > tarRow ? curRow : tarRow;
		int min = curRow < tarRow ? curRow : tarRow;
		for (int i = min + 1; i < max; i++) {
			if (Board[i][curCol] != EMPTY && jump)
				return false;

			if (Board[i][curCol] != EMPTY && !jump)
				jump = true;
		}

		// 如果没有隔一个子，就不能吃子
		if (!jump && Board[tarRow][tarCol] != EMPTY)
			return false;

		return true;
	}
	else if (col_change) {
		int max = curCol > tarCol ? curCol : tarCol;
		int min = curCol < tarCol ? curCol : tarCol;
		for (int i = min + 1; i < max; i++) {
			if (Board[curRow][i] != EMPTY && jump)
				return false;

			if (Board[curRow][i] != EMPTY && !jump)
				jump = true;

		}

		if (!jump && Board[tarRow][tarCol] != EMPTY)
			return false;

		return true;
	}
	return false;

}

bool Sence::SoldierAllow(int type, int curRow, int curCol, int tarRow, int tarCol)
{
	if (type == R_Soldier) {
		// 过没过河
		if (curRow < 5) {
			// 过河
			int col_change = abs(tarCol - curCol);
			if (tarRow == curRow - 1 && col_change == 0 || tarRow == curRow && col_change == 1)
				return true;

			return false;
		}
		else {
			// 没过河
			if (tarRow == curRow - 1 && tarCol == curCol)
				return true;

			return false;
		}
	}
	else {
		// 过没过河
		if (curRow > 4) {
			// 过河
			int col_change = abs(tarCol - curCol);
			if (tarRow == curRow + 1 && col_change == 0 || tarRow == curRow && col_change == 1)
				return true;

			return false;
		}
		else {
			// 没过河
			if (tarRow == curRow + 1 && tarCol == curCol)
				return true;

			return false;
		}

	}
	return false;
}

bool Sence::isRed(int type)
{
	if (type >= 0 && type <= 6)
		return true;

	return false;
}

bool Sence::isBlack(int type)
{
	if (type >= 7 && type <= 13)
		return true;

	return false;
}
