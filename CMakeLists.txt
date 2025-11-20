cmake_minimum_required(VERSION 3.24)
project(2526_TI301_PJT C)

set(CMAKE_C_STANDARD 11)

add_executable(TI_301_PJT
        main.c utils.c
        utils.h
        hasse.c
        hasse.h
        matrix.c
        matrix.h
        Partie_1.c
        Partie_1.h
        Partie_2.c
        Partie_2.h)

