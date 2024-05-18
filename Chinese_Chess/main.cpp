#include <iostream>
#include "Sence.h"

int main() {
	Sence* sence = new Sence;

	sence->Init();
	sence->PlayGame();

	getchar();

	delete sence;


	return 0;
}