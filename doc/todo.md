# ToDO

This document collects all issues and features that should be implemented. The implementation works from top to bottom. Hence, edit the order of the elements in the list to change importance.

- Implement the following test cases
    - Classical LFSR 1+x+x^4 (sn = sn-1 + sn-4)
    - test with 64-bit LFSR since this requires all bits of the state and LFSR 1 +x^9 + x^34 + x^61 + x^64
- Refactor LFSR-class to use standard uint64_t
- Refactor to add extra functions that are reused in pulic functions (example calculate new bit, update)
- Create a main program that takes input length, polynomial, state and generates a sequence of bits
- Add documentation in lib, test and bin to describe package