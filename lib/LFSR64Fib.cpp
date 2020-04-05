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
void LFSR64Fib::set_feedback(unsigned int l, unsigned long long fx){
    length = l;
    mask = 1ULL<<(length-1);
    feedback = fx;
} 

//set the state of the lfsr to s
void LFSR64Fib::set_state(unsigned long long s) {
    state = s;
}

//clock the lfsr once and output the bit that will be shifted out
int LFSR64Fib::next_bit(){
    int out = (int)state&1;
    //calculate the feeedback value
    unsigned long long feed_val = state & feedback;
    feed_val ^= (feed_val>>1);
    feed_val ^= (feed_val>>2);
    feed_val ^= (feed_val>>4);
    feed_val ^= (feed_val>>8);
    feed_val ^= (feed_val>>16);
    feed_val ^= (feed_val>>32);
    feed_val &= 1;
    //shift and input feedback bit
    state >>= 1;
    if(feed_val){
        state|=mask; 
    } 
    return out;
} 

//set the feedback funtion from an array of tap positions, 
//example s_k = s_k-1 + s_k-4 -> array{1,4}  
void LFSR64Fib::set_characteristic(std::vector<int> &cx){
    //start by detecting length and then set mask
    length = 0;
    for(int i=0;i < cx.size();++i ){
        if(cx[i] > length){
            length = cx[i] ;
        } 
    }
    mask = 1ULL<<(length-1);
    //set feedback
    feedback = 0;
    for(int i=0;i<cx.size();++i){
        feedback|=1ULL<<(length-cx[i]); 
    }
} 

//generate a 64-bit word
unsigned long long LFSR64Fib::next_64(){
    unsigned long long out_word = 0;
    unsigned long long msb = 0x8000000000000000ULL;
    for(int i=0;i<64;++i){
        out_word>>=1;
        int bit = next_bit();
        if(bit){
            out_word|=msb; 
        } 
    } 
    return out_word;
} 