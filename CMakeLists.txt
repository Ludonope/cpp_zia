# General CMake of the project
cmake_minimum_required(VERSION 3.7)

# Add Hunter packet manager
include("cmake_modules/HunterGate.cmake")
HunterGate(
	URL "https://github.com/ruslo/hunter/archive/v0.19.192.tar.gz"
	SHA1 "820996fcbb96b07684955e053d2be3eee8538183"
)

project(cpp_zia)
#set(CMAKE_BUILD_TYPE Debug)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# Location of additional cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# C++ rules
if((CMAKE_CXX_COMPILER_ID MATCHES GNU) OR (CMAKE_CXX_COMPILER_ID MATCHES Clang))
    #set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3 -DDEBUG -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_COVERAGE_FLAGS "--coverage")
    set(CMAKE_CXX_COVERAGE_LINK_FLAGS "")

    # Special Linux and OSX rules
    if (UNIX)
        set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -W -Wall -Wextra")
        if (NOT APPLE)
          set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -pthread")
          set(CMAKE_EXE_LINKER_FLAGS      "${CMAKE_EXE_LINKER_FLAGS} -ldl -pthread")
        endif()
        if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
            set(CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS} -std=c++1z -Weverything -Wno-c++98-compat-pedantic -Wno-weak-vtables -Wno-global-constructors -Wno-exit-time-destructors")
        else()
            set(CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS} -std=c++17 -Wno-literal-suffix")
        endif()
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} /std:c++17 /W3")
    set(CMAKE_CXX_FLAGS_DEBUG   "/Od /MDd /Zi /D DEBUG /D _DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/Ox /MDd /D NDEBUG")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_COVERAGE_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_CXX_COVERAGE_LINK_FLAGS}")
endif()

# Set ouput dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

configure_file("conf_example.json" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
configure_file("zia.crt" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
configure_file("zia.key" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)

# Get dependencies
if (UNIX)
	hunter_add_package(GTest)
	hunter_add_package(benchmark)
endif()

hunter_add_package(OpenSSL)
find_package(Sanitizers)
enable_testing()

# Build static libraries
add_subdirectory(common)
add_subdirectory(libcrypto)

# Build the core
add_subdirectory(core)

# Build the modules
add_subdirectory(autoindex)
add_subdirectory(custom_root)
add_subdirectory(network)
add_subdirectory(network_ssl)
add_subdirectory(php)
add_subdirectory(getfile)
