cmake_minimum_required(VERSION 3.13)
project(CServer C)

set(CMAKE_C_STANDARD 99)

add_executable(CServer server.c)

add_executable(CClient client.c)