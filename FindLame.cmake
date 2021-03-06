# LAME_FOUND - system has liblame
# LAME_INCLUDE_DIRS - the liblame include directory
# LAME_LIBRARIES - The liblame libraries

##find_path(LAME_INCLUDE_DIRS lame/lame.h)
##find_library(LAME_LIBRARIES mp3lame)

##include(FindPackageHandleStandardArgs)
##find_package_handle_standard_args(Lame DEFAULT_MSG LAME_INCLUDE_DIRS LAME_LIBRARIES)

##list(APPEND LAME_DEFINITIONS -DHAVE_LIBMP3LAME=1)

##mark_as_advanced(LAME_INCLUDE_DIRS LAME_LIBRARIES LAME_DEFINITIONS)
FIND_PATH(LAME_INCLUDE_DIR lame/lame.h)
FIND_LIBRARY(LAME_LIBRARIES NAMES mp3lame)

IF(LAME_INCLUDE_DIR AND LAME_LIBRARIES)
	SET(LAME_FOUND TRUE)
ENDIF(LAME_INCLUDE_DIR AND LAME_LIBRARIES)

IF(LAME_FOUND)
	IF (NOT Lame_FIND_QUIETLY)
		MESSAGE(STATUS "Found lame includes:	${LAME_INCLUDE_DIR}/lame/lame.h")
		MESSAGE(STATUS "Found lame library: ${LAME_LIBRARIES}")
	ENDIF (NOT Lame_FIND_QUIETLY)
ELSE(LAME_FOUND)
	IF (Lame_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could NOT find lame development files")
	ENDIF (Lame_FIND_REQUIRED)
ENDIF(LAME_FOUND)
