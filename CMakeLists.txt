cmake_minimum_required(VERSION 2.6)
project(sdltest)

add_executable(sdltest main.cpp)

install(TARGETS sdltest RUNTIME DESTINATION bin)
