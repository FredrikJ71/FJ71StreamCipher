#include<iostream>
#include<cstdint>
#include"FJ71StreamCipher/LFSR64Fib.h"

int main(int argc, char* argv[]) {
    std::cout << "Running " << argv[0] << '\n';
    //verify that twe have enough input parameters
    if(argc!=5){
        std::cout << "Call with " << argv[0] << "length of lfsr, feedback polynomial (in Hex), ";
        std::cout << "inital state (in Hex), and number of bits to generate\n";
    }
    //parse input parameters 
    int lfsr_length = std::stoi(argv[1]); 
    std::cout << "Creating an lfsr with length " << lfsr_length << '\n';
    uint64_t feedback = std::stoi(argv[2],0,16);
    std::cout << "fx " << feedback << "\n";
    uint64_t state = std::stoi(argv[3],0,16);
    std::cout << "initial state " << state << "\n";
    int data_length = std::stoi(argv[4]); 
    std::cout << "Generate " << data_length << " bits\n";
    //calculate how many 64-bit blocks that should be generated
    //and how many extra bits
    int words64 = data_length/64;
    int extra_bits = data_length%64;
    //create lfsr
    LFSR64Fib my_lfsr(lfsr_length,feedback,state);
    //generate the 64 bit blocks
    for(int i=0;i<words64;++i){
        std::cout << std::hex << my_lfsr.next_64() << std::dec << " ";
        if( (i>0) && (i%8 == 0)){
            std::cout << '\n';
        }
    } 
    //generate final bits
    if(extra_bits>0){
        uint64_t final_word = 0;
        uint64_t mask = 1;
        for(int i=0;i<extra_bits;++i){
            if(my_lfsr.next_bit()){
                final_word|=mask; 
            }
            mask <<= 1; 
        }
        std::cout << std::hex << final_word << std::dec; 
    } 
    std::cout << "\n";
}
