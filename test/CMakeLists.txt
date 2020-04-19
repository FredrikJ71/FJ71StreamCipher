add_executable(LFSRtest
    LFSRtest.cpp
    ../lib/LFSR64Fib.cpp
)

target_include_directories(LFSRtest PUBLIC ../include)
