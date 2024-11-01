vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
		OUT_SOURCE_PATH SOURCE_PATH
		REPO lilggamegenius/DspTool
		REF "${VERSION}"
		SHA512 1e0b2c549a356b94ff1af677dbff9c56af855b47801057c24244cb6e549c1afaf899bad4a80d853044f31a62dec16ab90145931d75403a65a5cab1b2f053e477
		HEAD_REF master
)

vcpkg_cmake_configure(
		SOURCE_PATH "${SOURCE_PATH}"
		OPTIONS
			-DDspTool_INSTALL_CMAKEDIR=share/dsptool/cmake
			-DCMAKE_INSTALL_LIBDIR=share/debug/lib
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "DspTool")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
