#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_
#include <future>
#include <mutex>
#include <string>
#include <string>
#include <iostream>
#include <dirent.h>
#include "assemblerlame.h"
#include<algorithm>
#include<future>
#include<fstream>
const char identifier[5] = ".wav";
const std::string rep = ".mp3";

struct mp3_file_unit// this is a file container, it has all the essential info to run the file
{
	std::future<void> run_handle;
	assembler* assembly_object;
	int thread_parts;
};
class filehandler
{
public:
	filehandler();
	filehandler(std::string address, int _threads_avaliable);
	~filehandler();
	status thread_handler(); //updates stdout with the info from threads
	void emulate();//runs all .pcm files and puts them in file_containers

	void handle_assemblers(assembler* _file);//the command to run the file_containers
	int get_thread_parts(std::string input);
	

private:
	std::string _input_address;
	std::string _output_address;
	static int _threads_available;
	status _status;
	std::vector<std::string> _input_files;
	//std::vector<mp3_file_unit*> _proc_handle;
	std::vector<assembler*> _proc_db;
	std::condition_variable _condition2;
	MessageQueue <status> _queue2;
	std::mutex _lock;

};


#endif