# embedded_python_example

This requires you to have installed 
- python3-devel-3.10 (dnf install python3-devel)
- Python 3.10

Commands I use to compile & run
- Compile via: clang main.c avp_transformer.c -lpython3.10 -o main
- Run 'tests' via: ./main
- Run custom via something like this: ./main scripts.valid transform "my name jeff" 32
    - Note: The program is hardcoded to look in the scrips dir for python scripts
    - Note: The program expects that the python function takes in a single argument (string) and returns a single value (string)
