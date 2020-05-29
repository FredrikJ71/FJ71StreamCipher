add_library (LFSR64
    STATIC
    FJ71StreamCipher/LFSR64Fib.h
    LFSR64Fib.cpp
)

target_include_directories(LFSR64 PUBLIC ../include)


