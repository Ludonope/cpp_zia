set(LIB_ZIA_AUTOINDEX zia_autoindex)
set(ZIA_API_INC "${CMAKE_HOME_DIRECTORY}/sza/")
set(ZIA_COMMON_INC "${CMAKE_HOME_DIRECTORY}/common/src/")

file(GLOB LIB_ZIA_AUTOINDEX_SRC ./src/*.cpp)
file(GLOB LIB_ZIA_AUTOINDEX_HEADERS ./src/*.hpp)

add_library(${LIB_ZIA_AUTOINDEX} SHARED ${LIB_ZIA_AUTOINDEX_SRC})
target_include_directories(${LIB_ZIA_AUTOINDEX} PUBLIC "./src/" ${ZIA_API_INC} ${ZIA_COMMON_INC})
target_link_libraries(${LIB_ZIA_AUTOINDEX} zia_common)
add_sanitizers(${LIB_ZIA_AUTOINDEX})
