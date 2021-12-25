#pragma once
#include <stdio.h>


enum Command{
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


enum ErrorCode{
    ERRCOMMAND = 1,
    ERRPOINTER,
    ERRMEMORY,
    ERRFILE,
    ERRLABEL
};


union types{
	float f;
	unsigned char b[4];
};


/// <summary>
/// Create file with binary code
/// </summary>
int CompilingFile(char* file_name, char* binary_file);


/// <summary>
/// Write data to the log file
/// </summary>
/// <param name="format">Format writing data</param>
/// <param name="">Params for writing</param>
void FileLogComp(const char* format, ...);


/// <summary>
/// Detect comand variations in the transmitted array by digital commands and write it to file
/// </summary>
/// <param name="dfile">Transmitted binary file</param>
/// <param name="p">Data array</param>
/// <param name="lines">Number of lines in data</param>
int BeginingIdentity(FILE* dfile, char** p, int lines, char data_labels[][10], int adress[]);


/// <summary>
/// Detect register number and add its number to data array
/// </summary>
/// <param name="data">Data array</param>
/// <param name="i">Current position in data array</param>
/// <param name="reg">Name of register</param>
void FillRegField(char* data, int* i, char* reg);


/// <summary>
/// Check existence of labels and count size of every command
/// </summary>
/// <param name="p">Lines of data</param>
/// <param name="lines">Number of lines</param>
/// <param name="data_labels">Array of found labels</param>
/// <param name="adress">Index of found labels</param>
void PreIdentity(char** p, int lines, char data_labels[][10], int adress[]);
