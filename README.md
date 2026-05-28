# Calculator

A simple console-based calculator that parses mathematical expressions, builds an abstract syntax tree (AST), computes the result, and visualizes the tree structure.

## Features

*   Supports binary operators: `+`, `-`, `*`, `/`
*   Handles operator precedence and parentheses `( )`
*   Multi-digit numbers and ignores whitespaces
*   Recursive descent parser
*   Expression evaluation with division-by-zero check
*   Console visualization of the syntax tree (ASCII graphics)
*   Interactive menu
*   Manual memory management

## How It Works

1.  **Tokenization** — converts input string into a list of tokens.
2.  **Parsing** — the `Parser` class builds an expression tree using recursive descent.
3.  **Evaluation** — the `solve()` function traverses the tree and computes the result.
4.  **Visualization** — the `printTree()` function draws the tree sideways with `R--` (right child) and `L--` (left child) markers.
5.  **Cleanup** — the `deleteTree()` function frees dynamically allocated memory.

## Build and Run

The project consists of a single file `calculator.cpp`. Compile with any C++11 (or later) compiler:

```bash
g++ calculator.cpp -o calculator
./calculator