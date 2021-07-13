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

	std::cout << "Cracking password length..." << std::endl;
	int length = CrackLength(username);
	std::cout << username << "'s password most likely is " << length << " characters long..." << std::endl;

	std::cout << "Cracking password..." << std::endl;
	std::string result = CrackPassword(username, length);

	auto endTime = std::chrono::high_resolution_clock::now();
	auto crackDuration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);

	PrintSucessMessage();
	std::cout << username << "'s password is " << result << std::endl;
	std::cout << "Password cracked in " << crackDuration.count() << " seconds" << std::endl;
}

int TimingAttack::CrackLength(const std::string& username) const
{
	int mostLikely = -1;
	long long longestTime = std::numeric_limits<int>::min();
	for (int i = 1; i <= maxLength; i++)
	{
		int trials = 10;
		long long fastestTrialTime = std::numeric_limits<int>::max();
		for (int j = 0; j < trials; j++)
		{
			long long time = LengthTrial(username, i);
			//std::cout << "Length " << i << ": Trial #" << j << " Avg Time: " << time << std::endl;
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

long long TimingAttack::LengthTrial(const std::string& username, const int length) const
{
	int passes = 1;
	long long sum = 0;
	for (int i = 0; i < passes; i++)
	{
		std::string randomStr = GenerateRandomString(length);
		auto startTime = std::chrono::high_resolution_clock::now();
		database.CheckPassword(username, randomStr);
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
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

const std::string TimingAttack::CrackPassword(const std::string& username, const int length) const
{
	int trials = 10;
	int currentCharIndex = 0;
	int totalPossibleChars = maxASCII - minASCII + 1;
	//start with random guess
	std::string guess = GenerateRandomString(length);
	const User* user = database.FindUser(username);
	while (true)
	{
		for (int i = 0; i < totalPossibleChars; i++)
		{
			//try out a new letter
			char guessChar = minASCII + i;
			//insert the new letter in the position we are currently guessing with our guess so far
			std::string altGuess = guess.substr(0, currentCharIndex) + guessChar + guess.substr(currentCharIndex + 1);

			//find the fastest averaged time for the new guess
			long long fastestAltTime = std::numeric_limits<int>::max();
			for (int j = 0; j < trials; j++)
			{
				long long time = PasswordTrial(username, altGuess);
				if (time < fastestAltTime)
				{
					fastestAltTime = time;
				}
			}

			//find fastest averaged time for the current guess
			long long fastestGuessTime = std::numeric_limits<int>::max();
			for (int j = 0; j < trials; j++)
			{
				long long time = PasswordTrial(username, guess);
				if (time < fastestGuessTime)
				{
					fastestGuessTime = time;
				}
			}

			//check if we guessed correctly
			if (database.CheckPassword(user, altGuess))
			{
				return altGuess;
			}
			//if not guessed correctly, update current guess to be this alt guess if it is slower, meaning
			//we are getting closer to the actual password...
			if (fastestAltTime > fastestGuessTime)
			{
				guess = altGuess;
				std::cout << guess << std::endl;
			}
		}
		currentCharIndex++;
		//if we reach the end of the length but haven't found the correct password yet, loop back around to the front
		currentCharIndex %= length;
	}
	return "PASSWORD NOT FOUND :(";
}

long long TimingAttack::PasswordTrial(const std::string& username, const std::string& guess) const
{
	int passes = 1;
	long long sum = 0;
	for (int i = 0; i < passes; i++)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		database.CheckPassword(username, guess);
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
		sum += duration.count();
	}
	return sum;
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
