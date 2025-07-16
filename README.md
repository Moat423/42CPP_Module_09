# CPP Module 09 - STL Containers and Algorithms

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-98-blue.svg" alt="C++ 98">
  <img src="https://img.shields.io/badge/Status-Learning%20Project-green.svg" alt="Learning Project">
  <img src="https://img.shields.io/badge/42%20School-CPP%20Module%2009-darkgreen.svg" alt="42 School Project">
</p>

## 📝 Overview

This module focuses on the Standard Template Library (STL) containers and algorithms in C++98. The primary goal is to understand and implement various container types and utilize STL algorithms effectively. This module is particularly important as it introduces fundamental concepts used extensively in modern C++ programming.

## 🎯 Learning Objectives

- Understanding STL Containers
- Working with different container types
- Implementation of algorithms using STL
- Data structure efficiency and complexity
- Template programming concepts

## 🔍 Key Concepts

### STL Containers Used
1. **std::map**
    - Ordered associative container
    - Self-balancing binary search tree (typically Red-Black Tree)
    - Key-value pairs stored in sorted order by key
    - O(log n) complexity for insertion, deletion, and search
    - Perfect for Bitcoin exchange rates with dates as keys
    - Iterator traversal gives sorted output automatically

2. **std::stack**
    - LIFO (Last In, First Out) container
    - Adapter container
    - Based on other containers (usually deque)
    - No direct iteration through elements
    - Only top element is accessible
    - O(1) complexity for push and pop operations
    - Typically implemented over std::deque by default (I will be using std::list because of my restrictions)
    std::list
        - Double-linked list implementation
        - Non-continuous memory storage
        - Efficient insertion and deletion

3. **std::vector**
    - Dynamic array implementation
    - Contiguous memory storage
    - Fast random access O(1)
    - Amortized O(1) insertion at end
    - Poor for frequent insertions/deletions in middle O(n)
    - Memory reallocation can be costly
    - Size vs Capacity management

4. **std::deque**
    - Double-ended queue
    - Non-contiguous memory storage
    - Fast insertion/deletion at both ends O(1)
    - Random access O(1)
    - No reallocation of existing elements
    - More complex internal structure than vector
    - Higher memory overhead per element

### Main Exercises

#### 1. Bitcoin Exchange (ex00)
- Implementation of a Bitcoin price calculator
- Reading and parsing CSV files
- Date handling and validation
- Error management and data processing

#### 2. RPN (Reverse Polish Notation) (ex01)
- Implementation of a Reverse Polish Notation calculator
- Stack-based arithmetic operations
- Error handling for invalid expressions
- Understanding postfix notation

#### 3. PmergeMe (ex02)
- Implementation of a merge-insert sort algorithm
- Container comparison (std::vector vs std::list)
- Performance analysis and benchmarking
- Advanced sorting techniques

## 🛠️ Requirements

- C++ compiler supporting C++98
- Make
- Basic understanding of template programming
- Knowledge of STL containers and algorithms

## 🚀 Getting Started

```bash
# Clone the repository
git clone [your-repo-link]

# Navigate to the project directory
cd cpp-module-09

# Build the project
make

# Run individual exercises
./btc input.txt        # For ex00
./RPN "1 2 + 3 *"     # For ex01
./PmergeMe 3 2 1 4 5   # For ex02
```

## 📚 Project Structure

```
cpp-module-09/
├── ex00/
│   ├── btc.cpp
│   ├── BitcoinExchange.hpp
│   └── BitcoinExchange.cpp
├── ex01/
│   ├── RPN.cpp
│   ├── RPN.hpp
│   └── main.cpp
├── ex02/
│   ├── PmergeMe.cpp
│   ├── PmergeMe.hpp
│   └── main.cpp
└── README.md
```

## 💡 Key Learnings

1. **Container Selection**
   - Understanding when to use different container types
   - Performance implications of container choices
   - Memory management considerations

2. **Algorithm Implementation**
   - Efficient sorting techniques
   - Stack-based calculations
   - Data processing and validation

3. **Error Handling**
   - Robust input validation
   - Exception handling
   - Edge case management

## ⚠️ Common Pitfalls

1. Memory management with STL containers
2. Iterator invalidation
3. Performance considerations with different container types
4. Error handling in template functions

## 📖 Additional Resources

- [CPP Reference](http://www.cplusplus.com/reference/)
- [STL Containers Documentation](http://www.cplusplus.com/reference/stl/)
- [Effective STL by Scott Meyers](https://www.amazon.com/Effective-STL-Specific-Standard-Template/dp/0201749629)

## 🤝 Contributing

Feel free to submit issues and enhancement requests!

## 📜 License

This project is part of the 42 School curriculum. For more information about 42's licensing, please visit [42 School](https://42.fr/).
