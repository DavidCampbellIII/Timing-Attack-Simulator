#pragma once
#include <iostream>
#include <vector>
#include "User.h"

class Database
{
public:
	Database();
	virtual ~Database();
	bool HasUser(const std::string& username) const;
	bool CheckPassword(const std::string& username, const std::string& password) const;
	void AddUser(const std::string& username, const std::string& password);
	const std::vector<User>& GetAllUsers() const;

private:
	std::vector<User>* users;

	const User* FindUser(const std::string& username) const;
};