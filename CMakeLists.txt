cmake_minimum_required(VERSION 3.17)
project(Tensat)

set(CMAKE_CXX_STANDARD 20)

add_executable(Tensat
        egraph.h
        extractor.h
        index_map.h
        language.h
        machine.h
        main.cpp
        pattern.h
        rewrite.h
        runner.h
        types.h
        util.h)