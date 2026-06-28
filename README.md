# hash-table-in-c

Hash table with open addressing in C — insert, remove, get, and rehash for the Programming and Algorithms course.

[![License: MIT](https://img.shields.io/badge/License-MIT-3da639.svg)](LICENSE)
![Status](https://img.shields.io/badge/status-completed-6f42c1)

[![C](https://img.shields.io/badge/C-00599C?logo=c&logoColor=white)](https://en.cppreference.com/w/c)
[![Make](https://img.shields.io/badge/Make-4495D5?logo=make&logoColor=white)](https://www.gnu.org/software/make/)

[Portuguese](README.pt.md) | English

## About

Implementation of a hash table with open addressing and chaining for collision resolution. Includes insert, remove, get, dynamic rehash functions, iterator over keys and values, and a unit test suite.

## Requirements

| Tool | Minimum version |
| ---- | --------------- |
| GCC  | 9+              |
| Make | 4+              |

## How to run

```bash
make        # compiles
make test   # compiles and runs the test suite
```

Source code is in `src/` and tests in `test/`.

## License

Distributed under the **MIT** license, © 2024 Nycolas Souza.

It is a permissive license: anyone can use, copy, modify, and distribute the code, including in commercial projects, as long as the copyright notice and license text are retained.

The full text is in [LICENSE](LICENSE).
