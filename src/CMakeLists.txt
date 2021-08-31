cmake_minimum_required(VERSION 3.17)
project(Tensat)

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
        util.h unionfind.h eclass.h)

target_link_libraries(Tensat ${CONAN_TARGETS})