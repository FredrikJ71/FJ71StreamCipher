#include "FJ71StreamCipher/LFSR64Fib.h"
#include<iostream>
#include<vector>
#include<cstdint>

//testing 4-bit lfsr with c(x)=1+x+x^4
bool test_Fib_4(){
    bool succeed = true;
     // create a 4-bit LFSR with c(x)=1+x+x^4
    // starting state 1000 (s_0=1, s_1=0, s_2 = 0, s_3 = 0)
    // generated sequence should be (periodic with period 15)
    // (1000 1111 0101 100)
    // Hence the first 64-bits will be
    // 1000 1111 0101 1001 0001 1110 1011 0010 0011 1101 0110 0100 0111 1010 1100 1000
    // written as 64-bit hexadecimal word (note s_0 least significant bit)
    // 0x135e26bc4d789af1
    std::vector<int> correct_seq{1,0,0,0,1,1,1,1,0,1,0,1,1,0,0,1,0,0,0,1,1,1,1,0,1,0,1,1,0,0,1,0, 
            0,0,1,1,1,1,0,1,0,1,1,0,0,1,0,0,0,1,1,1,1,0,1,0,1,1,0,0,1,0,0,0};
    uint64_t correct_word = 0x135e26bc4d789af1;
    LFSR64Fib mylfsr{};
    //set feedback polynomial 1001 and starting state 1000
    mylfsr.set_feedback(4,0x9);
    mylfsr.set_state(1);
    //generate 64-bits 1 by 1
    std::vector<int> generated_seq(64);
    for(int i=0;i<64;++i){
        generated_seq[i] = mylfsr.next_bit();
    }
    if(generated_seq!=correct_seq){
        std::cout << "Error with basic 4-bit Fibonacci lfsr\n";
        succeed = false;
    }
    //generate all 64-bits in one chunck
    mylfsr.set_state(1);
    uint64_t test_word = mylfsr.next_64();
    if(test_word != correct_word){
        std::cout << "Error with basic 4-bit Fibonacci lfsr next_64()\n";
        succeed = false;
    }
    
    //test initate with vector with coefficients of characteristic polynomial
    //c(x)=1+x+x^4
    std::vector<int> char_pol{1,4};
    mylfsr.set_characteristic(char_pol);
    mylfsr.set_state(1);
    test_word = mylfsr.next_64();
    if(test_word != correct_word){
        std::cout << "Error when initiating from vector\n";
        succeed = false;
    }
    //test another 4-bit lfsr c(x)=1 + x^3 +x^4
    //starting with 1000 the following sequence will be generated
    // 1000 1001 1010 111
    std::vector<int> correct_seq2{1,0,0,0,1,0,0,1,1,0,1,0,1,1,1};
    //c(x) = 1 + x^3 +x^4 -> f(x) = 1+x+x^4
    mylfsr.set_feedback(4,0x3);
    mylfsr.set_state(1);
    //generate 15-bits 1 by 1
    std::vector<int> generated_seq2(15);
    for(int i=0;i<15;++i){
        generated_seq2[i] = mylfsr.next_bit();
    } 
    if(generated_seq2 != correct_seq2){
        std::cout << "Error with second 4-bit lfsr as feedback\n";
        succeed = false;
    }
    //repeat the following test but with characteristic polynomial instead
    std::vector<int> char_pol2{3,4};
    mylfsr.set_characteristic(char_pol2);
    mylfsr.set_state(1);
    //generate 15-bits 1 by 1
    for(int i=0;i<15;++i){
        generated_seq2[i] = mylfsr.next_bit();
    } 
    if(generated_seq2 != correct_seq2){
        std::cout << "Error with second 4-bit lfsr as characteristic\n";
        succeed = false;
    }
    //make a test where we change length
    mylfsr.set_feedback(14,0xac);
    uint64_t temp = mylfsr.next_64();
    //now reset lfsr
    mylfsr.set_characteristic(char_pol2);
    mylfsr.set_state(1);
    //generate 15-bits 1 by 1
    for(int i=0;i<15;++i){
        generated_seq2[i] = mylfsr.next_bit();
    } 
    if(generated_seq2 != correct_seq2){
        std::cout << "Error after manipulating length\n";
        succeed = false;
    }
    return succeed;
}

bool test_Fib_64(){
    bool succeed = true;
    const int test_length = 10000; //number of bits to verify
     // create a 64-bit LFSR with c(x)=1 + x^9 + x^34 + x^61 + x^64
     // s_t = s_t-9 + s_t-34 + s_t-61 + s_t-64
    // starting state 1 (s_0=1, s_1=0, s_2 = 0, ..., s_63 = 0)
    
    // Generare a reference sequence from the linear recurrence equation 
    std::vector<int> correct_seq(test_length);
    correct_seq[0] = 1;
    for(int i=1;i<64;++i){
        correct_seq[i] = 0;
    }
    for(int i=64;i<test_length;++i){
        correct_seq[i] = correct_seq[i-9] ^ correct_seq[i-34] ^ correct_seq[i-61] ^ correct_seq[i-64];
    }

    //create a 64-bit lfsr that should generate same sequence
    LFSR64Fib mylfsr{};
    std::vector<int> generated_seq(test_length);
    std::vector<int> char_pol{9,34,61,64};
    mylfsr.set_characteristic(char_pol);
    mylfsr.set_state(1);
    //generate 64-bits 1 by 1
    for(int i=0;i<test_length;++i){
        generated_seq[i] = mylfsr.next_bit();
    }
    if(generated_seq!=correct_seq){
        std::cout << "Error with 64-bit Fibonacci lfsr\n";
        succeed = false;
    }
    //verify as a feedback polynomial
    //c(x)=1+x^9+x^34+x^61+x^64
    //f(x)=x^64 + x^55 + x^30 + x^3 + 1
    //In hexadecimal, fx = 0x0080000040000009
    mylfsr.set_feedback(64,0x0080000040000009ULL);
    mylfsr.set_state(1);
     for(int i=0;i<test_length;++i){
        generated_seq[i] = mylfsr.next_bit();
    }
    if(generated_seq!=correct_seq){
        std::cout << "Error with 64-bit Fibonacci lfsr as feedback polynomial\n";
        succeed = false;
    }

    return succeed;
}

int main() {
    bool all_test = true;
    std::cout << "Testing binary LFSR in Fibonacci mode\n"; 
    std::cout << "4-bit LFSR: ";
    if(test_Fib_4()){
        std::cout << "Passed!\n";
    }
    else {
        std::cout << "Failed!\n";
        all_test = false;
    }
    std::cout << "64-bit LFSR: ";
    if(test_Fib_64()){
        std::cout << "Passed!\n";
    }
    else {
        std::cout << "Failed!\n";
        all_test = false;
    }
    if(all_test){
        std::cout << "Passed all tests!\n";
    }
    else {
        std::cout << "At least one test failed! Verify output to locate error!\n";
    }
}