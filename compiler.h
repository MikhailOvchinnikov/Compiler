#pragma once
#include <stdio.h>


/// <summary>
/// Create file with binary code
/// </summary>
void CompilingFile();


/// <summary>
/// Detect comand variations in the transmitted array by digital commands and write it to file
/// </summary>
/// <param name="dfile">Transmitted binary file</param>
/// <param name="p">Data array</param>
/// <param name="lines">Number of lines in data</param>
void BeginingIdentity(FILE* dfile, char** p, int lines, char data_labels[][10], int adress[]);


/// <summary>
/// Detect register number and add its number to data array
/// </summary>
/// <param name="data">Data array</param>
/// <param name="i">Current position in data array</param>
/// <param name="reg">Name of register</param>
void FillRegField(char* data, int* i, char* reg);

/// <summary>
/// Separate int figure to 4 char type
/// </summary>
/// <param name="data">Data array</param>
/// <param name="i">Current position in data array</param>
/// <param name="digit">Transmitted figure</param>
void SeparateIntToChar(char* data, int* i, int digit);


/// <summary>
/// Check existence of labels
/// </summary>
/// <param name="p">Lines of data</param>
/// <param name="lines">Number of lines</param>
/// <param name="data_labels">Array of found labels</param>
/// <param name="adress">Index of found labels</param>
void PreIdentity(char** p, int lines, char data_labels[][10], int adress[]);
