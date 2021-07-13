#pragma once
#include <iostream>
#include <vector>
#include "User.h"

class Database
{
public:
	Database();
	virtual ~Database();
	const User* FindUser(const std::string& username) const;
	bool HasUser(const std::string& username) const;
	bool CheckPassword(const std::string& username, const std::string& password) const;
	bool CheckPassword(const User* user, const std::string& password) const;
	void AddUser(const std::string& username, const std::string& password);
	const std::vector<User>& GetAllUsers() const;

private:
	std::vector<User>* users;

	bool CompareStrings(const std::string& s1, const std::string& s2) const;
};