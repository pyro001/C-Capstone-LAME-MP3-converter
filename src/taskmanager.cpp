#include "taskmanager.h"

#include <string>

filehandler::filehandler()
{
}

filehandler::filehandler(std::string address, int _threads_avaliable)
{
	_status.completed = 0;
	_status.total = 0;
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
				this->_input_files.emplace_back(address +"\\"+ b);
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


status filehandler::thread_handler()
{
	{
		auto futuremsg = std::async(std::launch::async, &MessageQueue<status>::receive, &_queue2);
		status resp = futuremsg.get();
		std::lock_guard<std::mutex> lckgrd(_lock);
		std::cout << "CORE: " << resp.completed << "\t: parts out of:\t" << resp.total << std::endl;
	
	}
	return _status;
}

void filehandler::emulate()
{
	std::vector<std::future<void>>* futures= new std::vector<std::future<void>>;
	std::lock_guard<std::mutex> lckgrd(_lock);
	//mp3_file_unit* temp=(mp3_file_unit*) malloc(sizeof(mp3_file_unit));
	for (auto& i: _input_files )
	{
		std::string pcm_name = i;
		std::string mp3_name = i;
		mp3_name.replace(mp3_name.end() - 4, mp3_name.end(), ".mp3");

		//(temp->assembly_object) = new assembler(pcm_name, wav_name);
		//temp->thread_parts = this->get_thread_parts(pcm_name);
		//courtesy:https://stackoverflow.com/questions/48857679/stdasync-with-class-member-function
		//temp->run_handle = std::move(std::async(std::launch::async, std::bind(&assembler::run, temp->assembly_object)));
		//this->_proc_handle.push_back(temp);
		
		assembler* temp = new assembler(pcm_name, mp3_name);
		_proc_db.push_back(temp);
		//courtesy:https://stackoverflow.com/questions/48857679/stdasync-with-class-member-function
		futures->emplace_back(std::async(std::launch::async, std::bind(&assembler::run,temp )));
		
	}
	for (auto& a: *futures)
	{
		a.get();
	}
	delete (futures);
	int iterator;
	while (true)
	{
		iterator = 0;
		
		this->_status.completed = 0;
		this->_status.total = 0;
		for (auto& i : _proc_db)
		{
			status temp_stat=i->get_state();
			_status.total += temp_stat.total;
			_status.completed += temp_stat.completed;
			auto ftr = std::async(std::launch::async, &MessageQueue<status>::send, &_queue2, std::move(_status));
			if (temp_stat.total != 0 && temp_stat.total == temp_stat.completed)
			{
				iterator++;
			}
		}

		if (iterator == (_proc_db.size()))
		{
			break;
		}
	}
	for (auto& i : _proc_db)
	{   
		delete(i);
	}
	_proc_db.clear();
	std::cout << "end of emulate";

}

int filehandler::get_thread_parts(std::string input)
{
	std::lock_guard<std::mutex> lckgrd(_lock);
	std::ifstream in_file(input, std::ios::binary);
	in_file.seekg(0, std::ios::end);
	int file_size = in_file.tellg();
	//std::cout << "Size of the file is" << " " << file_size << " " << "bytes" << std::endl;
	return ((file_size / WAV_SIZE) / 4 + 1);
}



filehandler::~filehandler()
{
}