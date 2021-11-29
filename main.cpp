// Compiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "compiler.h"
#include <iostream>

int main()
{
    char file_name[] = "../../ConsolCommandStack/ConsolCommandStack/files/quadratic_equation.txt";
    char binary_file[] = "../../ConsolCommandStack/ConsolCommandStack/files/_quadratic_equation.txt";
    CompilingFile(file_name, binary_file);
}

