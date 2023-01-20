cmake_minimum_required(VERSION 3.16)
project(DspTool C)

set(CMAKE_C_STANDARD 11)

include_directories(dsptool)

add_compile_definitions(COMPILING_DLL)

add_library(DspTool SHARED
		dsptool/decode.c
		dsptool/dsptool.h
		dsptool/encode.c
		dsptool/math.c)

set_target_properties(DspTool PROPERTIES PUBLIC_HEADER dsptool/dsptool.h)

install(TARGETS DspTool
		PUBLIC_HEADER
		LIBRARY
		)