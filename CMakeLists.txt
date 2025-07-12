cmake_minimum_required(VERSION 3.31)
project(EDA_Trabalho_Final C)

set(CMAKE_C_STANDARD 99)

add_executable(EDA_Trabalho_Final main.c
        lib/TA.c
        lib/TA.h
        lib/TABM.c
        lib/TABM.h
        lib/THASH.c
        lib/THASH.h
        lib/gerar10k.c
        lib/gerar10k.h
        lib/testehash.c
        lib/testehash.h)
