#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_
#include <future>
#include <mutex>
#include <string>
#include "assemblerlame.h"
class filehandler
{
public:
	filehandler();
	filehandler(std::string address);
	void thread_handler(); //updates threads
	void run();//
	~filehandler();

private:
	std::string _input_address;
	static int _threads_available;
};


#endif