# Math Parser

## Overview

Hey, I created a Math Parser using C. The program is designed to take user input, tokenize it, and evaluate the mathematical expression, then return the output back to the user. The implementation uses the Shunting Yard algorithm for infix to postfix conversion as a way to evaluate the result. It can handle basic arithmetic operations such as addition, subtraction, multiplication, and division, as well as parentheses behavior.

Hope you’d like it!

## Features
- Tokenizes mathematical expressions
- Supports basic operations: addition, subtraction, multiplication, division as well as parentheses behavior
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
   gcc -g main.c tokenization.c stack.c numeric_operations.c -o math_parser

4. Run the program::
   ```bash
   ./math_parser

For Visual Studio/Code

3. Load the math_parser.c source and header files in your IDE.

4. Build and run the program using your IDE's functionality.

## Usage
Once the Math Parser is running, you can input mathematical expressions directly You can choose between two modes: Default and Verbose. 
## Defualt mode:
in this mode the output will be shown without any process indicators
## Example:
Enter equation: 3 + 5 * (2 - 8)
Result: -27
## Verbose mode (-v):
   this mode will show the parser processing stages such as:
   - Input status
   - Memory allocation status
   - Tokenization status
   - Postfix notation

## Example (Linux/GCC):
./math_parser -v

## Output:
Enter equation: 3 + 5 * (2 - 8)
Input reading succeeded
Equation entered: 3 + 5 * (2 - 8)
Memory allocation succeeded for string
Memory allocation succeeded for tokens
Successfully tokenized the string
Postfix notation: 3528-*+
Result: -27

## What's Next
In the future, I'd like to add these features:

- **File Scanning**: Enable the parser to read mathematical expressions from a file. This will allow users to input expressions via files instead of using the command line.

- **Print Options**: Implement functionality to format and print results in a more user-friendly way. This could include options for displaying expressions with proper notation or saving outputs to a file.

- **Error Handling Enhancements**: Improve error detection and user feedback for invalid inputs, helping users understand what went wrong and how to fix it.

- **Extended Functionality**: Consider adding support for more advanced mathematical functions, such as algebraic operations.

- **Graphical User Interface (GUI)**: Find a solution to providing a more user-friendly graphical interface for interaction, rather than relying solely on the command line or IDE.




  

