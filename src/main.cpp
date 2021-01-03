#include <stdio.h>
#include <lame/lame.h>
#include <thread>
#include <vector>
#include <iostream>
#include <future>
#include <mutex>
#include "assemblerlame.h"
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

	
	bool done = false;
	assembler arg123("E:\\C++\\capstone\\CMakeProject1\\src\\testcase.wav","E:\\C++\\capstone\\CMakeProject1\\src\\testcase.mp3");
	arg123.run();
	
	
	while (!done)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		status i= arg123.get_state();
		if (i.total != 0 && i.total == i.completed)
			break;

	}




	return (0);
	}
