cmake_minimum_required(VERSION 3.15)
project(crypteo)

add_executable(crypteo
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
	include/constants.h
)

target_compile_features(crypteo PRIVATE cxx_std_20)
target_include_directories(crypteo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

