# Brainfuck Interpreter - C++ 

## Nedelcu Alexandru-Constantin (341) & Săndulescu Ioan (341)

The interpreter reads the Brainfuck source code and input, validates matching brackets and builds a jump table to efficiently handle loops. It then executes the instructions using a 30000-cell memory tape, displaying the output in the terminal and saving it to output.out.

## How to Use


1. Write your Brainfuck program in `code.in`.
2. Add any required input to `input.in` (leave it empty if no input is needed).
3. Compile and run the interpreter:

   ```bash
   g++ main.cpp -o brainfuck
   ./brainfuck
   ```

4. The output will be displayed in the terminal and saved to `output.out`.

**Note:** All three files (`code.in`, `input.in`, and `output.out`) are expected to be in the working directory. `output.out` is created automatically.