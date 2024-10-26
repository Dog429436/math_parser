# Math Parser

## Overview

Hey, I created a Math Parser using C. The program is designed to take user input, tokenize it, and evaluate the mathematical expression, then return the output back to the user. The implementation uses the Shunting Yard algorithm for infix to postfix conversion as a way to evaluate the result. It can handle basic arithmetic operations such as addition, subtraction, multiplication, and division, as well as parentheses behavior.

Hope you’d like it!

## Features
- Tokenizes mathematical expressions
- Supports basic operations: addition, subtraction, multiplication, and division
- Converts infix expressions to postfix notation
- Evaluates postfix expressions
- Dynamic memory management for arrays

## Prerequisites
- GCC compiler, Visual Studio, or Visual Studio Code

## Installation
To build and run the Math Parser, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/YourUsername/math_parser.git
   
2. Navigate to the project directory:
   ```bash
   cd math_parser

For GCC Compiler

3. Compile the source code:
   ```bash
   gcc -o math_parser math_parser.c

4. Run the program::
   ```bash
   ./math_parser

For Visual Studio/Code

3. Load the math_parser.c source and header files in your IDE.

4. Build and run the program using your IDE's functionality.

## Usage
Once the Math Parser is running, you can input mathematical expressions directly. For example:
Input: 3 + 5 * (2 - 8) Output: -27

Input: (10 + 2) * 3 Output: 36

## What's Next
The Math Parser is just the beginning! Here are some exciting features planned for future development:

- **File Scanning**: Enable the parser to read mathematical expressions from a file. This will allow users to input multiple expressions at once, making it easier to handle larger calculations or batch processing.

- **Print Options**: Implement functionality to format and print results in a more user-friendly way. This could include options for printing expressions with proper notation or saving outputs to a file for easy sharing.

- **Error Handling Enhancements**: Improve error detection and user feedback for invalid inputs, helping users understand what went wrong and how to fix it.

- **Extended Functionality**: Consider adding support for more advanced mathematical functions, such as trigonometric functions, exponentiation, and logarithms.

- **Graphical User Interface (GUI)**: Explore the possibility of creating a GUI for the Math Parser to enhance user experience and accessibility.




  

