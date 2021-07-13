#include <iostream>
#include <string>
#include <cstdlib>
#include "Database.h"
#include "TimingAttack.h"

enum class ProgramState
{
	VALID_MENU_CHOICE, INVALID_MENU_CHOICE, QUIT_PROGRAM
};

void AddUser(Database& database)
{
	std::cout << "What is the username of the new user?" << std::endl;
	std::string username;
	std::cin.ignore();
	std::getline(std::cin, username);

	std::cout << "What is the password of the new user?" << std::endl;
	std::string password;
	std::cin.ignore();
	std::getline(std::cin, password);

	database.AddUser(username, password);
}

void PrintAllUsers(const Database& database)
{
	std::vector<User> users = database.GetAllUsers();
	std::cout << "ALL KNOWN USERS (" << users.size() << ")" << std::endl;
	for (User user : users)
	{
		std::cout << user.GetUserName() << std::endl;
	}
}

void CrackPassword(const Database& database)
{
	std::cout << "What is the username of the user whose password you'd like to crack?" << std::endl;
	std::string username;
	std::cin.ignore();
	getline(std::cin, username);

	TimingAttack timingAttack(database, ' ', '~', 32);
	timingAttack.StartTimingAttack(username);
}

void PrintMainMenu()
{
	std::cout << "1. Add User" << std::endl
		<< "2. See All Users" << std::endl
		<< "3. Crack Password" << std::endl
		<< "4. Quit" << std::endl;
}

const ProgramState ExecuteMenuChoice(Database& database, const int choice)
{
	switch (choice)
	{
		case 1:
			AddUser(database);
			break;
		case 2:
			PrintAllUsers(database);
			break;
		case 3:
			CrackPassword(database);
			break;
		case 4:
			std::cout << "Goodbye!" << std::endl;
			return ProgramState::QUIT_PROGRAM;
		default:
			return ProgramState::INVALID_MENU_CHOICE;
	}
	return ProgramState::VALID_MENU_CHOICE;
}

int main()
{
	system("MODE CON COLS=150 LINES=22");

	Database database;

	bool isRunning = true;
	while (isRunning)
	{
		PrintMainMenu();
		int choice;
		std::cin >> choice;
		ProgramState state = ExecuteMenuChoice(database, choice);
		if(state == ProgramState::QUIT_PROGRAM)
		{
			isRunning = false;
		}
		else if (state == ProgramState::INVALID_MENU_CHOICE)
		{
			std::cout << choice << " is not a valid option!" << std::endl;
		}
	}

	return 0;
}