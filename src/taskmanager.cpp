#include "taskmanager.h"

#include <string>

filehandler::filehandler()
{
}

filehandler::filehandler(std::string address, int _threads_avaliable)
{
	int i = 0;
	//dirent ref :https://www.bfilipek.com/2019/04/dir-iterate.html
	struct dirent* entry = nullptr;
	DIR* dp = nullptr;

	dp = opendir(address.c_str());
	if (dp != nullptr) {
		while ((entry = readdir(dp)))
		{

			if (strstr(entry->d_name, identifier))
			{
				printf("\nwav Found:\t%s\n", entry->d_name);
				std::string b(entry->d_name);
				this->_input_files.emplace_back(address + b);
				i++;
			}
		}
	}

	closedir(dp);
	if (i)
	{
		this->_input_address = address;
	}
}


void filehandler::thread_handler()
{
}

void filehandler::emulate()
{
	mp3_file_unit* temp;
	for (auto& i : this->_input_files)
	{
		std::string pcm_name = i;
		std::string wav_name;
		i.replace(i.find(".wav"), rep.length(), wav_name);
		(temp->assembly_object) = new assembler(pcm_name, wav_name);
		temp->thread_parts = this->get_thread_parts(pcm_name);
		//courtesy:https://stackoverflow.com/questions/48857679/stdasync-with-class-member-function
		temp->run_handle = std::move(std::async(std::launch::async, std::bind(&assembler::run, temp->assembly_object)));
		this->_proc_handle.push_back(temp);

	}
	for (auto& i : _proc_handle)
	{

		handle_assemblers((i));
	}
}

int filehandler::get_thread_parts(std::string input)
{
	std::ifstream in_file(input, std::ios::binary);
	in_file.seekg(0, std::ios::end);
	int file_size = in_file.tellg();
	//std::cout << "Size of the file is" << " " << file_size << " " << "bytes" << std::endl;
	return ((file_size / WAV_SIZE) / 4 + 1);
}

void filehandler::handle_assemblers(mp3_file_unit* _file)
{
	_file->run_handle.get();


	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		status i = _file->assembly_object->get_state();
		if (i.total != 0 && i.total == i.completed)
			break;

	}
}

filehandler::~filehandler()
{
}