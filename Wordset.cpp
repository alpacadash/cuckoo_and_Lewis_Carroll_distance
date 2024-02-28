#include "Wordset.hpp"

#include <string>
#include <cmath>

// returns s, as a number in the given base, mod the given modulus
size_t polynomialHashFunction(const std::string &string, unsigned int base,
                              unsigned int mod) {
    // TODO: Implement this. You may remove the stub (it is only here
    //       to allow it to compile).
    unsigned hashVal = 0;
    unsigned baseVal = 1;

    for (int i = string.length() - 1; i >= 0; i--) {
        hashVal = (hashVal + (string[i] - 'a' + 1) * baseVal) % mod;
        baseVal = (baseVal * base) % mod;
    }

    return hashVal;
}
