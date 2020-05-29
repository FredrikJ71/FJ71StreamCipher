add_executable(LFSRtest
    LFSRtest.cpp
)

target_include_directories(LFSRtest PUBLIC ../include)
target_link_libraries(LFSRtest ../lib/LFSR64)
