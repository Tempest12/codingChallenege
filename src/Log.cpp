#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>

#include "Log.hpp"

#include "PigLatinConverter.h"

static std::queue<std::string*>* futureQueue = NULL;
static std::mutex*               queueMutex  = NULL;
static std::condition_variable*  wakeUp      = NULL;
static std::ofstream*            logFile     = NULL;
static PigLatinConverter*        converter   = NULL;

static bool live = true;

void Log::addToQueue(std::string* arg)
{
	std::unique_lock<std::mutex> qLock = std::unique_lock<std::mutex>(*queueMutex);

	futureQueue->push(arg);
	wakeUp->notify_all();

	qLock.unlock();
}

bool Log::init(PigLatinConverter* converterRef)
{
	futureQueue = new std::queue<std::string*>();

	queueMutex = new std::mutex();

	wakeUp = new std::condition_variable();

	logFile = new std::ofstream("log.txt", std::ofstream::out | std::ofstream::trunc);

	converter = converterRef;

	return logFile->is_open() && logFile->good();
}

void Log::runLoop(void)
{
	while(live)
	{
		std::string* temp = readFromQueue();

		if(temp == NULL)
		{
			continue;
		}

		std::string pigLatin = converter->convert(*temp);

		if(logFile->good() == true)
		{
			*logFile << pigLatin << std::endl;
		}

		delete temp;
	}

	die();
}

void Log::stop(void)
{
	live = false;
	wakeUp->notify_all();
}

// Private functions:
std::string* Log::readFromQueue(void)
{
	std::string* answer = NULL;

	std::unique_lock<std::mutex> qLock = std::unique_lock<std::mutex>(*queueMutex);

	if(futureQueue->empty() == true)
	{
		wakeUp->wait(qLock);
	}
	else
	{
		answer = futureQueue->front();
		futureQueue->pop();
	}

	qLock.unlock();

	return answer;
}

void Log::die(void)
{
	logFile->close();

	// Get rid of any of the messages we didn't get to.
	while(futureQueue->empty() == false)
	{
		std::string* temp = futureQueue->front();
		futureQueue->pop();
		delete temp;
	}

	delete logFile;
	delete queueMutex;
	delete futureQueue;
	delete wakeUp;
}