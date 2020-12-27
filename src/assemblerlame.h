#ifndef ASSEMBLERLAME_H_
#define ASSEMBLERLAME_H_
#define WAV_SIZE 8192
#define MP3_SIZE 8192
#include <lame/lame.h>
enum converter_quality
{
perfect=0,
good=2,
acceptable=0
};
class assembler
{
private:
    FILE *_inputfile;
    struct _wavFormat
    {
        /* data */
    };
    struct _lameParams
    {
        /* data */
    };
    
    /* data */
public:
    assembler(/* args */);

    assembler(FILE _input,int vbr,int quality);
    ~assembler();
};




#endif