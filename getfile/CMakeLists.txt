set(LIB_ZIA_GETFILE zia_getfile)
set(ZIA_API_INC "${CMAKE_HOME_DIRECTORY}/sza/")
set(ZIA_COMMON_INC "${CMAKE_HOME_DIRECTORY}/common/src/")

file(GLOB LIB_ZIA_GETFILE_SRC ./src/*.cpp)
file(GLOB LIB_ZIA_GETFILE_HEADERS ./src/*.hpp)

add_library(${LIB_ZIA_GETFILE} SHARED ${LIB_ZIA_GETFILE_SRC})
target_include_directories(${LIB_ZIA_GETFILE} PUBLIC "./src/" ${ZIA_API_INC} ${ZIA_COMMON_INC})
target_link_libraries(${LIB_ZIA_GETFILE} zia_common)
add_sanitizers(${LIB_ZIA_GETFILE})
