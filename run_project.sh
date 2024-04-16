#!/bin/bash
gcc -o my_program main.c algorithms.c process.c queue.c scheduler.c -I.
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    ./my_program
else
    echo "Compilation failed."
fi
