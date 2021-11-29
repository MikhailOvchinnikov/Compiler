#define _CRT_SECURE_NO_WARNINGS
#define COM_LEN 50

#include "compiler.h"
#include "commands.h"
#include "..\..\Sort_Onegin\Sort_Onegin\sort_text.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



void BeginingIdentity(FILE* dfile, char** p, int lines, char data_labels[][10], int adress[])
{
    char* data = (char*)calloc(lines * 20, sizeof(char));

    if (data == NULL)
    {
        fputs("Error memory allocation for *data\n", stderr);
        exit(2);
    }
    char command[COM_LEN] = {};
    float digit = 0;
    char reg[50] = {};
    int size = 0;
    int ret_index = 0;
    for (int i = 0, k = 0; k < lines; k++)
    {
        bool wrong_command = 0;

        if (sscanf(p[k], "%s [%s + %f]", command, reg, &digit) == 3)
        {
            if (!strcmp(command, "pop"))
            {
                data[i++] = POP;
                data[i++] = 1;
                data[i++] = 1;
                data[i++] = 1;
                FillRegField(data, &i, reg);
                SeparateFloatToChar(data, &i, digit);
            }
            else if (!strcmp(command, "push"))
            {
                data[i++] = PUSH;
                data[i++] = 1;
                data[i++] = 1;
                data[i++] = 1;
                FillRegField(data, &i, reg);
            }
            else
            {
                wrong_command = 1;
            }
        }
        else if (sscanf(p[k], "%s %s + %f", command, reg, &digit) == 3)
        {
            if (!strcmp(command, "push"))
            {
                data[i++] = PUSH;
                data[i++] = 1;
                data[i++] = 1;
                data[i++] = 0;
                FillRegField(data, &i, reg);
                SeparateFloatToChar(data, &i, digit);
            }
            else
            {
                wrong_command = 1;
            }
        }
        else if (sscanf(p[k], "%s %[^,],%f", command, reg, &digit) == 3)
        {
            if (!strcmp(command, "in"))
            {
                data[i++] = IN;
                FillRegField(data, &i, reg);
                SeparateFloatToChar(data, &i, digit);
            }
            else
            {
                wrong_command = 1;
            }
        }
        else if (sscanf(p[k], "%s [%f]", command, &digit) == 2)
        {
            if (!strcmp(command, "pop"))
            {
                data[i++] = POP;
                data[i++] = 0;
                data[i++] = 1;
                data[i++] = 1;
                SeparateFloatToChar(data, &i, digit);
            }
            else if (!strcmp(command, "push"))
            {
                data[i++] = PUSH;
                data[i++] = 0;
                data[i++] = 1;
                data[i++] = 1;
                SeparateFloatToChar(data, &i, digit);
            }
            else
            {
                wrong_command = 1;
            }
        }
        else if (sscanf(p[k], "%s [%s]", command, reg) == 2)
        {
            if (!strcmp(command, "pop"))
            {
                data[i++] = POP;
                data[i++] = 1;
                data[i++] = 0;
                data[i++] = 1;
                FillRegField(data, &i, reg);
            }
            else if (!strcmp(command, "push"))
            {
                data[i++] = PUSH;
                data[i++] = 1;
                data[i++] = 0;
                data[i++] = 1;
                FillRegField(data, &i, reg);
            }
            else
            {
                wrong_command = 1;
            }
        }
        else if (sscanf(p[k], "%s %f", command, &digit) == 2)
        {
            if (!strcmp(command, "push"))
            {
                data[i++] = PUSH;
                data[i++] = 0;
                data[i++] = 1;
                data[i++] = 0;
                SeparateFloatToChar(data, &i, digit);
            }
            else if (!strcmp(command, "jmp"))
            {
                data[i++] = JMP;
                SeparateFloatToChar(data, &i, digit);
            }
            else
            {
                wrong_command = 1;
            }
        }
        else if (sscanf(p[k], "%s %s", command, reg) == 2)
        {
            if (!strcmp(command, "pop"))
            {
                data[i++] = POP;
                data[i++] = 1;
                data[i++] = 0;
                data[i++] = 0;
                FillRegField(data, &i, reg);
            }
            else if (!strcmp(command, "push"))
            {
                data[i++] = PUSH;
                data[i++] = 1;
                data[i++] = 0;
                data[i++] = 0;
                FillRegField(data, &i, reg);
            }
            else if (!strcmp(command, "jmp"))
            {
                data[i++] = JMP;
                for (int p = 0; p < 10; p++)
                {
                    if (strlen(data_labels[p]) == 0)
                    {
                        printf("Labels not found\n");
                        exit(5);
                    }
                    if (!strcmp(reg, data_labels[p]))
                    {
                        SeparateIntToChar(data, &i, adress[p]);
                        break;
                    }
                }
            }
            else if (!strcmp(command, "ja"))
            {
                data[i++] = JA;
                for (int p = 0; p < 10; p++)
                {
                    if (strlen(data_labels[p]) == 0)
                    {
                        printf("Labels not found\n");
                        exit(5);
                    }
                    if (!strcmp(reg, data_labels[p]))
                    {
                        SeparateIntToChar(data, &i, adress[p]);
                        break;
                    }
                }
            }
            else if (!strcmp(command, "call"))
            {
                data[i++] = CALL;
                for (int p = 0; p < 10; p++)
                {
                    if (strlen(data_labels[p]) == 0)
                    {
                        printf("Labels not found\n");
                        exit(5);
                    }
                    if (!strcmp(reg, data_labels[p]))
                    {
                        SeparateIntToChar(data, &i, adress[p] - 1);
                        break;
                    }
                }
                ret_index = i;
            }
            else
            {
                wrong_command = 1;
            }
        }
        else if (sscanf(p[k], "%s", command) == 1)
        {
            if (!strcmp(command, "pop"))
            {
                data[i++] = POP;
                data[i++] = 0;
                data[i++] = 0;
                data[i++] = 0;
            }
            else if (!strcmp(command, "add"))
            {
                data[i++] = ADD;
            }
            else if (!strcmp(command, "sub"))
            {
                data[i++] = SUB;
            }
            else if (!strcmp(command, "mul"))
            {
                data[i++] = MUL;
            }
            else if (!strcmp(command, "div"))
            {
                data[i++] = DIV;
            }
            else if (!strcmp(command, "sqrt"))
            {
                data[i++] = SQRT;
            }
            else if (!strcmp(command, "out"))
            {
                data[i++] = OUT;
            }
            else if (!strcmp(command, "dmp"))
            {
                data[i++] = DMP;
            }
            else if (!strcmp(command, "hlt"))
            {
                data[i++] = HLT;
            }
            else if (!strcmp(command, "ret"))
            {
                data[i++] = RET;
            }
            else
            {
                wrong_command = 1;
            }
        }
        if (wrong_command)
        {
            int n = 0;
            sscanf(p[k], "%[^:]:%n", command, &n);
            if (!n)
            {
                printf("Wrong command was getted \"%s\"\n", command);
                exit(3);
            }
        }
        size = i;
    }
    fwrite(data, sizeof(char), size, dfile);
    free(data);
}


