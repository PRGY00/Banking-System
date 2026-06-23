cmake_minimum_required(VERSION 4.0)
project(Banking_System C)

set(CMAKE_C_STANDARD 11)

add_executable(Banking_System main.c
        bank.h
        user.h
        bank.c
        user.c)
