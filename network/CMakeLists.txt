set(LIB_ZIA_NETWORK zia_network)
set(ZIA_API_INC "${CMAKE_HOME_DIRECTORY}/sza/")

file(GLOB LIB_ZIA_NETWORK_SRC ./src/*.cpp)
file(GLOB LIB_ZIA_NETWORK_HEADERS ./src/*.hpp)

add_library(${LIB_ZIA_NETWORK} SHARED ${LIB_ZIA_NETWORK_SRC})
target_include_directories(${LIB_ZIA_NETWORK} PUBLIC "./src/" ${ZIA_API_INC})
add_sanitizers(${LIB_ZIA_NETWORK})
