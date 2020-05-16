# mahi::util CMakeLists.txt
# Evan Pezent (epezent@rice.edu)
# Updated: 3/2020

cmake_minimum_required(VERSION 3.13.0)

#===============================================================================
# USER OPTIONS
#===============================================================================

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(MAHI_UTIL_EXAMPLES         "Turn ON to build example executable(s)"             ON)
else()
    option(MAHI_UTIL_EXAMPLES         "Turn ON to build example executable(s)"            OFF)
endif()

option(MAHI_UTIL_COROUTINES       "Turn ON to build experimental coroutine support"        ON)
option(MAHI_UTIL_DEFAULT_LOG      "Turn ON to enable a default log output to console/file" ON)
option(MAHI_UTIL_LOG_CAPTURE_FILE "Turn ON to enable filename capture in logs"             ON)

#===============================================================================
# FRONT MATTER
#===============================================================================

# create the project
project(mahi-util VERSION 1.0.0 LANGUAGES C CXX)

if (BUILD_SHARED_LIBS)
    message(WARNING "mahi::util has not beeing tested as a shared library. Proceed at your own risk.")
endif()

# add ./cmake to CMake module path so our .cmake files can be found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(GNUInstallDirs) # defines conventional GNU isntallation directories

# Enable IDE folders and set them for predefined CMake projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if (UNIX)
        set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DLinux -D_GNU_SOURCE=1 -std=c++11")
    endif()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O3") # ALL WARNINGS
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /MP")    # MULTICORE BUILDS
endif()

#===============================================================================
# 3RD PARTY DEPENDENCIES
#===============================================================================

# JSON
set(JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/json/include)

# FMT
add_subdirectory(3rdparty/fmt)
install(TARGETS fmt EXPORT mahi-util-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# CXXOPTS
set(CXXOPTS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxxopts/include)

#===============================================================================
# CREATE LIBRARY
#===============================================================================

add_library(util "")
add_library(mahi::util ALIAS util)
set_target_properties(util PROPERTIES DEBUG_POSTFIX -d)
target_compile_features(util PUBLIC cxx_std_11)
install(TARGETS util EXPORT mahi-util-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(util PROPERTIES OUTPUT_NAME "mahi-util")

# defines
target_compile_definitions(util PUBLIC MAHI_UTIL) # for compatibility checks

# set necessary coroutine options
if(MAHI_UTIL_COROUTINES)
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(util PUBLIC "/await")
        target_compile_definitions(util PUBLIC MAHI_COROUTINES)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND APPLE)
        target_compile_options(util PUBLIC "-fcoroutines-ts")
        target_compile_definitions(util PUBLIC MAHI_COROUTINES)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
        target_compile_options(util PUBLIC "-fcoroutines-ts")
        target_compile_definitions(util PUBLIC MAHI_COROUTINES)
    else()
        message("Coroutines not supported by this compiler!")
        set(MAHI_UTIL_COROUTINES OFF)
    endif()
endif()

# enable logger
if (MAHI_UTIL_DEFAULT_LOG)
    target_compile_definitions(util PUBLIC MAHI_DEFAULT_LOG)
endif()

# enable logger file capture
if(MAHI_UTIL_LOG_CAPTURE_FILE)
    target_compile_definitions(util PUBLIC MAHI_LOG_CAPTURE_FILE)
endif()

# add source files
add_subdirectory(src/Mahi/Util)

# add include files
file(GLOB_RECURSE MAHI_UTIL_HEADERS "include/*.hpp" "include/*.h" "include/*.inl")
target_sources(util PRIVATE ${MAHI_UTIL_HEADERS}) # for intellisense
target_include_directories(util
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${JSON_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${CXXOPTS_INCLUDE_DIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# link libraries
target_link_libraries(util PUBLIC fmt::fmt)

if(UNIX)
    target_link_libraries(util PUBLIC pthread)
    if(NOT APPLE)
        target_link_libraries(util PUBLIC rt)
    endif()
endif()
if(WIN32)
    target_link_libraries(util PUBLIC winmm Pdh Version)
endif()

#===============================================================================
# WINDOWS ONLY
#===============================================================================

if(WIN32)
target_compile_definitions(util
    PUBLIC
        -D_CRT_SECURE_NO_WARNINGS          # remove secure warnings (e.g sprinft_s)
        -DNOMINMAX                         # remove min/max macros
        -D_WINSOCK_DEPRECATED_NO_WARNINGS  # remove winsock deprecated warnings
) 
endif(WIN32)

#===============================================================================
# NI LRT ONLY
#===============================================================================
if (NI_LRT)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
    target_link_libraries(util
        PRIVATE
            dl # Dynamic Linking Library
            rt # Realtime Extensions library
    )
    if (CMAKE_SYSTEM_PROCESSOR MATCHES ARM)
        target_compile_definitions(util PRIVATE MyRio_1900) # needed by NI src
    endif()
endif()

#===============================================================================
# EXAMPLE EXECUTABLES
#===============================================================================

if(MAHI_UTIL_EXAMPLES)
    message("Building mahi::util examples")
    add_subdirectory(examples)
endif()

#===============================================================================
# INSTALL
#===============================================================================

if (MSVC)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(MAHI_INSTALL_POSTFIX "") # 64 bit Windows is default
    else()
        set(MAHI_INSTALL_POSTFIX "-x86")
    endif()
elseif(NI_LRT AND CMAKE_SYSTEM_PROCESSOR MATCHES ARM)
    set(MAHI_INSTALL_POSTFIX "-nilrt-arm")
elseif(NI_LRT AND CMAKE_SYSTEM_PROCESSOR MATCHES ARM)
    set(MAHI_INSTALL_POSTFIX "-nilrt-x64")
endif()

set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}${MAHI_INSTALL_POSTFIX}")

# install headers
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${JSON_INCLUDE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CXXOPTS_INCLUDE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# set where we want to install our config
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/mahi-util)

# export the targets to a script
install(EXPORT mahi-util-targets
	FILE
		mahi-util-targets.cmake
	NAMESPACE
		mahi::
	DESTINATION
		${INSTALL_CONFIGDIR}
)

# include helper functions for creating config files that can be included by other projects to find and use a package
include(CMakePackageConfigHelpers)

# generate a package configuration file and a package version file
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/mahi-util-config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/mahi-util-config.cmake
	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/mahi-util-config-version.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# install the config and configversion modules
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/mahi-util-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/mahi-util-config-version.cmake
	DESTINATION ${INSTALL_CONFIGDIR}
)

# export from the build tree
export(EXPORT mahi-util-targets
	   NAMESPACE mahi::
	   FILE ${CMAKE_CURRENT_BINARY_DIR}/mahi-util-targets.cmake)
