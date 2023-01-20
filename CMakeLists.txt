cmake_minimum_required(VERSION 3.16)
project(DspTool C)

set(CMAKE_C_STANDARD 11)

include_directories(dsptool)

add_compile_definitions(COMPILING_DLL)

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		add_compile_options (-fdiagnostics-color=always)
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		add_compile_options (-fcolor-diagnostics)
	endif ()
endif ()

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