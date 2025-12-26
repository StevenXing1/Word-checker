# Word Checker

A high-performance spell checker implementation featuring multiple advanced data structures for efficient word lookup and intelligent spelling suggestions.

## 🌟 Overview

Word Checker is a C++ application that validates word spellings against a dictionary dataset and provides intelligent suggestions for misspelled words. The project demonstrates three distinct data structure implementations, allowing for performance comparisons and educational insights into their trade-offs.

## ✨ Features

- **Spell Checking**: Validates whether a word exists in the dictionary
- **Smart Suggestions**: Generates spelling correction suggestions using five algorithms:
  - **Character Swapping**: Swaps adjacent characters (e.g., "teh" → "the")
  - **Character Insertion**: Inserts characters at various positions
  - **Character Deletion**: Removes individual characters
  - **Character Replacement**: Replaces characters with alternatives
  - **Character Splitting**: Splits words into two valid words

- **Multiple Data Structure Implementations**:
  - **AVL Tree Set**: Self-balancing binary search tree with O(log n) operations
  - **Hash Set**: Separately-chained hash table with dynamic resizing
  - **Skip List Set**: Probabilistic data structure with expected O(log n) performance

## 🏗️ Project Structure

```
Word-checker/
├── app/                      # Main application entry point
│   └── main.cpp             # Interactive spell check shell
├── core/                     # Core implementations
│   ├── WordChecker.cpp      # Spell checking and suggestion logic
│   ├── WordChecker.hpp
│   ├── AVLSet.hpp           # AVL tree implementation
│   ├── HashSet.hpp          # Hash table implementation
│   └── SkipListSet.hpp      # Skip list implementation
├── exp/                      # Experimental testing
│   └── expmain.cpp
└── gtest/                    # Unit tests
    ├── AVLSet_SanityCheckTests.cpp
    ├── HashSet_SanityCheckTests.cpp
    ├── SkipListSet_SanityCheckTests.cpp
    └── WordChecker_SanityCheckTests.cpp
```

## 🚀 Getting Started

### Prerequisites

- C++17 compatible compiler (GCC, Clang, or MSVC)
- CMake (optional, for building)
- Google Test (for running unit tests)

### Building the Project

**Using g++:**
```bash
# Build the main application
g++ -std=c++17 -o wordchecker app/main.cpp core/WordChecker.cpp

# Build with tests
g++ -std=c++17 -o test_runner gtest/*.cpp core/WordChecker.cpp -lgtest -lgtest_main -pthread
```

**Using CMake (recommended):**
```bash
mkdir build && cd build
cmake ..
make
```

### Running the Application

```bash
./wordchecker
```

The application provides an interactive shell where you can:
- Check if words are spelled correctly
- Get suggestions for misspelled words
- Switch between different data structure implementations
- Load custom dictionaries

## 💡 Usage Example

```
> CHECK hello
"hello" is spelled correctly.

> CHECK wrld
"wrld" is misspelled. Suggestions:
  - world
  - wild
  - wold

> LOAD dictionary.txt
Dictionary loaded successfully.
```

## 🧪 Testing

The project includes comprehensive unit tests using Google Test framework:

```bash
# Run all tests
./test_runner

# Run specific test suite
./test_runner --gtest_filter=WordChecker*
```

## 🔧 Technical Details

### Data Structures

1. **AVL Set** (`AVLSet.hpp`)
   - Self-balancing binary search tree
   - Guarantees O(log n) worst-case performance
   - Ideal for sorted data access

2. **Hash Set** (`HashSet.hpp`)
   - Separately-chained hash table
   - Dynamic resizing when load factor exceeds 0.8
   - Average O(1) lookup, insertion, and deletion

3. **Skip List Set** (`SkipListSet.hpp`)
   - Probabilistic data structure
   - Multiple levels with randomized forward pointers
   - Expected O(log n) operations

### Suggestion Algorithms

The `WordChecker` class implements five distinct algorithms to generate spelling suggestions:

1. **Swap adjacent characters**: For transposition errors
2. **Insert missing characters**: For omission errors
3. **Delete extra characters**: For insertion errors
4. **Replace wrong characters**: For substitution errors
5. **Split combined words**: For concatenation errors

## 📊 Performance Characteristics

| Data Structure | Insertion | Lookup | Deletion | Memory Overhead |
|---------------|-----------|--------|----------|-----------------|
| AVL Tree      | O(log n)  | O(log n) | O(log n) | Moderate       |
| Hash Set      | O(1) avg  | O(1) avg | O(1) avg | High           |
| Skip List     | O(log n)* | O(log n)* | O(log n)* | High          |

*Expected time complexity

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

## 📝 License

This project was developed as part of ICS 46 - Data Structure Implementation and Analysis coursework at UC Irvine.

## 🙏 Acknowledgments

- Original project framework from ICS 46 Winter 2022
- Inspired by classic spell-checking algorithms
- Built with educational purposes to demonstrate data structure implementations

## 📧 Contact

For questions or suggestions, please open an issue on GitHub.

---

**Note**: This is an educational project demonstrating data structure implementations and algorithm design patterns.
