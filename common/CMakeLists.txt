set(LIB_ZIA_COMMON zia_common)
set(ZIA_API_INC "${CMAKE_HOME_DIRECTORY}/sza/")

file(GLOB LIB_ZIA_COMMON_SRC ./src/*.cpp)
file(GLOB LIB_ZIA_COMMON_HEADERS ./src/*.hpp)

add_library(${LIB_ZIA_COMMON} STATIC ${LIB_ZIA_COMMON_SRC})
target_include_directories(${LIB_ZIA_COMMON} PUBLIC "./src/" ${ZIA_API_INC})
add_sanitizers(${LIB_ZIA_COMMON})

add_subdirectory(test)
