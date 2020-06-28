# MOFA
MOFA, or Mips Otiose Fallible Assembler, is a MIPS assembly language assembler that supports almost nothing.


## Usage
```
Accepted options:
  -h [ --help ]         Display help message.
  -i [ --input ] arg    Input file name. Positional option.
  -o [ --output ] arg   Optional output file name. Replace extension by '.hex'
                        unless specified.
```


## Limitations
Only supports 31 basic instructions listed [here](https://comp2008.gitee.io/lab2/instruction_detail/) and their basic usages. Everything else is not supported.


## Build Requirements
- C++17 capable compiler
- [boost::program_options](https://www.boost.org/doc/libs/release/doc/html/program_options.html)
