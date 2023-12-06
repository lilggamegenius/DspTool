cmake_minimum_required(VERSION 3.16)
project(DspTool
		VERSION 1.0.0
		LANGUAGES C
		)

if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
		NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
	set(CMAKE_CXX_VISIBILITY_PRESET hidden)
	set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif ()

if (DEFINED DspTool_SHARED_LIBS)
	set(BUILD_SHARED_LIBS "${DspTool_SHARED_LIBS}")
endif ()

add_library(DspTool
		include/DspTool/dsptool.h
		decode.c
		encode.c
		math.c)
add_library(DspTool::DspTool ALIAS DspTool)
set_target_properties(DspTool PROPERTIES
		VERSION ${DspTool_VERSION}
		SOVERSION ${DspTool_VERSION_MAJOR})
target_include_directories(
		DspTool PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/DspTool>")
target_compile_features(DspTool PUBLIC cxx_std_11)

include(GenerateExportHeader)
generate_export_header(DspTool EXPORT_FILE_NAME include/DspTool/export.h)
target_compile_definitions(
		DspTool PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:DspTool_STATIC_DEFINE>")
target_include_directories(
		DspTool PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
option(DspTool_INCLUDE_PACKAGING "Include packaging rules for DspTool" "${is_top_level}")
if (DspTool_INCLUDE_PACKAGING)
	add_subdirectory(packaging)
endif ()

#=================================================================================================
if (BUILD_SHARED_LIBS)
	add_compile_definitions(COMPILING_DLL)
endif ()

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		add_compile_options (-fdiagnostics-color=always)
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		add_compile_options (-fcolor-diagnostics)
	endif ()
endif ()