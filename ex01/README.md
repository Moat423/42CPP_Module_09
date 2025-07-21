# Reverse polish Notation

[wikipedia RPN](https://en.wikipedia.org/wiki/Reverse_Polish_notation)
RPN is a form of writing mathematical expressions, that requires no brackets. it puts the operation sign, after the numbers to be evaluated.
There must always be two number (or one number and the result from a previous operation) before the sign. As it is evaluated left to right, you imediatly put the result of the last operation on top of the current stack, and then read on. Therefore the result becomes one of the possible numbers to an operation.

### std::istringstream

[cplusplus istringstream](https://cplusplus.com/reference/sstream/istringstream/istringstream/)

istringstream is an object that is great for parsing whitespace separated tokens:
```C++
std::istringstream iss(input);
std::string token;

while (iss >> token) {
    // Each iteration, token gets the next whitespace-separated word
}
```
Each extraction iss >> token reads from the stream up to the next whitespace (space, tab, newline).
