Part 1
There are a series of Required test cases for part 1 only. They are marked “RequiredTable” in
the tests file. You must pass these tests to get any credit for part 1. However, they are not
required for getting credit on Part 2. They will be treated separately. You are responsible for
implementing the following functions in WordSet.hpp. I recommend reading this section in its
entirety before beginning.
size_t polynomialHashFunction(const std::string & string,
unsigned base, unsigned mod)
template<unsigned int H1, unsigned int H2, unsigned int
EvictThreshold>
class BaseWordSet
Since your WordSet structure will be hashing strings, you will need a hash function in order to
map said strings to integers. This will indicate which index a given string is stored at in the
hash table.
This function will interpret the string parameter, which will contain only lower-case letters, as
coefficients for a polynomial of degree equal to the length of the string. You will evaluate it at
the point “base,” which can be thought of (if you prefer) as interpreting the string as a
base-that integer. Treat ‘a’ as 1, ‘b’ as 2, and so on. It returns the smallest positive integer
that is a representation of that string, in that base, mod the given modulus.
For example:
polynomialHashFunction(“abz”, 5, 10) = (1 * 52 + 2 * 51 + 26 * 50) % 10.
Be careful about when you take the modulus within the hash function, as these numbers can
get quite large. As a hint, you will want to recall some properties of modulus operations from
previous classes. Be careful about when you take the modulus outside the hash function for
the same reason. For this function you can only assume that base * base will not overflow
an unsigned 4-byte integer. That is, the range of base is: 2 <= base < .232 − 1
Do not assume any parameters match the named constants in the file! You should only
compute the hash value based on the parameters passed to this function.
For purposes of this project, this function will only ever receive a parameter that consists of
one or more lower-case letters.
This is the declaration of the class you will be working in. You will notice that it is templated,
but not in a way you are probably used to. Instead of using typename we are using an actual
type (unsigned int). This is known as a non-type template parameter. What it allows us to
do, is pass in constant values to the BaseWordSet at compile time. For this project we are
using it as an alternative to constants defined within the class. In your code, you can just treat
template<unsigned int H1, unsigned int H2, unsigned int
EvictThreshold>
BaseWordSet<H1, H2, EvictThreshold>::WordSet(size_t
initialCapacity)
This is the constructor for the WordSet class. You are required to use Cuckoo hashing to
implement your hash table. You should initialize any private member data here. There is only
one parameter to this function.
initialCapacity - This is the starting size of your two arrays used in Cuckoo hashing.
Your WordSet must be implemented using the cuckoo hashing policy. This means there will
be two underlying arrays, as the cuckoo hashing policy will use both for the eviction
H1, H2, and EvictThreshold as static class member variables (they aren’t the same, but you
can use them in the same way).
You will also see later on in the file, a set of lines that looks like:
using WordSet = BaseWordSet<_impl::BASE_H1, _impl::BASE_H2,
_impl::BASE_EVICTION_THRESHOLD>;
What this does is make it so that we can use WordSet which is a BaseWordSet with all of its
template parameters specified with default values. This WordSet is then used in all of the
provided tests. But you should make sure to test other values using BaseWordSet<yourH1,
yourH2, yourEvictThreshold>.
Example:
const size_t BASE_CAPACITY = 11;
proj3::WordSet words{BASE_CAPACITY};
Is the same as:
const size_t BASE_CAPACITY = 11;
proj3::BaseWordSet<proj3::_impl::BASE_H1,
proj3::_impl::BASE_H2,
proj3::_impl::BASE_EVICTION_THRESHOLD>
words{BASE_CAPACITY};
Is the same as:
const size_t BASE_CAPACITY = 11;
proj3::BaseWordSet<37, 41, 5> words{BASE_CAPACITY};
mechanism, as described in class. Your two tables must be implemented as dynamically
allocated C-style arrays. This means no vectors. initialCapacity will be the starting size
for each of these arrays. So, initially your WordSet will look like the following, assuming an
initialCapacity of 5 is used:
You will see two non-type template parameters (as described above) in the BaseWordSet
class H1 and H2. Under the cuckoo hashing policy you will sometimes be inserting strings into
T1 and sometimes into T2. When determining the index of a string for T1 you should call
polynomialHashFunction and use H1 as the “base” parameter. For example, let’s say we
want to insert “hello” into T1 we would call
polynomialHashFunction(“hello”, H1, initialCapacity). The result of this would be
the index we would store “hello” at. You would use H2 if you need to insert into T2, for example
if “hello” were to get evicted from T1.
If EvictThreshold evictions happen you should resize your hash table. More on this in the
documentation for the insert() function.
Also observe that this class, while templated with values, is not type-templated because it
will only store strings. Yay for not needing to test other types :D
template<unsigned int H1, unsigned int H2, unsigned int
EvictThreshold>
BaseWordSet<H1, H2, EvictThreshold>::~WordSet()
You need to implement the destructor for this class. Remember, the underlying storage for
your hash table must be implemented using dynamically allocated C-style arrays. You should
figure out a way to free the memory you used for those arrays here.
This means you should test your implementation using Valgrind, more specifically
./tests.sh -memcheck (or ./tests.sh -m)
Suppressed leaks are fine. Those are from the standard library and are explicitly ignored
using the memcheck.supp file. You will be penalized for anything else Valgrind reports.
template<unsigned int H1, unsigned int H2, unsigned int
EvictThreshold>
void BaseWordSet<H1, H2, EvictThreshold>::insert(const
std::string & string)
You should insert strings into your WordSet set using the cuckoo hashing policy. As
mentioned in the constructor documentation there will be two underlying arrays storing the
strings. When inserting a string using the cuckoo hashing policy you should always insert the
new string into its corresponding slot in T1 (determined by calling polynomialHashFunction
correctly). If there is already a string there, the existing string should be evicted and inserted
into T2. This eviction should continue up to EvictThreshold times, at which point a resize
should occur.
If you have a seemingly infinite rehashing loop, you probably have an issue with your hash
function.
When you resize and rehash, the next table size should be the smallest prime number that is
no smaller than twice the current table size. For example, if your current table size is 11, your
next one is 23. If your current table size were somehow 13, your next one would be 29.
For purposes of this project, insert will only ever receive a parameter that consists of one or
more lower-case letters. You will never be asked to insert the empty string.
template<unsigned int H1, unsigned int H2, unsigned int
EvictThreshold>
[[nodiscard]] bool BaseWordSet<H1, H2,
EvictThreshold>::contains(const std::string & string) const
Returns true if string s is in the WordSet, false if not.
template<unsigned int H1, unsigned int H2, unsigned int
EvictThreshold>
[[nodiscard]] size_t BaseWordSet<H1, H2, EvictThreshold>::size()
const noexcept
Returns the number of distinct strings in the WordSet.
template<unsigned int H1, unsigned int H2, unsigned int
EvictThreshold>
[[nodiscard]] size_t BaseWordSet<H1, H2,
EvictThreshold>::capacity() const noexcept
Returns the current capacity of the WordSet. In other words, how large each of the underlying
arrays are.
template<unsigned int H1, unsigned int H2, unsigned int
EvictThreshold>
void BaseWordSet<H1, H2, EvictThreshold>::erase(const
std::string & string)
If the given parameter is in the WordSet, it removes it. This does nothing if the WordSet does
not contain that string.
Part 2
There are a series of Required test cases for part 2 only. They are marked “Required” in the
tests file. You must pass these tests to get any credit for part 2. However, they are not required
for getting credit on Part 1. They will be treated separately.
template<unsigned int H1, unsigned int H2, unsigned int
EvictThreshold>
[[nodiscard]] std::vector<std::string> convert(const std::string
&start, const std::string &end, const BaseWordSet<H1, H2,
EvictThreshold> & words)
This function will return the conversion between start and end, according to the lowest Lewis
Carroll distance. The first element of the vector should be start, the last end, and each
consecutive should be one letter apart. Each element should be a valid word (i.e. it is present
in the words WordSet passed in). If there are two or more equally least Lewis Carroll
distance ways to convert between the two words, you may return any of them.
If there is no path between start and end, return an empty vector. The behavior of this
function is undefined (which means it won't be tested) for the case where start == end.
It is recommended that you compute the distance via a breadth-first search. To visualize this,
imagine a graph where the words are vertices and two vertices share an (undirected) edge if
they are one letter apart. If you do not know what this means, please ask any of the helpers or
post on EdStem.
You may use std::queue -- you do not need to write your own
A good thing to do the first time you see a word in the previous part is to place it into a
std::unordered_map<string, string>, where the key is the word you just saw and the value is
the word that you saw immediately before it. This will allow you to later produce the path:
you know the last word, and you know the prior word for each word in the path except the
first. Furthermore, if the key isn’t in that map, this tells you that you haven’t seen it before.
Your implementation does not have to be the most efficient thing ever, but it cannot be “too
slow.” In general, any test case that takes over a minute on GradeScope may be deemed a
wrong answer, even if it will later return a correct one. Note that your computer might be
faster than the one on GradeScope: if you have a particularly fast computer, I recommend you
treat the one minute limit as if it were lower, perhaps as low as 30 seconds.