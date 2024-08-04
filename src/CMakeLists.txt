add_library(cmdplus STATIC cmdplus.cpp)


set_property(TARGET cmdplus PROPERTY VERSION ${Upstream_VERSION})
set_property(TARGET cmdplus PROPERTY SOVERSION ${LIBV})
set_property(TARGET cmdplus PROPERTY INTERFACE_systempp_MAJOR_VERSION ${LIBV})
set_property(TARGET cmdplus APPEND PROPERTY COMPATIBLE_INTERFACE_STRING cryptplus_MAJOR_VERSION)

install(TARGETS cmdplus DESTINATION lib EXPORT cmdplusTargets)

install(FILES cmdplus.h DESTINATION include/cmdplus)

set(ConfigPackageLocation lib/cmake/libcmdplus)

export(EXPORT cmdplusTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/libcmdplusTargets.cmake"
  NAMESPACE cmdplus::
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/libcmdplusVersion.cmake"
  VERSION ${Upstream_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(EXPORT cmdplusTargets
  FILE
    libcmdplusTargets.cmake
  NAMESPACE
    cmdplus::
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    "libcmdplusConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libcmdplusVersion.cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/libcmdplusConfig.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    set(CPACK_GENERATOR WIX)
    set(CPACK_PACKAGE_NAME "libcmdplus")
    set(CPACK_PACKAGE_VENDOR "tuxist.de")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libcmdplus - A Commandline Argument Parser Library")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_VERSION_MAJOR "1")
    set(CPACK_PACKAGE_VERSION_MINOR "0")
    set(CPACK_PACKAGE_VERSION_PATCH "0")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "libcmdplus")
    INCLUDE(CPack)
endif()
