#include "Database.h"
#include <algorithm>

Database::Database()
{
    users = new std::vector<User>;
}

Database::~Database()
{
    users->clear();
    delete users;
}

bool Database::HasUser(const std::string& username) const
{
    return FindUser(username) != nullptr;
}

bool Database::CheckPassword(const std::string& username, const std::string& password) const
{
    const User* user = FindUser(username);
    if (user != nullptr)
    {
        return user->GetPassword() == password;
    }
    return false;
}

void Database::AddUser(const std::string& username, const std::string& password)
{
    if (HasUser(username))
    {
        std::cout << "ERROR - USER WITH USERNAME '" << username << "' ALREADY DEFINED" << std::endl;
        return;
    }

    User user(username, password);
    users->push_back(user);
}

const std::vector<User>& Database::GetAllUsers() const
{
    return *users;
}

const User* Database::FindUser(const std::string& username) const
{
    //find user with same username
    std::vector<User>::const_iterator it =
        std::find_if(users->cbegin(), users->cend(), [&username](const User& user) {return user.GetUserName() == username; });

    if (it != users->end())
    {
        return &(*it);
    }
    return nullptr;
}
