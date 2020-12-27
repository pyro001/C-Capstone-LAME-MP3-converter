cmake_minimum_required(VERSION 3.15)
project(CAPSTONE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
set(CMAKE_EXE_LINKER_FLAGS "-flto")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2 --std=c++17 -pthread -Wall -flto" )

enable_language(CXX)

find_package(Lame REQUIRED)
find_package (Threads REQUIRED)
set(LIBS ${LIBS} ${LAME_LIBRARIES} Threads::Threads)
include_directories(${LAME_INCLUDE_DIRS})

add_executable(CAPSTONE src/main.cpp)
target_link_libraries(CAPSTONE ${LIBS})
