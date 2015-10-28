#Simplified SQL Interpreter
SSQL is a subset of features in standard SQL language
Four operations supported in SSQL:
* Create a new table
* Insert a row into an existed table
* Delete rows from an existed table
* Query on an existed table

No join, aggregation, complex constraints...

###Compiling command
1. `make`

    Generate a executable program named ssql
2. `make clean`

    Clear the made program.
    
3. `./ssql [path/]inputfile`

    Run the program and interpret the SSQL statements in the inputfile in the given path.

###Directory Detail
- src: include header files and the main program main.cpp
- test: include test cases we provide
- doc: our design and test report
