cmake_minimum_required(VERSION 3.31)
project(EDA_Trabalho_Final C)

set(CMAKE_C_STANDARD 99)

add_executable(EDA_Trabalho_Final main.c
        lib/src/TA.c
        lib/hdr/TA.h
        lib/src/TABM.c
        lib/hdr/TABM.h
        lib/src/THASH.c
        lib/hdr/THASH.h
        lib/src/gerar10k.c
        lib/hdr/gerar10k.h
        lib/src/testehash.c
        lib/hdr/testehash.h
        lib/src/THEAP.c
        lib/hdr/THEAP.h)
