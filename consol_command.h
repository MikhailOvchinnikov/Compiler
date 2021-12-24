#pragma once
#include "..\..\Stack_dinamic\Stack_dinamic\stack.h"
#include <stdio.h>


enum Command
{
    PUSH = 1,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    OUT,
    DMP,
    HLT = 11,
    MOV,
    JMP,
    JA,
    CALL,
    RET,
    IN
};


enum Mask
{
    OOO = 0,
    IOO = 100,
    IOI = 101,
    III = 111,
    IIO = 110,
    OIO = 10,
    OII = 11
};


/// <summary>
/// Error codes
/// </summary>
enum ErrorCode
{
    //Error memory allocation
    MEMORYERROR = 1,

    //Error file reading
    FILEERROR,

    //Error command was got
    COMMANDERROR,

    //Division by 0
    DIVISIONERROR,

    //NULL pointer
    NULLPTR,
};


void FileLogCMD(const char* format, ...);


/// <summary>
/// Check appropriate size of data for operations
/// </summary>
/// <param name="stack">Transmitted stack</param>
/// <returns>1 if size of data is equal or over 2, else - 0</returns>
bool CheckMoreTwoElem(Stack* stack);


/// <summary>
/// Replace last value in the stack to its square root
/// </summary>
/// <param name="stack">Transmitted stack</param>
void Sqrt(Stack* stack);


/// <summary>
/// Print all elements of stack
/// </summary>
/// <param name="stack">Transmitted stack</param>
void Out(Stack* stack);


/// <summary>
/// Define a presence of a register based on the mask
/// </summary>
/// <param name="mask">Transmitted mask of expression</param>
/// <param name="data">Array of the data</param>
/// <param name="ind">Current pointer to the data</param>
/// <returns>0 if mask doesn't match and number of a register, if the mask match with one of the exist</returns>
char SwitchMaskReg(int mask, char* data, int* ind);


/// <summary>
/// Define a presence of a something value based on the mask
/// </summary>
/// <param name="mask">Transmitted mask of expression</param>
/// <param name="data">Array of the data</param>
/// <param name="ind">Current pointer to the data</param>
/// <returns>0 if mask doesn't match and value, if the mask match with one of the exist</returns>
float SwitchMaskVal(int mask, char* data, int* ind);


/// <summary>
/// Read binary file and separate to commands
/// </summary>
/// <param name="stack">Transmitted stack for data writing</param>
/// <param name="data">Reading data</param>
/// <param name="size">Data size</param>
/// <param name="memory">Pointer to operate memory</param>
/// <param name="registers">Pointer to register's array</param>
/// <returns>0 if everything is fine, else error code</returns>
int IdentifyData(Stack* stack, char* data, int size, float* memory, float* registers);


/// <summary>
/// Count number of lines in the transmitted file
/// </summary>
/// <param name="file">Transmitted file</param>
/// <param name="text">Pointer for write data from the file</param>
/// <param name="syms">Number of lines in the file</param>
/// <returns>0 if file is empty or error was found, else number of lines</returns>
int GetParametersFileInt(FILE* file, int* text, int syms);
