#include <stdio.h>
#include <lame/lame.h>
#include <thread>
#include <vector>
#include <iostream>
#include <future>
#include <mutex>
#include "assemblerlame.h"
#include "taskmanager.h"
#include <dirent.h>
int main(int argc, char** argv)
{

	std::string input_path;
	if (argc <= 1)
	{
		std::cout << "path not entered on commande line add path now::\n and hit return ";
		std::cin >> input_path;
		std::cout << input_path;
		
	}
	else {
		 std::string a (argv[1]);
		 input_path = a;
	}
	size_t const hardware_concurrency = std::thread::hardware_concurrency();
	std::cout << "MAX concurrent logical cores::" << hardware_concurrency;
	
	bool done = false;
	filehandler arg123(input_path,hardware_concurrency);
	arg123.emulate();
	
	
	while (!done)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		status i= arg123.thread_handler();
		if (i.total != 0 && i.total == i.completed)
			break;

	}




	return (0);
	}
