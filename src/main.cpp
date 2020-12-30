#include <stdio.h>
#include <lame/lame.h>
#include <thread>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <future>
#include <mutex>
#include "assemblerlame.h"
#define MP3_SIZE 8192
#define WAV_SIZE 8192
bool done = false;

short int pcm_buffer[WAV_SIZE * 2];
std::vector<std::future<void>> futures;
//void encode(int read, lame_t lame, short int pcm_buffer[],FILE* mp3)
//{
//    lame= lame_init();
//    unsigned char mp3_buffer[8192];
//    std::mutex lck;
//    
//    int write = 0;
//
//    if (read == 0)
//
//    {
//        write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
//      
//    }
//    else
//    {
//        write = lame_encode_buffer_interleaved(lame, pcm_buffer, read, mp3_buffer, MP3_SIZE);
//      
//    }
//   
//  
//    std::cout << "encode called" << read << std::endl;
//
//
//    fwrite(mp3_buffer, write, 1, mp3);
//
//}
typedef struct header_file
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
	int subchunk2_size; // subchunk2_size denotes the number of samples.
} header;
typedef struct header_file* header_p;

int main(void)
{
	
	assembler a;
	a.run();

	while (!done)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}




	return (0);
	}
