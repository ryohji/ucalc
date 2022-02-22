# ucalc
Micro calculator implementation to explain a software development fundamentals.


## Usage

Pass the expression in a Reverse Polish Notation form as the command line
arguments. The result of calculation echo'ed back to stdout.  
For example:
```sh
$ ucalc + 1 - 2 \* 3 / 4 5
0.6
```


## How to build

Use C/C++ compiler to build the command.
```sh
clang -c src/ucalc.c
clang -c src/parser.cc -std=c++14
clang -o ucalc -lc++ *.o
```

Or you can use `make` to build:
```sh
make -C src
```

Note that `make` generates the command under the src directory.
