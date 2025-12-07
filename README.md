# START-compiler

An open source compiler for the educational language START, originally developed by Leiden University and used by Leiden University in the Introduction to Programming course.

Full credit for the Start programming language belongs to Joost Broekens, Jonne Goedhart, and any other Leiden University employees who contributed.
We had no part in the creation of this language, this compiler is a hobby project to learn more about languages.


The language does all of:
- Lexical analysis and tokenization
- Recursive descent parser generating AST
- x86-64 assembly code generation
- Support for:
  - Integer and float variable declarations
  - Arithmetic expressions (+, -, *, /)
  - Comparison operators (<, >, <=, >=, ==, !=)
  - Logical operators (&, |, !)
  - If statements
  - While loops
  - Variable assignments

## Building
Standard (no CMake because its not large enough)
```bash
make
```

## Usage

Compile a START program to x86-64 assembly (ARM is not supported because we all used intel chips during development):

```bash
./start-compiler program.start > output.s
```

Alternatively if you just want to see the assembly output
```bash
./start-compiler program.start
```

Assemble and link:

```bash
gcc output.s -o program
./program
```

See `example.start` for an example demonstrating all language features.
