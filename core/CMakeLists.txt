set(LIB_ZIA libzia)
set(ZIA zia)
set(ZIA_API_INC "${CMAKE_HOME_DIRECTORY}/sza/")
set(ZIA_COMMON_INC "${CMAKE_HOME_DIRECTORY}/common/src/")

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	if (CMAKE_BUILD_TYPE MATCHES DEBUG)
		set(LIB_MSVCRT msvcrt)
	else()
		set(LIB_MSVCRT msvcrtd)
	endif()
else()
	set(LIB_MSVCRT)
endif()

# We compile the project as a static library to make unit testing easier
file(GLOB LIB_ZIA_SRC ./src/*.cpp)
file(GLOB LIB_ZIA_HEADERS ./src/*.hpp)

add_library(${LIB_ZIA} ${LIB_ZIA_SRC})
target_include_directories(${LIB_ZIA} PUBLIC "./src/" ${ZIA_API_INC} ${ZIA_COMMON_INC})
add_sanitizers(${LIB_ZIA})

# Build the core
add_executable(${ZIA} main.cpp)
target_include_directories(${ZIA} PUBLIC "./src/")
target_link_libraries(${ZIA} ${LIB_ZIA} ${LIB_MSVCRT} zia_common)
add_sanitizers(${ZIA})
