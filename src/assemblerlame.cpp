#include "assemblerlame.h"

#include <future>

void assembler::run()
{ lameParams _prms= this->get_lame_params(meta);
	int read;
	int order_var = 0;
	do
	{
		std::vector<short int> buffer(2*WAV_SIZE);
		read = fread(&buffer[0], 2 * sizeof(short int), WAV_SIZE, _inputfile);
	
		_total_blocks++;
		order_var++;
		conversion_block temp;
		temp.readlength=read;
		temp.order = order_var;
		temp.pcmbuffer = buffer;
		temp._params = _prms;
		this->_converter_data.emplace_back(temp);
	} while (read != 0);
	//now the whole vector should be full of building block structures which are enough to run encoding on
	std::vector<std::future<converted_mp3>> futures;
	for (int i = 1; i < (this->_converter_data.size())+1; i++)
	{
		auto a=_converter_data.at(i);
		converter _conv_temp(a);
		
		futures.emplace_back(std::async(&(converter::encode_mp3),_conv_temp,std::move(a)));
	}
	
}

void assembler::reset_mp3()
{
	typedef std::vector<unsigned char> mp3data;
	mp3data bare = mp3data(MP3_SIZE, '\0');
	this->_mp3Construct.clear();
	for (int i = 0; i < _total_blocks; i++)
	{
		converted_mp3 temp;
		temp.mp3_buffer = bare;
		temp.order = i;
		temp.write = 0;
		this->_mp3Construct.emplace_back(temp);
	}
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
		auto a = fread(meta, 1, sizeof(wavFormat), _inputfile);
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
}

converter::converter(conversion_block input)
{
	this->_conversion_block.order = input.order;
	this->_conversion_block.pcmbuffer = input.pcmbuffer;
	this->_conversion_block._params = input._params;
	this->_conversion_block.readlength = input.readlength;
	this->_converted.order = this->_conversion_block.order;
}

converter::~converter()
{
}

converted_mp3 converter::encode_mp3(conversion_block input)
{
	this->_conversion_block.order = input.order;
	this->_conversion_block.pcmbuffer = input.pcmbuffer;
	this->_conversion_block._params = input._params;
	this->_conversion_block.readlength = input.readlength;
	this->_converted.order = this->_conversion_block.order;
	lame_t lame = lame_init();
	lame_set_in_samplerate(lame, int(this->_conversion_block._params.samplerate));
	lame_set_VBR(lame, vbr_default);
	//lame_set_VBR_mean_bitrate_kbps(lame, this->_conversion_block._params.bit_rate);
	lame_set_mode(lame, this->_conversion_block._params.channel);
	lame_set_VBR(lame, vbr_default);
	lame_set_VBR_mean_bitrate_kbps(lame, 128);
	int write;
	lame_set_VBR_q(lame, 0);
	std::vector<unsigned char> mp3_buffer(MP3_SIZE);
	if (this->_conversion_block.readlength == 0)

	{
		write = lame_encode_flush(lame, &mp3_buffer[0], MP3_SIZE);

	}
	else
	{
		write = lame_encode_buffer_interleaved(lame, &this->_conversion_block.pcmbuffer[0], this->_conversion_block.readlength, &mp3_buffer[0], MP3_SIZE);

	}

	this->_converted.mp3_buffer = mp3_buffer;
	this->_converted.write = write;
	return this->_converted;
}



