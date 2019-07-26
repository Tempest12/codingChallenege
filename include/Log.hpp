#ifndef _LOG_HPP
#define _LOG_HPP

#include <string>

class PigLatinConverter;

class Log
{
// Static Members:
public:
protected:
private:

// Static functions:
public:

	static void addToQueue(std::string* arg);
	static bool	init(PigLatinConverter* converterRef);
	static void runLoop(void);
	static void stop(void);

protected:
private:

	//static void init(void);
	static std::string* readFromQueue(void);
	static void die(void);
};

#endif