#include "assemblerlame.h"

#include <future>

void assembler::run()
{
	lameParams _prms = this->get_lame_params(meta);
	int read;
	int order_var = 0;
	std::vector<converted_mp3> writefile;
	std::vector<std::future<converted_mp3>> futures;
	std::vector<short int> buffer(2 * WAV_SIZE);

	std::vector<converter> _conversion_block_object_db;
	do
	{
		std::cout << "looping though vectors \n";
		read = fread(&buffer[0], 2 * sizeof(short int), WAV_SIZE, _inputfile);

		_total_blocks++;
		order_var++;
		conversion_block temp;
		temp.readlength = read;
		temp.order = order_var;
		temp.pcmbuffer = buffer;
		temp._params = _prms;
		//std::cout << temp.readlength;
		this->_converter_data.emplace_back(temp);
		converter* _conv_temp = new converter(temp);
		_conv_temp->encode_mp3();
		//_conversion_block_object_db.emplace_back((_conv_temp));
		futures.emplace_back(std::async(std::launch::async, &(converter::encode_mp3), _conv_temp));

	} while (read != 0);
	//now the whole vector should be full of building block structures which are enough to run encoding on
//	for(auto &i:_conversion_block_object_db)
	

	while (!futures.empty())
	{
		//futures.emplace_back(std::async(std::launch::async, &(converter::encode_mp3), _conv_temp, (temp)));
		auto& a = futures.begin();
		a->wait();
		auto temp_write_conv = a->get();
		if (temp_write_conv.write > 0 && temp_write_conv.order > 0)
		{
			writefile.emplace_back(temp_write_conv);
			futures.erase(futures.begin());
			std::cout << sizeof(temp_write_conv.mp3_buffer) << "\t::\t" << temp_write_conv.write << std::endl;

			fwrite((temp_write_conv.mp3_buffer), temp_write_conv.write, 1, _opfile);
		}
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
	}
}

assembler::assembler()
{
	this->_inputfile = fopen("E:\\C++\\capstone\\CMakeProject1\\src\\testcase.wav", "rb");
	this->_opfile = fopen("E:\\C++\\capstone\\CMakeProject1\\src\\testcase.mp3", "wb");
	_total_blocks = 1;
	_complete_bocks = 0;
	_completion_percentage = 0;
}


assembler::assembler(std::string input, std::string op)
{
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
	if (!input.pcmbuffer.empty() && input.readlength != 0)
	{

		this->_conversion_block.order = input.order;
		this->_conversion_block.pcmbuffer = input.pcmbuffer;
		this->_conversion_block._params = input._params;
		this->_conversion_block.readlength = input.readlength;
		this->_converted.order = this->_conversion_block.order;

		converter::set = true;
	}
	else
	{
		std::cout << input.pcmbuffer.size() << std::endl;
	}
}

converter::~converter()
{
	std::cout << "\n DESTRUCTOR block_|N\n";
}

converted_mp3 converter::encode_mp3()
{
	
	lame_t lame = lame_init();
	lame_set_in_samplerate(lame, 11000);
	lame_set_VBR(lame, vbr_default);
	lame_init_params(lame);
	int write;
	lame_set_VBR_q(lame, 0);
	int a = this->_conversion_block.pcmbuffer.size();
	//std::cout << "\t:\t" << input.pcmbuffer.size() << "\t:\t" << input.readlength;
	unsigned char mp3_arr[MP3_SIZE];
	short int pcmarr[WAV_SIZE * 2];
	std::copy(this->_conversion_block.pcmbuffer.begin(), this->_conversion_block.pcmbuffer.end(), pcmarr);
	int read = this->_conversion_block.readlength;
	if (this->_conversion_block.readlength == 0)

	{
		write = lame_encode_flush(lame, mp3_arr, MP3_SIZE);

	}
	else
	{
		write = lame_encode_buffer_interleaved(lame, pcmarr,read, mp3_arr, MP3_SIZE);
	}
//	mp3_buffer.insert(mp3_buffer.end(), &mp3_arr[0], &mp3_arr[MP3_SIZE]);

	std::memcpy(this->_converted.mp3_buffer,mp3_arr, sizeof(unsigned char) *MP3_SIZE);
	this->_converted.write = write;
	return this->_converted;
}



