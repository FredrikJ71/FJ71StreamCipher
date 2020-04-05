#ifndef LFSR64FIB_H
#define LFSR64FIB_H

#include <vector>

//Class that represents a binary linear feedback shift register 
//with maximum length 64.
//The lfsr is implemented as a classic Fibonnaci lfsr that satisfies
//s_k = s_k-1*c_1 + s_k-2*c_2 + ... + s_k-l*c_l where
//c(x) = 1 + c_1*x + c_2*x^2 + ... + c_l*x^l is the characteristic polynomial
//Alternatively the feedback can be determined by the feedback polynomial
//f(x) = c_l + c_l-1*x + c_l-2*x^2 + ... + c_l-lx^l 
//Hence, f(x) is the reciprocal polynomial of c(x)

class LFSR64Fib{
    public:
    
    //create a default lfsr, length 1 with characteristic polynomial 1+x
    LFSR64Fib(){
        length = 1;
        set_mask();
        feedback = 1;
        state = 1;
    }

    //change the feedback polynomial
    //note that by definition c_0 is 1 for a binary lfsr
    //hence the feedback polynimial fx should be in the form
    //fx = c_1,c_2, ... c_l where c_l is the least significant bit of fx
    void set_feedback(unsigned int l, unsigned long long fx);

    //set the state of the lfsr to s
    void set_state(unsigned long long s);

    //clock the lfsr once and output the bit that will be shifted out
    int next_bit();

    //set the characteristic polynomial from an array of tap positions, 
    //example s_k = s_k-1 + s_k-4 -> array{1,4}  
    void set_characteristic(std::vector<int> &cx);

    //generate a 64-bit word
    unsigned long long next_64();

    private:
    unsigned long long state;
    unsigned long long feedback;
    unsigned int length;
    unsigned long long mask;

    //calculate mask, assume length is set
    inline void set_mask() {
        mask = 1ULL<<(length-1);
    }

}; 

#endif