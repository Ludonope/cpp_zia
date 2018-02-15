cmake_minimum_required(VERSION 3.7)

# General configuration
set(LIB_ZIA_CRYPTO zia_crypto)

file(GLOB LIB_ZIA_CRYPTO_SRC ./src/*.cpp)
file(GLOB LIB_ZIA_CRYPTO_HEADERS ./src/*.hpp)
add_library(${LIB_ZIA_CRYPTO} STATIC ${LIB_ZIA_CRYPTO_SRC} ${LIB_ZIA_CRYPTO_HEADERS})
target_include_directories(${LIB_ZIA_CRYPTO} PUBLIC "./src/")
