set(LIB_ZIA_CUSTOM_ROOT zia_custom_root)
set(ZIA_API_INC "${CMAKE_HOME_DIRECTORY}/sza/")
set(ZIA_COMMON_INC "${CMAKE_HOME_DIRECTORY}/common/src/")

file(GLOB LIB_ZIA_CUSTOM_ROOT_SRC ./src/*.cpp)
file(GLOB LIB_ZIA_CUSTOM_ROOT_HEADERS ./src/*.hpp)

add_library(${LIB_ZIA_CUSTOM_ROOT} SHARED ${LIB_ZIA_CUSTOM_ROOT_SRC})
target_include_directories(${LIB_ZIA_CUSTOM_ROOT} PUBLIC "./src/" ${ZIA_API_INC} ${ZIA_COMMON_INC})
target_link_libraries(${LIB_ZIA_CUSTOM_ROOT} zia_common)
add_sanitizers(${LIB_ZIA_CUSTOM_ROOT})
