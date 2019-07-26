#include <algorithm>
#include <iostream>
#include <string>
#include <thread>

#include "Driver.hpp"
#include "Log.hpp"
#include "PigLatinConverter.h"

#include <ctype.h>

PigLatinConverter* converter = NULL;

void Driver::runLoop(void)
{
	bool run = true;
	std::string input = "";
	std::thread logThread;

	converter = new PigLatinConverter();
	
	if(Log::init(converter) == false)
	{
		std::cout << "Failed to open log file or allocate the required resources." << std::endl;
		return;
	}

	logThread = std::thread(Log::runLoop);

	while(run == true)
	{
		std::getline(std::cin, input);

		run = keepRunning(input);

		if(run == true)
		{
			std::string* arg = new std::string(input);

			Log::addToQueue(arg);
		}
	}

	Log::stop();
	converter->cancel();
	logThread.join();

	delete converter;
}

bool Driver::keepRunning(std::string& input)
{
	std::string temp = std::string(input);

	std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

	return temp.compare("q")    != 0 &&
	       temp.compare("exit") != 0 &&
	       temp.compare("quit") != 0;
}