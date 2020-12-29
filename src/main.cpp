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
typedef struct header_file *header_p;

int main(void)
{
    
    int read;
    DIR* dir = opendir("E:\\C++\\capstone\\CMakeProject1\\src");
    FILE *wav = fopen("E:\\C++\\capstone\\CMakeProject1\\src\\testcase.wav", "rb");
    FILE *mp3 = fopen("E:\\C++\\capstone\\CMakeProject1\\src\\testcase.mp3", "wb");
  
    short int pcm_buffer[WAV_SIZE * 2];
    // std::vector<typeof(mp3_buffer)> mp3vect;
    * pcm_buffer = (short int)malloc(sizeof(pcm_buffer));
    // int BUFSIZE = 256;					// BUFSIZE can be changed according to the frame size required (eg:512)
    int count = 0; // For counting number of frames in wave file.
    // short int buff16[BUFSIZE];				// short int used for 16 bit as input data format is 16 bit PCM audio
    header_p meta = (header_p)malloc(sizeof(header)); // header_p points to a header struct that contains the wave file metadata fields
    // int nb;							// variable storing number of bytes returned
    if (wav)
    {
        auto a = fread(meta, 1, sizeof(header), wav);
        std::cout << "read " << a << " bytes in the header" << std::endl;
        std::cout << " Size of Header file is " << sizeof(*meta) << " bytes" << std::endl;
        std::cout << " Sampling rate of the input wave file is " << meta->sample_rate << " Hz" << std::endl;
        std::cout << " Number of samples in wave file are " << meta->subchunk2_size << " samples" << std::endl;
        std::cout << " The number of channels of the file is " << meta->num_channels << " channels" << std::endl;
        std::cout << " Number of frames in the input wave file are " << count << std::endl;
    }

   // lame_t lame = lame_init();
   // lame_set_in_samplerate(lame,int(meta->sample_rate));
   // lame_set_VBR(lame, vbr_default);
   // lame_set_VBR_mean_bitrate_kbps(lame, 128);
   //
   // lame_set_VBR_q(lame, 0);
   // 
   // if (meta->num_channels == 1)
   //     lame_set_mode(lame,MONO);
   // else
   //     lame_set_mode(lame,STEREO);
   // lame_init_params(lame);
   // 
   //

   //fclose(wav);
   //wav = fopen("E:\\C++\\capstone\\C-Capstone-LAME-MP3-converter\\src\\testcase.wav", "rb");
   // do
   // {
   //     read = fread(pcm_buffer, 2 * sizeof(short int), WAV_SIZE, wav);
   //     lame_t lame2 = lame;
   //     lame_init_params(lame2);

   //     futures.emplace_back(std::async(std::launch::deferred, (&encode), std::move(read),(lame), (pcm_buffer), mp3));

   //   


   // }
   // while (read != 0);

   // while (!futures.empty())
   // {
   //     done = false;
   //     auto& a = futures.front();
   //     a.wait();
   //     a.get();
   //     futures.erase(futures.begin());
   //}
   // done = true;
   // //fclose(wav);
   // std::cout << "test";
   // lame_close(lame);

    assembler a;
    a.run();
  
   while(!done)
   {
       std::this_thread::sleep_for(std::chrono::milliseconds(100));
       
   }
   fclose(mp3);
   
    

    return 0;
}
