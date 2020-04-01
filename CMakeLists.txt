#boilerplate to initiate project
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
project(FJ71StreamCiphers LANGUAGES CXX)

#create an option that might be useful later on
option (SECRET FEATURE "Enable secret feature" OFF)

#add directory for building library, tests and binaries
add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(bin)
