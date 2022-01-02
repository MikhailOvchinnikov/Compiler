#pragma once
#include <stdio.h>


typedef struct Label Label;

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
/// Write data to the log file
/// </summary>
/// <param name="format">Format writing data</param>
/// <param name="">Params for writing</param>
void FileLogComp(const char* format, ...);


/// <summary>
/// Parse transmitted line and count byte size of programm
/// </summary>
/// <param name="data_ptr">Transmitted pointer to data rot writing to file or NULL pointer</param>
/// <param name="line">Set of lines from a reading file</param>
/// <param name="labels">Array for writing labels</param>
/// <param name="lines">Number of lines in a programm</param>
/// <returns>Byte size of programm</returns>
int ParseLine(char* data_ptr, char** line, Label* labels, int lines);


/// <summary>
/// Detect comand variations in the transmitted array by digital commands and write it to file
/// </summary>
/// <param name="dfile">Transmitted binary file</param>
/// <param name="sp_str">Data array</param>
/// <param name="lines">Number of lines in data</param>
/// <returns>-1 if an error occured, else -1</returns>
int Identity(FILE* dfile, char** sp_str, int lines);


/// <summary>
/// Create file with binary code
/// </summary>
int CompilingFile(char* file_name, char* binary_file);


/// <summary>
/// Detect register number and add its number to data array
/// </summary>
/// <param name="data">Data array</param>
/// <param name="i">Current position in data array</param>
/// <param name="reg">Name of register</param>
void FillRegField(char* data, int* i, char* reg);
