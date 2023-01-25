cmake_minimum_required(VERSION 3.16)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if (NOT DEFINED DspTool_INSTALL_CMAKEDIR)
	set(DspTool_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/DspTool"
			CACHE STRING "Path to DspTool CMake files")
endif ()

install(TARGETS DspTool EXPORT DspTool_Targets
		RUNTIME COMPONENT DspTool_Runtime
		LIBRARY COMPONENT DspTool_Runtime
		NAMELINK_COMPONENT DspTool_Development
		ARCHIVE COMPONENT DspTool_Development
		INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(DIRECTORY "${DspTool_SOURCE_DIR}/include/" "${DspTool_BINARY_DIR}/include/"
		TYPE INCLUDE
		COMPONENT DspTool_Development)

if (BUILD_SHARED_LIBS)
	set(type shared)
else ()
	set(type static)
endif ()

install(EXPORT DspTool_Targets
		DESTINATION "${DspTool_INSTALL_CMAKEDIR}"
		NAMESPACE DspTool::
		FILE DspTool-${type}-targets.cmake
		COMPONENT DspTool_Development)

write_basic_package_version_file(
		DspToolConfigVersion.cmake
		COMPATIBILITY SameMajorVersion)

install(FILES
		"${CMAKE_CURRENT_SOURCE_DIR}/DspToolConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/DspToolConfigVersion.cmake"
		DESTINATION "${DspTool_INSTALL_CMAKEDIR}"
		COMPONENT DspTool_Development)