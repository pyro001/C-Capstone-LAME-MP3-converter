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


### Rubric Fulfillment
-(1) application is called with pathname as argument, e.g. <<>applicationname<>> F:\MyWavCollection all WAV-files contained directly in that folder are to be encoded to MP3
  -works and tested on windows and linux

-(2) use all available CPU cores for the encoding process in an efficient way by utilizing multi-threading
  -works and tested on windows and linux the threads are all called with the std::async method whic spawn asynchronous threads if available, a deeper more controlled approch was attempted with asking the worker functions to pause via the message queue system but was later removed owing to functionality defects 

-(3) statically link to lame encoder library
 -lame no longer provides static linking with linuc since the packages can be apt- installed into system.The cmake of the project takes care of this and would run the installer for the user, for windows.

-(4) application should be compilable and runnable on Windows and Linux
  -done

-(5) the resulting MP3 files are to be placed within the same directory as the source WAV files, the filename extension should be changed appropriately to .MP3
  -done
-(6) non-WAV files in the given folder shall be ignored

-(7) multithreading shall be implemented in a portable way, for example using POSIX pthreads.
  -the code is called with -pthreads and since this is honoured in mordern cross platform gcc compilers, the code should be using posix threads everywhere.
-(8) frameworks such as Boost or Qt shall not be used
  -done

-(9) the LAME encoder should be used with reasonable standard settings (e.g. quality based encoding with quality level "good")
  -done
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
-Note that because TurboVNC does not provide audio from VM's there might be some issues viewing the mp3 output in a VM, which in the text format looks like garbage data. 

-For testing the code C-Capstone-LAME-MP3-converter\testfile has a bunch of sample "wav" files

-To run `./CAPSTONE "<locationof installation/C-Capstone-LAME-MP3-converter/testfile>"`

### Class Structure
There are 4 classes :

-1.`class converter` : this is the smallest building block and converts a tiny chunk of a wav file to the mp3 format

-2.`class assembler`: this splits the wav file, acquires parameters for lame to work from the file itself, asynchronously calls the converter units and then compiles the responses into a proper mp3 file

-3.`class MessageQueue`: this allows the asssembler class to convey its status while the assembler functionality is still running 

-4.`class filehandler`: this acquires all wav files in the provided folder and asynchronously launches all assembler units to allow all the files to be concurrently converted
