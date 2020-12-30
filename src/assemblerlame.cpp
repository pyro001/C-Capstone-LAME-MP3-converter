#include "assemblerlame.h"
#include <cstring>
#include <future>

void assembler::run()
{
	this->setWavformat();
	lameParams _prms = this->get_lame_params(meta);
	int read;
	int order_var = 0;
	std::vector<converted_mp3> writefile;
	std::vector<std::future<converted_mp3>> futures;
	short int buffer[2 * WAV_SIZE];

	std::vector<converter *> _conversion_block_object_db;
	do
	{
		std::cout << "looping though vectors \n";
		read = fread(buffer, 2 * sizeof(short int), WAV_SIZE, _inputfile);

		_total_blocks++;
		order_var++;
		conversion_block temp;
		temp.readlength = read;
		temp.order = order_var;
		std::memcpy(temp.pcmbuffer, buffer, 2 * sizeof(short int) * MP3_SIZE);
		;
		temp._params = _prms;
		//std::cout << temp.readlength;
		this->_converter_data.emplace_back(temp);
		converter *_conv_temp = new converter(temp);
		_conversion_block_object_db.push_back(_conv_temp);
		futures.push_back(std::async(std::launch::async, &(converter::encode_mp3), _conv_temp));

	} while (read != 0);
	//now the whole vector should be full of building block structures which are enough to run encoding on
	//	for(auto &i:_conversion_block_object_db)

	for (auto &iterator_futures : futures)
	{
		//futures.emplace_back(std::async(std::launch::async, &(converter::encode_mp3), _conv_temp, (temp)));

		auto temp_write_conv = iterator_futures.get();

		if (temp_write_conv.write > 0 && temp_write_conv.order > 0)
		{
			writefile.emplace_back(temp_write_conv);
		}
	}
	for (auto &i : _conversion_block_object_db)
		delete (i);
	std::sort(writefile.begin(),writefile.end());
	for(auto &i:writefile)
	{

			std::cout << i.write << "\t::\t" << i.order << std::endl;

			fwrite((i.mp3_buffer), i.write, 1, _opfile);
	}
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

	if (_inputfile)
	{
		fread(meta, 1, sizeof(wavFormat), _inputfile);

		fclose(_inputfile);
	}
	else
	{

		_inputfile = fopen(_input.c_str(), "rb");
		if (_inputfile)
		{
			fread(meta, 1, sizeof(wavFormat), _inputfile);

			fclose(_inputfile);
		}
	}
	_inputfile = fopen(_input.c_str(), "rb");
}

assembler::assembler()
{
	 if (WIN32)
	 {
	 	_input = "E:\\C++\\capstone\\CMakeProject1\\src\\testcase.wav";
	 	this->_inputfile = fopen("E:\\C++\\capstone\\CMakeProject1\\src\\testcase.wav", "rb");
	 	this->_opfile = fopen("E:\\C++\\capstone\\CMakeProject1\\src\\testcase.mp3", "wb");
	 }
	 else
	 {
	_input = "testcase.wav";
	this->_inputfile = fopen("testcase.wav", "rb");
	this->_opfile = fopen("testcase.mp3", "wb");
	}
	_total_blocks = 1;
	_complete_bocks = 0;
	_completion_percentage = 0;
}

assembler::assembler(std::string input, std::string op)
{
	this->_input = input;
	std::cout << "\n CONSTRUCTOR ASSEMBLER|Y\n";
	_total_blocks = 1;
	_complete_bocks = 0;
	_completion_percentage = 0;
	if (!input.empty() && !op.empty())
	{
		this->_inputfile = fopen(input.c_str(), "rb");

		this->_opfile = fopen(op.c_str(), "wb");
	}
}

assembler::~assembler()
{
	std::cout << "\n DESTRUCTOR ASSEMBLER|N\n";
	fclose(_inputfile);
	fclose(_opfile);
}

converter::converter()
{
	std::cout << "\n CONSTRUCTOR ASSEMBLER|N\n";
}

converter::converter(conversion_block input)
{
	std::cout << "\n CONSTRUCTOR ASSEMBLER|Y\n";
	if (sizeof(input.pcmbuffer) != 0 && input.readlength != 0)
	{

		this->_conversion_block.order = input.order;
		std::memcpy(this->_conversion_block.pcmbuffer, input.pcmbuffer, sizeof(short int) * 2 * WAV_SIZE);
		this->_conversion_block._params = input._params;
		this->_conversion_block.readlength = input.readlength;
		this->_converted.order = this->_conversion_block.order;

		converter::set = true;
	}
	else
	{
		std::cout << sizeof(input.pcmbuffer) << std::endl;
	}
}

converter::~converter()
{
	std::cout << "\n DESTRUCTOR block_|N\n";
}

converted_mp3 converter::encode_mp3()
{

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

	std::memcpy(this->_converted.mp3_buffer, mp3_arr, sizeof(unsigned char) * MP3_SIZE);
	this->_converted.write = write;
	return this->_converted;
}
