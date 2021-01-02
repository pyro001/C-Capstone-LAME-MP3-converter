#include <stdio.h>
#include <lame/lame.h>
#include <thread>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <future>
#include <mutex>
#include "assemblerlame.h"

int main(void)
{
	bool done = false;
	assembler a;
	a.run();

	while (!done)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		status i= a.get_state();
		if (i.total != 0 && i.total == i.completed)
			break;

	}




	return (0);
	}
