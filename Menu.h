#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include "Database.h"
#include "TimingAttack.h"

enum class ProgramState
{
	VALID_MENU_CHOICE, INVALID_MENU_CHOICE, QUIT_PROGRAM
};

class Menu
{
public:
	Menu (const char minValidChar, const char maxValidChar, const int maxPasswordLength);
	~Menu();
	void Run();

private:
	const char MIN_VALID_CHAR;
	const char MAX_VALID_CHAR;
	const int MAX_PASSWORD_LENGTH;

	Database* database;

	const ProgramState ExecuteMenuChoice(const int choice);
	void PrintMainMenu() const;
	const std::string GetUsername() const;
	const std::string GetPassword() const;
	const bool IsValidPassword(const std::string& password) const;
	void AddUser();
	void PrintAllUsers() const;
	void CrackPassword() const;
};