#ifndef LFSR64FIB_H
#define LFSR64FIB_H

#include <vector>
#include<cstdint>

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

    //create an lfsr with length l and feedback polynomial given by fx
    LFSR64Fib(unsigned int l, uint64_t fx){
        length = l;
        set_mask();
        feedback = fx;
        state = 1;
    }

    //create an lfsr with length l and feedback polynomial given by fx
    //initial state s
    LFSR64Fib(unsigned int l, uint64_t fx, uint64_t s){
        length = l;
        set_mask();
        feedback = fx;
        state = s;
    }

     //create an lfsr with characteristic polynomial as coefficients in cx
    //initial state 1
    LFSR64Fib(const std::vector<int> &cx){
        set_characteristic(cx);
        state = 1;
    }

    //create an lfsr with characteristic polynomial as coefficients in cx
    //initial state s
    LFSR64Fib(const std::vector<int> &cx, uint64_t s){
        set_characteristic(cx);
        state = s;
    }


    //change the feedback polynomial
    //note that by definition c_0 is 1 for a binary lfsr
    //hence the feedback polynimial fx should be in the form
    //fx = c_1,c_2, ... c_l where c_l is the least significant bit of fx
    void set_feedback(unsigned int l, uint64_t fx);

    //set the state of the lfsr to s
    void set_state(uint64_t s);

    //clock the lfsr once and output the bit that will be shifted out
    int next_bit();

    //set the characteristic polynomial from an array of tap positions, 
    //example s_k = s_k-1 + s_k-4 -> array{1,4}  
    void set_characteristic(const std::vector<int> &cx);

    //generate a 64-bit word
    uint64_t next_64();


    //-------------------------------------------------------------------------------------
    private:

    uint64_t state;
    uint64_t feedback;
    unsigned int length; //number of bits
    uint64_t mask;

    //calculate mask, assume length is set
    inline void set_mask() {
        mask = 1ULL<<(length-1);
    }

    //calculate new bit
    inline int new_bit() {
        //calculate bit pattern from lfsr
        uint64_t temp = state & feedback;
        //calulate xor of all bits in pattern
        temp ^= (temp>>1);
        temp ^= (temp>>2); 
        temp ^= (temp>>4);
        temp ^= (temp>>8); 
        temp ^= (temp>>16);
        temp ^= (temp>>32); 
        return temp & 1;
    }

    //update state of lfsr
    inline void update(){
        //start by calculate new bit
        int nb = new_bit();
        //shift and insert new bit ()
        state = nb?(state>>1)|mask:state>>1;
    } 

    //find the largest value in a vector of integers
    inline int max(const std::vector<int> &vec){
        int m_val = 0;
        for(auto& val : vec){
            if(val > m_val)
                m_val = val;
        }
        return m_val; 
    }  

}; 

#endif