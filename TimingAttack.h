#pragma once
#include "Database.h"

class TimingAttack
{
public:
	TimingAttack(const Database& database, const char minASCII, const char maxASCII, const int maxLength);
	void StartTimingAttack(const std::string& username);

private:
	const Database& database;
	//min and max ASCII values of allowed characters
	const char minASCII;
	const char maxASCII;
	const int maxLength;

	int CrackLength(const std::string& username) const;
	long long LengthTrial(const std::string& username, const int length) const;
	const std::string GenerateRandomString(const int length) const;
	const std::string CrackPassword(const std::string& username, const int length) const;
	long long PasswordTrial(const User* user, const std::string& guess) const;
	void PrintStartMessage() const;
	void PrintSucessMessage() const;
};