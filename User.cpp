#include "User.h"

User::User(const std::string username, const std::string password) :
    username(username),
    password(password)
{
    //purposely empty...
}

const std::string User::GetUserName() const
{
    return username;
}

const std::string User::GetPassword() const
{
    return password;
}

bool User::operator==(const User& rhs) const
{
    //no need to compare password, as we will not allow two users with the same username in the database,
    //therefore username is enough to make the distinction
    return username == rhs.username;
}
