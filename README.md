# CPPND: Capstone  Repo
TASK

Write a C/C++ command line application that encodes a set of WAV files to MP3

Requirements:

(1) application is called with pathname as argument, e.g. <<>applicationname<>> F:\MyWavCollection all WAV-files contained directly in that folder are to be encoded to MP3

(2) use all available CPU cores for the encoding process in an efficient way by utilizing multi-threading

(3) statically link to lame encoder library

(4) application should be compilable and runnable on Windows and Linux

(5) the resulting MP3 files are to be placed within the same directory as the source WAV files, the filename extension should be changed appropriately to .MP3

(6) non-WAV files in the given folder shall be ignored

(7) multithreading shall be implemented in a portable way, for example using POSIX pthreads.

(8) frameworks such as Boost or Qt shall not be used

(9) the LAME encoder should be used with reasonable standard settings (e.g. quality based encoding with quality level "good")

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* Install [Lame](https://sourceforge.net/projects/lame/files/lame/) 
  * Linux <::> sudo apt-get install -y lame
  * Windows <::> vcpkg install mp3lame
  


## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./CAPSTONE` "<path\to\file\>"
### Testing the Project
1 .Note that because TurboVNC does not provide audio from Udacity VM's there might be some issues viewing the mp3 output, which in the text format looks like garbage data. 
2. for testing the code C-Capstone-LAME-MP3-converter\testfile has a bunch of sample "wav" files
3. to run `./CAPSTONE "<locationof installation/C-Capstone-LAME-MP3-converter/testfile>"`

### Class Structure
there 4 classes :
1.class converter : this is the smallest building block and converts a tiny chunk of a wav file to the mp3 format
2.class assembler: this splits the wav file, acquires parameters for lame to work, asynchronously calls the converter units and then compiles the responses into a proper mp3 file
3.class MessageQueue: this allows the asssembler class to convey its status while the assembler functionality is still running 
4.class filehandler: this acquires all wav files in the provided folder and asynchronously launches all assembler units to allow all the files to be concurrently converted
### Rubric Fulfillment
- The project demonstrates an understanding of C++ functions and control structures.
    - In the classes mentioned above, `for` loops and `while` loops are added: 
        1.main.cpp:34,
        2.taskmanager.cpp:78,96,102,108,125
        3.assemblerlame.cpp:55,82,86,101,115
    - if-else is used to validate data throughout
- The project reads data from a file and process the data, or the program writes data to a file.
    - the wav file is read to acquire format parameters in assembler::setWavformat()
    - wav files are read ,converted and then the converted data is written into mp3 files of the same name in assembler::run()
- The project uses multithreading.
    -  assembler::run() calls asynchronously run converter::encode_mp3() instances via futures
    - filehandler::emulate() calls asynchronously run instances of assembler::run() via futures 
- A promise and future is used in the project.
    - all asynchronous calls are made by futures ,filehandler::emulate() acquires no promise but assembler::run() acquires structure converted_mp3
- Templates generalize functions in the project. 
    -template function bool is_ready() in assemblerlame.cpp :line5 is used
- The project uses move semantics to move data, instead of copying it, where possible.
- A mutex or lock is used in the project. 
    - assembler::run() ,and most stdout calls within threaded functionalities are mutex protected
- A condition variable is used in the project.
    - the message queue class which passes the latest status message out to certain funcationalities uses conditional variables to notify 
