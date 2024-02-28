#ifndef __WORD_SET___HPP
#define __WORD_SET___HPP

#include <string>
#include <vector>
#include <cmath>

// You may assume, in writing this function, that the given
// string consists solely of lower-case letters.

// To get the "number" that a letter-digit represents,
// subtract 'a' from it.
// e.g., (s[0]-'a') is the most significant "digit" of s.
// That would give you a 0 for a, etc;  add one to get 'a' = 1, 'b' = 2, etc.

// After every "digit" is processed, mod the current value by mod.
// This will keep you from having overflow.
[[nodiscard]] size_t polynomialHashFunction(const std::string &string,
                                            unsigned base, unsigned mod);

// These are value template parameters. You can pass in a value rather than a
// type into the parameter and use its value. This allows them to be set
// dynamically rather than having static constants within the class.
// Think of them as class constants (you can just use them like a variables
// named H1, H2, and EvictThreshold)

template <unsigned int H1, unsigned int H2, unsigned int EvictThreshold>
class BaseWordSet {
   private:
    // You may declare private functions and member variables here.
    std::vector<std::string>* T1;
    std::vector<std::string>* T2;
    unsigned cap;
    unsigned evictionThreshold;
    unsigned count;

    bool isPrime(unsigned n) {
        if (n <= 1) return false;
        if (n <= 3) return true;

        if (n % 2 == 0 || n % 3 == 0) return false;

        unsigned i = 5;
        while (i * i <= n) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
            i += 6;
        }
        return true;
    }

    void resize() {
        unsigned temp = cap;
        unsigned newCapacity = cap * 2;

        while(isPrime(newCapacity) == false){
            newCapacity++;
        }

        std::vector<std::string>* newT1 = new std::vector<std::string>(newCapacity);
        std::vector<std::string>* newT2 = new std::vector<std::string>(newCapacity);
        std::swap(T1, newT1);
        std::swap(T2, newT2);
        cap = newCapacity;

        for (unsigned i = 0; i < temp; i++) {
            if (!(*newT1)[i].empty()) insert((*newT1)[i]);
            if (!(*newT2)[i].empty()) insert((*newT2)[i]);
        }

        delete newT1;
        delete newT2;

    }

   public:
    explicit BaseWordSet(size_t initialCapacity);
    ~BaseWordSet();

    // You do not need to worry about implementing these. They are only
    // here to ensure that we let the compiler know these functions
    // should not exist.
    BaseWordSet(const BaseWordSet &) = delete;
    BaseWordSet(BaseWordSet &&) = delete;
    BaseWordSet &operator=(const BaseWordSet &) = delete;
    BaseWordSet &operator=(BaseWordSet &&) = delete;

    // adds the given string to the WordSet, so that any
    // future calls to contains(s) will return true, unless that is later
    // removed.
    void insert(const std::string &string);

    [[nodiscard]] bool contains(const std::string &string) const;

    // return how many distinct strings are in the set
    [[nodiscard]] size_t size() const noexcept;

    // return how large the underlying array is.
    [[nodiscard]] size_t capacity() const noexcept;

    void erase(const std::string &string);
};

// You Should not use anything within the _impl namespace in your code.
// Instead reference the H1, H2, and EvictThreshold template parameters
namespace _impl {
const static unsigned int BASE_H1 = 37;
const static unsigned int BASE_H2 = 41;
const unsigned int BASE_EVICTION_THRESHOLD = 5;
}  // namespace _impl


// Add a usings statement with the default values for the project.
using WordSet =
    BaseWordSet<_impl::BASE_H1, _impl::BASE_H2, _impl::BASE_EVICTION_THRESHOLD>;

template <unsigned int H1, unsigned int H2, unsigned int EvictThreshold>
BaseWordSet<H1, H2, EvictThreshold>::BaseWordSet(size_t initialCapacity) {
    // TODO: Implement this

    cap = initialCapacity;
    count = 0;
    T1 = new std::vector<std::string>(initialCapacity);
    T2 = new std::vector<std::string>(initialCapacity);
    this->evictionThreshold = _impl::BASE_EVICTION_THRESHOLD;;

}

template <unsigned int H1, unsigned int H2, unsigned int EvictThreshold>
BaseWordSet<H1, H2, EvictThreshold>::~BaseWordSet() {
    // TODO: Implement this
    delete T1;
    delete T2;
}

template <unsigned int H1, unsigned int H2, unsigned int EvictThreshold>
void BaseWordSet<H1, H2, EvictThreshold>::insert(const std::string &string) {
    // TODO: Implement this
    std::string curr = string;
    for (unsigned i = 0; i < evictionThreshold; i++) {
        unsigned hash1 = polynomialHashFunction(curr, _impl::BASE_H1, cap);
        if ((*T1)[hash1].empty()) {
            (*T1)[hash1] = curr;
            count++;
            return;
        }
        std::swap((*T1)[hash1], curr);

        unsigned hash2 = polynomialHashFunction(curr, _impl::BASE_H2, cap);
        if ((*T2)[hash2].empty()) {
            (*T2)[hash2] = curr;
            count++;
            return;
        }
        std::swap((*T2)[hash2], curr);
    }
    resize();
    insert(curr);

}

template <unsigned int H1, unsigned int H2, unsigned int EvictThreshold>
bool BaseWordSet<H1, H2, EvictThreshold>::contains(
    const std::string &string) const {
    // TODO: Implement this. Stub is only to allow code to compile
    //       (you may remove it)
    unsigned hash1 = polynomialHashFunction(string, _impl::BASE_H1, cap);
    unsigned hash2 = polynomialHashFunction(string, _impl::BASE_H2, cap);

    if ((*T1)[hash1] == string || (*T2)[hash2] == string) {
        return true;
    }

    return false;
}

// return how many distinct strings are in the set
template <unsigned int H1, unsigned int H2, unsigned int EvictThreshold>
size_t BaseWordSet<H1, H2, EvictThreshold>::size() const noexcept {
    // TODO: Implement this. Stub is only to allow code to compile
    //       (you may remove it)

    return count;
}

// return how large the underlying array is.
template <unsigned int H1, unsigned int H2, unsigned int EvictThreshold>
size_t BaseWordSet<H1, H2, EvictThreshold>::capacity() const noexcept {
    // TODO: Implement this. Stub is only to allow code to compile
    //       (you may remove it)
    return cap;
}

// removes this word if it is in the wordset.
template <unsigned int H1, unsigned int H2, unsigned int EvictThreshold>
void BaseWordSet<H1, H2, EvictThreshold>::erase(const std::string &string) {
    // TODO: Implement this.
    unsigned hash1 = polynomialHashFunction(string, _impl::BASE_H1, cap);
    unsigned hash2 = polynomialHashFunction(string, _impl::BASE_H2, cap);
    if ((*T1)[hash1] == string) {
        (*T1)[hash1] = "";
        count--;
    } else if ((*T2)[hash2] == string) {
        (*T2)[hash2] = "";
        count--;
    }
}

#endif
