cmake_minimum_required(VERSION 3.15)
project(wave-cryptography)

add_executable(wave-cryptography
	main.c 
	src/chacha.c 
	src/encoder.c 
	src/messageHeader.c
	src/menuGUI.c 
	src/containerHeader.c
	src/util.c
	include/chacha.h
	include/encoder.h
	include/menuGUI.h
	include/messageHeader.h
	include/util.h
	include/containerHeader.h
)

target_compile_features(wave-cryptography PRIVATE cxx_std_20)
target_include_directories(wave-cryptography PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

