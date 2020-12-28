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
class assembler
{
private:
    FILE* _inputfile;
    std::vector<short int[MP3_SIZE * 2]>  mp3_vect;
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
    wavFormat _wavfmt;
    struct lameParams
    {
        int bit_rate;
        int vbr_q;
        int samples;
    };
    lameParams _lamePrms;
    /* data */
public:
    assembler(/* args */);
    void setWavformat(FILE * inputFile );
    assembler(FILE _input, int vbr, int quality);
    ~assembler();
};




#endif