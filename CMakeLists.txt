# This library consists on a couple of C files and a set of
# header files that depends on the target architecture.

# TODO: I suppose that this implementation is for LINUX
# systems, in the future this might change.

# TODO: there's no (easy) way to check processor type, for
# now I suppose that CMake option toggles are the easiest to
# work with.

# +--------------------------------------------------------+
# |                      CMake Stuff                       |
# +--------------------------------------------------------+

# TODO: I have no idea which version should be required for these features
cmake_minimum_required(VERSION 3.16)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

# The version of the shared library
set(LIBRDTSC_VERSION 0.3)
set(LIBRDTSC_SOVERSION 0)

project(librdtsc
  VERSION ${LIBRDTSC_VERSION}
  DESCRIPTION "A library to read the TSC on multiple platforms."
)


# +--------------------------------------------------------+
# |                 Compilation variables                  |
# +--------------------------------------------------------+

# The following toggles are defined by this file in the following section and
# they change the behavior of the resuling library once compiled and installed.

# TODO: notice that some of these variables exclude one another!
option(LIBRDTSC_OS_LINUX    "Defined when the target OS is Linux"       ON)
option(LIBRDTSC_ARCH_x86    "Defined on x86/x64 platforms"              ON)
option(LIBRDTSC_ARCH_ARM    "Defined on ARM32 platforms"                OFF)
option(LIBRDTSC_ARCH_ARM64  "Defined on ARM64 platforms"                OFF)
option(LIBRDTSC_USE_HPET    "Enables HPET usage on x86/x64 platforms"   OFF)
option(LIBRDTSC_USE_PMU     "Enables PMU usage on ARM platforms"        OFF)

# +--------------------------------------------------------+
# |          Library Build and Install Properties          |
# +--------------------------------------------------------+

add_library(rdtsc SHARED
    src/cycles.c
    src/common_timer.c
    src/timer.c
)

set_target_properties(rdtsc PROPERTIES
  VERSION   ${LIBRDTSC_VERSION}
  SOVERSION ${LIBRDTSC_SOVERSION}
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/librdtsc/config.h
  @ONLY
)

target_include_directories(rdtsc
  # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  # $<INSTALL_INTERFACE:include>
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Install directory changes depending on build mode
if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
    # During debug, the library will be installed into a local directory
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/_install CACHE PATH "" FORCE)
else ()
    # This will install in /usr/lib and /usr/include
    set(CMAKE_INSTALL_PREFIX /usr CACHE PATH "" FORCE)
endif ()

# Specifying what to export when installing (GNUInstallDirs required)
install(TARGETS rdtsc
        EXPORT librstsc-config
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}) # Windows specific

# Actually to export the "include" directory we need to copy manually files
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/librdtsc DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# # This exports a CMake file that allows the project to be imported
# # into other CMake projects from the install directory.
# # The directory in which the file will be installed is a per-project
# # directory in this case, otherwise use common cmake directory
# install(EXPORT librdtsc-config DESTINATION share/rdtsc/cmake)

# # This makes the project importable from the build directory
# export(TARGETS rdtsc FILE librdtsc-config.cmake)

# # Any CMake project that needs to use FORB library can simply import all
# # its settings by using the following command within CMakeLists.txt:
# #
# #   find_package(librdtsc)


# +--------------------------------------------------------+
# |               Library Testing Properties               |
# +--------------------------------------------------------+

add_executable(rdtsc_test
  test/rdtsc_test.c
)

set_target_properties(rdtsc_test PROPERTIES
  VERSION   ${LIBRDTSC_VERSION}
  SOVERSION ${LIBRDTSC_SOVERSION}
)

target_link_libraries(rdtsc_test PRIVATE rdtsc)
