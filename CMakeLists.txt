cmake_minimum_required(VERSION 3.15)
project(Capstone)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
set(CMAKE_EXE_LINKER_FLAGS "-flto")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2 --std=c++17 -pthread -Wall -flto" )

enable_language(CXX)
if(UNIX)
	find_package(Lame REQUIRED)
	find_package (Threads REQUIRED)
	set(LIBS ${LIBS} ${LAME_LIBRARIES} Threads::Threads)
	include_directories(${LAME_INCLUDE_DIRS})

	add_executable(Capstone src/main.cpp)
	target_link_libraries(Capstone ${LIBS})
endif(UNIX)
if(WIN32)
    message( "in the core: Windows found")
   
    message(${CMAKE_CURRENT_SOURCE_DIR}/src/)
   
	find_path(LAME_INCLUDE_DIR lame/lame.h)
#    find_package(Lame CONFIG REQUIRED)
    message(${LAME_INCLUDE_DIR})
	FIND_LIBRARY(LAME_LIBRARY libmp3lame)
    message(${LAME_LIBRARY})
#    if (LAME_INCLUDE_DIR AND LAME_LIBRARY)
#        message ("in the core libs found")
#    # you may need that if further action in your CMakeLists.txt depends
#    # on detecting your library
#        set(LAME_FOUND TRUE)
#    # those two, you really need
    include_directories(${LAME_INCLUDE_DIR})
#        set(CORE_LIB ${CORE_LIB} ${LAME_LIBRARY})
  #  endif ()
   # set(LIBS ${LIBS} ${LAME_LIBRARIES} Threads::Threads)
    file(GLOB project_SRCS src/*.cpp)

    add_executable (Capstone ${project_SRCS} )
    target_link_libraries(Capstone ${CMAKE_CURRENT_SOURCE_DIR}/src/libmp3lame.lib)
  
endif(WIN32)
