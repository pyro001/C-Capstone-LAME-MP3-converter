#ifndef ASSEMBLERLAME_H_
#define ASSEMBLERLAME_H_
#define WAV_SIZE 8192
#define MP3_SIZE 8192
#include <stdio.h>
#include <lame/lame.h>
#include <thread>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <future>
#include <mutex>
enum converter_quality
{
	perfect = 0,
	good = 2,
	acceptable = 5
};

struct lameParams
{
	MPEG_mode channel;
	int bit_rate;
	int vbr_q;
	int samplerate;
};
struct conversion_block
{
	int readlength;
	lameParams _params;
	short int pcmbuffer[WAV_SIZE*2];
	int order;
};
struct converted_mp3
{
	int write;
	int order;
	unsigned char mp3_buffer[MP3_SIZE];
};
class assembler :public conversion_block
{

public:
	struct wavFormat
	{
		char chunk_id[4];
		int chunk_size;
		char format[4];
		char subchunk1_id[4];
		int subchunk1_size;
		short int audio_format;
		short int num_channels;
		int sample_rate; // sample_rate denotes the sampling rate.
		int byte_rate;
		short int block_align;
		short int bits_per_sample;
		char subchunk2_id[4];
		int subchunk2_size;
	};

	typedef struct wavFormat* _wavfmt;
	assembler(std::string input, std::string op);
	void run();
	void reset_mp3();
	lameParams get_lame_params(_wavfmt input);
	void setWavformat();
	assembler();
	~assembler();

private:
	FILE* _inputfile;
	std::string _input;
	FILE* _opfile;
	int _total_blocks;
	int _complete_bocks;
	float _completion_percentage;
	_wavfmt meta = (_wavfmt)malloc(sizeof(wavFormat));

	lameParams _lamePrms;
	std::vector<converted_mp3> _mp3Construct;
	std::vector<conversion_block> _converter_data;
};


class converter 
{
public:
	converter();
	converter(conversion_block input);
	~converter();
	converted_mp3 encode_mp3();

private:
	bool set;
	conversion_block _conversion_block;
	converted_mp3 _converted;
	std::mutex lck;
};





#endif