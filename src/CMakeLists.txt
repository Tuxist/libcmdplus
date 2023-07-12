add_library(cmdplus STATIC cmdplus.cpp)


set_property(TARGET cmdplus PROPERTY VERSION ${Upstream_VERSION})
set_property(TARGET cmdplus PROPERTY SOVERSION ${LIBV})
set_property(TARGET cmdplus PROPERTY INTERFACE_systempp_MAJOR_VERSION ${LIBV})
set_property(TARGET cmdplus APPEND PROPERTY COMPATIBLE_INTERFACE_STRING cryptplus_MAJOR_VERSION)

install(TARGETS cmdplus DESTINATION lib EXPORT cmdplusTargets)

install(FILES cmdplus.h DESTINATION include/cmdplus)

set(ConfigPackageLocation lib/cmake/cmdplus)

export(EXPORT cmdplusTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/libcmdplusTargets.cmake"
  NAMESPACE Upstream::
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
    Upstream::
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
