#include "Menu.h"

Menu::Menu(const char minValidChar, const char maxValidChar, const int maxPasswordLength) :
	MIN_VALID_CHAR(minValidChar),
	MAX_VALID_CHAR(maxValidChar),
	MAX_PASSWORD_LENGTH(maxPasswordLength),
	database(new Database())
{
	//purposely empty...
}

Menu::~Menu()
{
	delete database;
}

void Menu::Run()
{
	bool isRunning = true;
	while (isRunning)
	{
		PrintMainMenu();
		int choice;
		std::cin >> choice;
		ProgramState state = ExecuteMenuChoice(choice);
		if (state == ProgramState::QUIT_PROGRAM)
		{
			isRunning = false;
		}
		else if (state == ProgramState::INVALID_MENU_CHOICE)
		{
			std::cout << choice << " is not a valid option!" << std::endl;
		}
	}
}

const ProgramState Menu::ExecuteMenuChoice(const int choice)
{
	switch (choice)
	{
	case 1:
		AddUser();
		break;
	case 2:
		PrintAllUsers();
		break;
	case 3:
		CrackPassword();
		break;
	case 4:
		std::cout << "Goodbye!" << std::endl;
		return ProgramState::QUIT_PROGRAM;
	default:
		return ProgramState::INVALID_MENU_CHOICE;
	}
	return ProgramState::VALID_MENU_CHOICE;
}

void Menu::PrintMainMenu() const
{
	std::cout << "1. Add User" << std::endl
		<< "2. See All Users" << std::endl
		<< "3. Crack Password" << std::endl
		<< "4. Quit" << std::endl;
}

const std::string Menu::GetUsername() const
{
	std::cout << "What is the username of the new user?" << std::endl;

	std::string username;
	std::cin.ignore();
	std::getline(std::cin, username);

	while (database->HasUser(username))
	{
		std::cout << "Username '" << username << "' is already taken!  Please pick a different username: " << std::endl;
		std::getline(std::cin, username);
	}
	return username;
}

const std::string Menu::GetPassword() const
{
	std::cout << "What is the password of the new user?" << std::endl;

	std::string password;
	std::getline(std::cin, password);
	while (!IsValidPassword(password))
	{
		std::cout << "Please choose a different password: " << std::endl;
		std::getline(std::cin, password);
	}
	return password;
}

const bool Menu::IsValidPassword(const std::string& password) const
{
	if (password.length() > MAX_PASSWORD_LENGTH)
	{
		std::cout << "Password cannot be longer than " << MAX_PASSWORD_LENGTH << " characters!" << std::endl;
		return false;
	}

	if (password.length() == 0)
	{
		std::cout << "Password cannot be empty!" << std::endl;
		return false;
	}

	for (int i = 0; i < password.length(); i++)
	{
		char currentChar = password.at(i);
		if (currentChar < MIN_VALID_CHAR || currentChar > MAX_VALID_CHAR)
		{
			std::cout << "Password contains illegal character '" << currentChar << "'." << std::endl;
			return false;
		}
	}
	return true;
}

void Menu::AddUser()
{
	const std::string username = GetUsername();
	const std::string password = GetPassword();
	database->AddUser(username, password);
}

void Menu::PrintAllUsers() const
{
	std::vector<User> users = database->GetAllUsers();
	std::cout << "ALL KNOWN USERS (" << users.size() << ")" << std::endl;
	for (User user : users)
	{
		std::cout << user.GetUserName() << "\t" << user.GetPassword() << std::endl;
	}
}

void Menu::CrackPassword() const
{
	std::cout << "What is the username of the user whose password you'd like to crack?" << std::endl;
	std::string username;
	std::cin.ignore();
	getline(std::cin, username);

	TimingAttack timingAttack(*database, MIN_VALID_CHAR, MAX_VALID_CHAR, MAX_PASSWORD_LENGTH);
	timingAttack.StartTimingAttack(username);
}
