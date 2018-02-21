find_package(OpenSSL REQUIRED)

set(LIB_ZIA_NETWORK_SSL zia_network_ssl)
set(ZIA_API_INC "${CMAKE_HOME_DIRECTORY}/sza/")
set(ZIA_COMMON_INC "${CMAKE_HOME_DIRECTORY}/common/src/")

file(GLOB LIB_ZIA_NETWORK_SSL_SRC ./src/*.cpp)
file(GLOB LIB_ZIA_NETWORK_SSL_HEADERS ./src/*.hpp)

add_library(${LIB_ZIA_NETWORK_SSL} SHARED ${LIB_ZIA_NETWORK_SSL_SRC})
if (MSVC)
	target_include_directories(${LIB_ZIA_NETWORK_SSL} PUBLIC "./src/" ${ZIA_API_INC} ${ZIA_COMMON_INC} ${OPENSSL_INCLUDE_DIR})
	target_link_libraries(${LIB_ZIA_NETWORK_SSL} zia_common ${OPENSSL_LIBRARIES} CRYPT32.lib)
else()
	target_include_directories(${LIB_ZIA_NETWORK_SSL} PUBLIC "./src/" ${ZIA_API_INC} ${ZIA_COMMON_INC})
	target_link_libraries(${LIB_ZIA_NETWORK_SSL} zia_common OpenSSL::SSL OpenSSL::Crypto)
endif()
add_sanitizers(${LIB_ZIA_NETWORK_SSL})
