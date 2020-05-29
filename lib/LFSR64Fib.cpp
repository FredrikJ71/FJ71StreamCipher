#include "FJ71StreamCipher/LFSR64Fib.h"

//Class that represents a binary linear feedback shift register 
//with maximum length 64.
//The lfsr is implemented as a classic Fibonnaci lfsr that satisfies
//s_k = s_k-1*c_1 + s_k-2*c_2 + ... + s_k-l*c_l where
//c(x) = 1 + c_1*x + c_2*x^2 + ... + c_l*x^l is the characteristic polynomial
//Alternatively the feedback can be determined by the feedback polynomial
//f(x) = c_l + c_l-1*x + c_l-2*x^2 + ... + c_l-lx^l 
//Hence, f(x) is the reciprocal polynomial of c(x)


//change the feedback polynomial
//note that by definition c_0 is 1 for a binary lfsr
//hence the feedback polynimial fx should be in the form
//fx = c_1,c_2, ... c_l where c_l is the least significant bit of fx
void LFSR64Fib::set_feedback(unsigned int l, uint64_t fx){
    length = l;
    set_mask();
    feedback = fx;
} 

//set the state of the lfsr to s
void LFSR64Fib::set_state(uint64_t s) {
    state = s;
}

//clock the lfsr once and output the bit that will be shifted out
int LFSR64Fib::next_bit(){    
    //calculate the bit that should be returned
    int out = (int)state&1;
    //update lfsr
    update();
    return out;
} 

//set the feedback funtion from an array of tap positions, 
//example s_k = s_k-1 + s_k-4 -> array{1,4}  
void LFSR64Fib::set_characteristic(const std::vector<int> &cx){
    //start by detecting length and then set mask
    length = max(cx);
    set_mask();
    //set feedback
    feedback = 0;
    for(auto &coef : cx){ 
        feedback|=1ULL<<(length-coef); 
    }
} 


//generate a 64-bit word
uint64_t LFSR64Fib::next_64(){
    uint64_t out_word = 0;
    uint64_t pos_mask  = 1; //start with least significant bit
    for(int i=0;i<64;++i){
        //store next bit in pos i 
        if(state&1){
            out_word|=pos_mask; 
        }
        //update pos_mask and lfsr
        pos_mask <<=1;
        update();

    } 
    return out_word;
} 
