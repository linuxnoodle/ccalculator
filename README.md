# CCalculator 
A fully functioning CAS written in pure C99, with no external dependencies (excluding graphics). 

# Installation
```bash
$ git clone https://github.com/linuxnoodle/ccalculator
$ cd ccalculator
$ make
```

# Todo 
- [x] Implement arbitrary text lexing
- [x] Implement basic lexing 
- [X] Construct custom hashmaps
- [X] Finish implementation of abstract syntax tree parsing 
    - [X] Form basic arithmetic operators into a tree
    - [X] Manage order of operations 
    - [X] Add functions (e.g, sin, cos, sqrt)
- [ ] Add IMGUI graphical support
- [ ] Add LaTeX/KaTeX support for WYSIWYG formatting

# License 
This project is under the [GPLv2 License](/LICENSE.txt)
