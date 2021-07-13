#pragma once
#include <iostream>

class User
{
public:
	User(const std::string username, const std::string password);
	const std::string GetUserName() const;
	const std::string GetPassword() const;

	bool operator==(const User& rhs) const;

private:
	std::string username;
	std::string password;
};