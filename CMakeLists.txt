cmake_minimum_required(VERSION 3.27)
project(CursedAslr CXX)

add_executable(cursed-aslr main.cpp)
target_compile_features(cursed-aslr PRIVATE cxx_std_17)
set_target_properties(cursed-aslr PROPERTIES POSITION_INDEPENDENT_CODE ON)
