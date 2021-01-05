#include "assemblerlame.h"
#include<fstream>
#include <errno.h>
template<typename R>
bool is_ready(std::future<R> const& f)
{ //courtesy:https://stackoverflow.com/questions/10890242/get-the-status-of-a-stdfuture
	return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

template <typename T>
T MessageQueue<T>::receive()
{

	std::unique_lock<std::mutex> uLock(_commlock);
	_condition.wait(uLock, [this] {return(!_queue.empty()); });
	T retval = std::move(_queue.back());//front()

	_queue.clear();//popfront()
	return retval;
	//The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
	// to wait for and receive new messages and pull them from the queue using move semantics. 
	// The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T&& msg)
{
	std::lock_guard<std::mutex> lckgrd(_commlock);
	_queue.emplace_back(msg);
	_condition.notify_one();
	//The method send should use the mechanisms std::lock_guard<std::mutex> 
	// as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}


void assembler::run()// uses 73952 bytes of stack needs heap memory movement
{
	std::lock_guard<std::mutex> lckgrd(lck);

	std::thread::id this_id = std::this_thread::get_id();

	//std::cout << "\n thread::assembler block " << this_id << "\n";

	if (this->_inputfile)
	{
		

		this->setWavformat();
		lameParams _prms = this->get_lame_params(meta);
		int read = 1;
		std::vector<converted_mp3>* writefile = new std::vector<converted_mp3>;
		std::vector<std::future<converted_mp3>>* futures = new std::vector<std::future<converted_mp3>>;


		std::vector<converter*>* _conversion_block_object_db = new std::vector<converter*>;
		while (read != 0)
		{
			short int buffer[2 * WAV_SIZE];
			//std::cout << "looping though vectors \n";
			read = fread(buffer, 2 * sizeof(short int), WAV_SIZE, this->_inputfile);
			this->_status.total++;

			conversion_block temp;
			temp.readlength = std::move(read);
			temp.order = std::move(this->_status.total);
			std::memcpy(temp.pcmbuffer, std::move(buffer), 2 * sizeof(short int) * MP3_SIZE);

			temp._params = std::move(_prms);
			//std::cout << temp.readlength;
			converter* _conv_temp = new converter(temp);
			//courtesy:https://stackoverflow.com/questions/48857679/stdasync-with-class-member-function
			futures->push_back(std::async(std::launch::async, std::bind(&converter::encode_mp3, _conv_temp)));
			_conversion_block_object_db->push_back(std::move(_conv_temp));
			auto ftr = std::async(std::launch::async, &MessageQueue<status>::send, &_queue, std::move(_status));

		}
		//now the whole vector should be full of building block structures which are enough to run encoding on
		int x=0;
		while (true)
		{
			 x = 0;

			for (auto& iterator_futures : *futures)
			{

				//futures.emplace_back(std::async(std::launch::async, &(converter::encode_mp3), _conv_temp, (temp)));
				if (is_ready(iterator_futures))

					x++;
			}
			if (x == futures->size())
			{
				break;

			}

		}
		for (auto& iterator_futures : *futures)
		{

			//futures.emplace_back(std::async(std::launch::async, &(converter::encode_mp3), _conv_temp, (temp)));

			auto temp_write_conv = std::move(iterator_futures.get());

			if (temp_write_conv.write > 0 && temp_write_conv.order > 0)
			{
				writefile->emplace_back(std::move(temp_write_conv));
			}

		}
		delete(futures);
		for (auto& i : *_conversion_block_object_db)
			delete (i);
		delete(_conversion_block_object_db);
		this->_status.completed++;
		std::sort(writefile->begin(), writefile->end());
		for (auto& i : *writefile)
		{
			this->_status.completed++;
			//std::cout << i.write << "\t::\t" << i.order << std::endl;

			fwrite(std::move(i.mp3_buffer), std::move(i.write), 1, std::move(_opfile));

			auto ftr = std::async(std::launch::async, &MessageQueue<status>::send, &_queue, std::move(_status));

		}
		delete(writefile);
	}

}

status assembler::get_state()
{
	auto futuremsg = std::async(std::launch::async, &MessageQueue<status>::receive, &_queue);
	status resp = futuremsg.get();
	std::lock_guard<std::mutex> lckgrd(lck);

	std::cout << "\nCompleted file: " + _input <<"\t"<< resp.completed << " :parts out of: " << resp.total << std::endl;
	return status(resp);
}

void assembler::reset_mp3()
{
	typedef std::vector<unsigned char> mp3data;
	mp3data bare = mp3data(MP3_SIZE, '\0');
	this->_mp3Construct.clear();
}

lameParams assembler::get_lame_params(_wavfmt input)
{
	lameParams op;
	op.bit_rate = int(input->byte_rate);
	op.samplerate = int(input->sample_rate);
	if (input->num_channels == 1)
		op.channel = MPEG_mode::MONO;
	else if (input->num_channels == 2)
		op.channel = MPEG_mode::STEREO;
	else
	{
		op.channel = MPEG_mode::NOT_SET;
	}
	op.vbr_q = 0;

	return op;
}

void assembler::setWavformat()
{

	if (this->_inputfile)
	{
		size_t a = fread(meta, 1, sizeof(wavFormat), this->_inputfile);

		fclose(this->_inputfile);
	}
	else
	{

		this->_inputfile = fopen(_input.c_str(), "rb");
		if (this->_inputfile)
		{
			size_t a = fread(meta, 1, sizeof(wavFormat), this->_inputfile);

			fclose(this->_inputfile);
		}
	}
	this->_inputfile = fopen(_input.c_str(), "rb");
}

assembler::assembler()
{
	_total_blocks = 0;
	_complete_bocks = 0;
	this->_status.completed = 0;
	this->_status.total = 0;
	std::ifstream in_file(_input, std::ios::binary);
	in_file.seekg(0, std::ios::end);
	int file_size = in_file.tellg();
	//std::cout << "Size of the file is" << " " << file_size << " " << "bytes" << std::endl;
}

assembler::assembler(std::string input, std::string op)
{
	if (!input.empty() && !op.empty())
	{
		this->_input = input;
		this->_inputfile = fopen(input.c_str(), "rb");

		this->_opfile = fopen(op.c_str(), "wb");
	}


	if (!this->_inputfile)
	{
		printf("\n\nError %d \n", errno);
		printf("It's null");
		perror("something broke, inputfile is null");
	}

	_total_blocks = 0;
	_complete_bocks = 0;
	this->_status.completed = 0;
	this->_status.total = 0;

	//std::cout << "Size of the file is" << " " << file_size << " " << "bytes" << std::endl;

}

assembler::~assembler()
{
	
	fclose(this->_inputfile);
	fclose(_opfile);
}

converter::converter()
{
	//std::lock_guard<std::mutex> lckgrd(lck);
	//std::cout << "\n CONSTRUCTOR ASSEMBLER|N\n";
}

converter::converter(conversion_block input)
{
	if (sizeof(input.pcmbuffer) != 0 && input.readlength != 0)
	{

		this->_conversion_block.order = input.order;
		std::memcpy(this->_conversion_block.pcmbuffer, input.pcmbuffer, sizeof(short int) * 2 * WAV_SIZE);
		this->_conversion_block._params = input._params;
		this->_conversion_block.readlength = input.readlength;
		this->_converted.order = this->_conversion_block.order;

		converter::set = true;
		//std::lock_guard<std::mutex> lckgrd(lck);

		//std::cout << "\n CONSTRUCTOR ASSEMBLER|Y\n";
	}
	else
	{
		///	std::cout << sizeof(input.pcmbuffer) << std::endl;
	}
}

converter::~converter()
{
	//std::lock_guard<std::mutex> lckgrd(lck);
	//std::cout << "\n DESTRUCTOR block_ Converter|N\n";
}

converted_mp3 converter::encode_mp3()
{
	std::thread::id this_id = std::this_thread::get_id();
	//lck.lock();
	//std::cout << "\nthread::encoding MP3 " << this_id << "\n";
	//lck.unlock();
	std::lock_guard<std::mutex> lckgrd(lck);
	lame_t lame = lame_init();
	lame_set_in_samplerate(lame, (this->_conversion_block._params.samplerate) / 2);
	lame_set_VBR(lame, vbr_default);
	int write;
	lame_set_VBR_q(lame, 0);
	lame_set_mode(lame, this->_conversion_block._params.channel);
	lame_init_params(lame);

	//std::cout << "\t:\t" << input.pcmbuffer.size() << "\t:\t" << input.readlength;
	unsigned char mp3_arr[MP3_SIZE];
	int read = this->_conversion_block.readlength;
	if (this->_conversion_block.readlength == 0)

	{
		write = lame_encode_flush(lame, mp3_arr, MP3_SIZE);
	}
	else
	{
		write = lame_encode_buffer_interleaved(lame, this->_conversion_block.pcmbuffer, read, mp3_arr, MP3_SIZE);
	}
	//	mp3_buffer.insert(mp3_buffer.end(), &mp3_arr[0], &mp3_arr[MP3_SIZE]);
	/**return code     number of bytes output in mp3buf.Can be 0
		* -1:  mp3buf was too small
		* -2 : malloc() problem
		* -3 : lame_init_params() not called
		* -4 : psycho acoustic problems*/

	std::memcpy(this->_converted.mp3_buffer, mp3_arr, sizeof(unsigned char) * MP3_SIZE);
	this->_converted.write = write;
	return this->_converted;
}
