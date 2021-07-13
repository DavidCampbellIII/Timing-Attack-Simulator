#include "TimingAttack.h"
#include <iostream>
#include <chrono>

TimingAttack::TimingAttack(const Database& database, const char minASCII, const char maxASCII, const int maxLength) :
	database(database),
	minASCII(minASCII),
	maxASCII(maxASCII),
	maxLength(maxLength)
{
	//purposely empty...
}

void TimingAttack::StartTimingAttack(const std::string& username)
{
	if (!database.HasUser(username))
	{
		std::cout << "ERROR - NO USER WITH THE USERNAME '" << username << "' WAS FOUND!" << std::endl;
		return;
	}

	PrintStartMessage();
	auto startTime = std::chrono::high_resolution_clock::now();

	int length = CrackLength(username);
	std::cout << username << "'s password most likely is " << length << " characters long..." << std::endl;
	std::string result = CrackPassword(length);

	auto endTime = std::chrono::high_resolution_clock::now();
	auto crackDuration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);

	PrintSucessMessage();
	std::cout << username << "'s password is " << result << std::endl;
	std::cout << "Password cracked in " << crackDuration.count() << " seconds" << std::endl;
}

int TimingAttack::CrackLength(const std::string username) const
{
	int mostLikely = -1;
	long long longestTime = std::numeric_limits<int>::min();
	for (int i = 1; i < maxLength; i++)
	{
		int trials = 1000;
		long long fastestTrialTime = std::numeric_limits<int>::max();
		for (int j = 0; j < trials; j++)
		{
			long long time = LengthTrial(username, i);
			if (time < fastestTrialTime)
			{
				fastestTrialTime = time;
			}
		}

		if (fastestTrialTime > longestTime)
		{
			longestTime = fastestTrialTime;
			mostLikely = i;
		}
	}
	return mostLikely;
}

long long TimingAttack::LengthTrial(const std::string username, const int length) const
{
	int passes = 100;
	long long sum = 0;
	for (int i = 0; i < passes; i++)
	{
		std::string randomStr = GenerateRandomString(length);
		auto startTime = std::chrono::high_resolution_clock::now();
		database.CheckPassword(username, randomStr);
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
		sum += duration.count();
	}
	return sum / passes;
}

const std::string TimingAttack::GenerateRandomString(const int length) const
{
	std::string result = "";
	for (int i = 0; i < length; i++)
	{
		char randomChar = rand() % (maxASCII - minASCII + 1) + minASCII;
		result += randomChar;
	}
	return result;
}

const std::string TimingAttack::CrackPassword(const int length) const
{
	return std::string();
}

void TimingAttack::PrintStartMessage() const
{
	std::cout	<< "  ____________  ________   ________      ___  _______________   ________ __       _____   ___________________  ________________ " << std::endl
				<< " /_  __/  _/  |/  /  _/ | / / ____/     /   |/_  __/_  __/   | / ____/ //_/      /  _/ | / /  _/_  __/  _/   |/_  __/ ____/ __ \\" << std::endl
				<< "  / /  / // /|_/ // //  |/ / / __      / /| | / /   / / / /| |/ /   / ,<         / //  |/ // /  / /  / // /| | / / / __/ / / / /" << std::endl
				<< " / / _/ // /  / _/ // /|  / /_/ /     / ___ |/ /   / / / ___ / /___/ /| |      _/ // /|  _/ /  / / _/ // ___ |/ / / /___/ /_/ / " << std::endl
				<< "/_/ /___/_/  /_/___/_/ |_/\\____/     /_/  |_/_/   /_/ /_/  |_\\____/_/ |_|     /___/_/ |_/___/ /_/ /___/_/  |_/_/ /_____/_____/  " << std::endl << std::endl;
}

void TimingAttack::PrintSucessMessage() const
{
	std::cout	<< " /$$$$$$$  /$$$$$$  /$$$$$$  /$$$$$$ /$$      /$$ /$$$$$$ /$$$$$$$ /$$$$$$$         /$$$$$$ /$$$$$$$  /$$$$$$  /$$$$$$ /$$   /$$/$$$$$$$$/$$$$$$$ " << std::endl
				<< "| $$__  $$/$$__  $$/$$__  $$/$$__  $| $$  /$ | $$/$$__  $| $$__  $| $$__  $$       /$$__  $| $$__  $$/$$__  $$/$$__  $| $$  /$$| $$_____| $$__  $$" << std::endl
				<< "| $$  \\ $| $$  \\ $| $$  \\__| $$  \\__| $$ /$$$| $| $$  \\ $| $$  \\ $| $$  \\ $$      | $$  \\__| $$  \\ $| $$  \\ $| $$  \\__| $$ /$$/| $$     | $$  \\ $$" << std::endl
				<< "| $$$$$$$| $$$$$$$|  $$$$$$|  $$$$$$| $$/$$ $$ $| $$  | $| $$$$$$$| $$  | $$      | $$     | $$$$$$$| $$$$$$$| $$     | $$$$$/ | $$$$$  | $$  | $$" << std::endl
				<< "| $$____/| $$__  $$\\____  $$\\____  $| $$$$_  $$$| $$  | $| $$__  $| $$  | $$      | $$     | $$__  $| $$__  $| $$     | $$  $$ | $$__/  | $$  | $$" << std::endl
				<< "| $$     | $$  | $$/$$  \\ $$/$$  \\ $| $$$/ \\  $$| $$  | $| $$  \\ $| $$  | $$      | $$    $| $$  \\ $| $$  | $| $$    $| $$\\  $$| $$     | $$  | $$" << std::endl
				<< "| $$     | $$  | $|  $$$$$$|  $$$$$$| $$/   \\  $|  $$$$$$| $$  | $| $$$$$$$/      |  $$$$$$| $$  | $| $$  | $|  $$$$$$| $$ \\  $| $$$$$$$| $$$$$$$/" << std::endl
				<< "|__/     |__/  |__/\\______/ \\______/|__/     \\__/\\______/|__/  |__|_______/        \\______/|__/  |__|__/  |__/\\______/|__/  \\__|________|_______/ " << std::endl << std::endl;
}
