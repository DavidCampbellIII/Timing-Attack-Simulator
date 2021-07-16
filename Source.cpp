#include <iostream>
#include "Menu.h"

int main()
{
	const char MIN_VALID_CHAR = '!';
	const char MAX_VALID_CHAR = '~';
	const int MAX_PASSWORD_LENGTH = 128;

	//make sure console window is large enough to display all the fun text!
	system("MODE CON COLS=150 LINES=22");
	srand(time(NULL));

	Menu menu(MIN_VALID_CHAR, MAX_VALID_CHAR, MAX_PASSWORD_LENGTH);
	menu.Run();

	return 0;
}