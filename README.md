# CcowCompiler
## Summery
A semester long final project for my senior level Computer Compilers class. This project was a great exhibit of all the skills learned throughout my college career.
The project required us to make a program for every phase of a computer compiler with that being, a lexical analyzer (Scanner), syntax analyzer, optimization, and code generation. With the end goal of creating our own unique coding language and generate machine code to run in the terminal.
The project utilized table driven problem solving technique in order to make the code adaptive to changes and modulate the grammar rules separate from the code base. My compiler had to handle strings and integers, as well as include all IF, FOR, and WHILE statements, be able to print and input from the terminal, and handle all compare operators and mathematical operators.
## Running the code
The code has been writen in C++ but I haven't gone the extra mine for a make file so You'll have to compile the Scanner.cpp and the SytaxAnalyzer.cpp<br/>
`g++ scanner.cpp -o scanner`<br/>
`g++ SyntaxAnalyzer.cpp -o syntax`<br><br>
Then you can run these programs with the file path as a parameter<br>
`./scanner.exe [code file path]`<br>
The output of this command is the symble table and the tokens files of your program<br>
`./syntax.exe`<br>
The ending result is a .asm that after copying in the boiler plate information should be able to run on Linux 32bit systems.<br><br>
Not the most pretty work but still effective. The final grade for this project was a B.