void CompilingFile(char* file_name, char* binary_file)
{
    FILE* file = fopen(file_name, "rb");
    if (file == NULL)
    {
        fputs("Error file open\n", stderr);
        exit(1);
    }

    int syms = CountSymbols(file);
    char* text = (char*)calloc((syms + 1), sizeof(char));

    int lines = GetParametersFile(file, text, syms);
    fclose(file);

    char** p = (char**)calloc(lines, sizeof(char*));
    if (p == NULL)
    {
        fputs("Error memory allocation for **p\n", stderr);
        exit(2);
    }
    for (int i = 0; i < lines; i++)
    {
        p[i] = (char*)calloc(100, sizeof(char));
        if (p[i] == NULL)
        {
            fputs("Error memory allocation for *p\n", stderr);
            exit(2);
        }
    }

    SeparateTextToLines(text, p);
    free(text);

    FILE* dfile = fopen(binary_file, "w");
    if (dfile == NULL)
    {
        fputs("Error file open\n", stderr);
        exit(1);
    }
    char data_labels[10][10] = {};
    int adress[10] = {};

    PreIdentity(p, lines, data_labels, adress);
    BeginingIdentity(dfile, p, lines, data_labels, adress);

    for (int i = 0; i < lines; i++)
    {
        free(p[i]);
    }
    fclose(dfile);
    free(p);
}

void FillRegField(char* data, int* i, char* reg)
{
    if (!strcmp(reg, "ax"))
    {
        data[(*i)++] = 1;
    }
    else if (!strcmp(reg, "bx"))
    {
        data[(*i)++] = 2;
    }
    else if (!strcmp(reg, "cx"))
    {
        data[(*i)++] = 3;
    }
}


void SeparateFloatToChar(char* data, int* i, float digit)
{
    types value;
    value.f = digit;
    for (int k = 0; k < sizeof(digit); k++)
    {
        data[(*i)++] = value.b[k];
    }
}

void SeparateIntToChar(char* data, int* i, int digit)
{
    data[(*i)++] = digit & 0xFF;
    data[(*i)++] = (digit >> 8) & 0xFF;
    data[(*i)++] = (digit >> 16) & 0xFF;
    data[(*i)++] = (digit >> 24) & 0xFF;
}


void PreIdentity(char** p, int lines, char data_labels[][10], int adress[])
{
    int n = 0;
    char label[20];
    char one[10] = {};
    char two[10] = {};
    char three[10] = {};
    float dig = 0;
    int i = 0;
    int n_lab = 0;
    for (int k = 0; k < lines; k++)
    {
        if (sscanf(p[k], "%s [%s + %s]", one, two, three) == 3 || sscanf(p[k], "%s %[^,],%f", one, two, &dig) == 3)
        {
            if (!strcmp(one, "push"))
            {
                i += 5;
            }
            else if (!strcmp(one, "pop"))
            {
                i += 9;
            }
            else if (!strcmp(one, "in"))
            {
                i += 6;
            }
        }
        else if (sscanf(p[k], "%s %s + %s", one, two, three) == 3)
        {
            i += 9;
        }
        else if (sscanf(p[k], "%s [%f]", one, &dig) == 2 || sscanf(p[k], "%s %f", one, &dig) == 2)
        {
            if (!strcmp(one, "push") || !strcmp(one, "pop"))
            {
                i += 8;
            }
            else if (!strcmp(one, "jmp"))
            {
                i += 5;
            }
        }
        else if (sscanf(p[k], "%s [%s]", one, two) == 2 || sscanf(p[k], "%s %s", one, two) == 2)
        {
            i += 5;
        }
        else if (sscanf(p[k], "%s", one) == 1)
        {
            sscanf(p[k], "%[^:]:%n", label, &n);
            if (n != 0)
            {
                n = 0;
                int p = 0;
                for (p; p < strlen(label); p++)
                {
                    data_labels[n_lab][p] = label[p];
                }
                data_labels[n_lab][++p] = '\0';
                adress[n_lab] = i;
                n_lab++;
            }
            else if (!strcmp(one, "pop"))
            {
                i += 4;
            }
            else
            {
                i++;
            }
        }

    }
}