#include "compiler.h"
//Header from the SortOnegin project
#include "..\..\Sort_Onegin\Sort_Onegin\sort_text.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

#define CommandMask(command, b1, b2, b3)\
data[i++] = command; data[i++] = b1;\
data[i++] = b2; data[i++] = b3;


//Max size of CMD command
const int COMMAND_SIZE = 10;

//Max number of bytes in command bytes definition
const int COMMAND_SYMS = 20;

//Max size of register name
const int REG_SIZE = 10;

//Max size of labels array
const int LABELS_ARR_SIZE = 10;

//Max size of label name
const int LABELS_SIZE = 10;


void FileLogComp(const char* format, ...)
{
    static FILE* log_file = fopen("comp_log.txt", "w");
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
}


int BeginingIdentity(FILE* dfile, char** sp_str, int lines, char data_labels[][10], int adress[])
{
    char* data = (char*)calloc(lines * COMMAND_SYMS, sizeof(char));

    if (data == NULL)
    {
        FileLogComp("Error memory allocation for *data\n");
        errno = ErrorCode::ERRMEMORY;
        return -1;
    }
    char command[COMMAND_SIZE] = {};
    float digit = 0;
    char reg[REG_SIZE] = {};
    int size = 0;
    for (int i = 0, k = 0; k < lines; k++)
    {
        bool flag = 1;
        if (sscanf(sp_str[k], "%s [%s + %f]", command, reg, &digit) == 3){
            if (!(flag = strcmp(command, "pop"))) {
                CommandMask(Command::POP, 1, 1, 1);
            }
            else if (!(flag = strcmp(command, "push"))) {
                CommandMask(Command::PUSH, 1, 1, 1);
            }
            else if (!(flag = strcmp(command, "out"))) {
                CommandMask(Command::OUT, 1, 1, 1);
            }
            else
                errno = ErrorCode::ERRCOMMAND;
            if (!flag) {
                FillRegField(data, &i, reg);
                *((float*)&data[i]) = digit;
                i += sizeof(float);
            }
        }
        else if (sscanf(sp_str[k], "%s %s + %f", command, reg, &digit) == 3)
        {
            if (!(flag = strcmp(command, "push"))) {
                CommandMask(Command::PUSH, 1, 1, 0);
            }
            else if (!(flag = strcmp(command, "out"))) {
                CommandMask(Command::OUT, 1, 1, 0);
            }
            else
                errno = ErrorCode::ERRCOMMAND;
            if (!flag) {
                FillRegField(data, &i, reg);
                *((float*)&data[i]) = digit;
                i += sizeof(float);
            }
        }
        else if (sscanf(sp_str[k], "%s %[^,],%f", command, reg, &digit) == 3)
        {
            if (!strcmp(command, "mov")) {
                data[i++] = Command::MOV;
                FillRegField(data, &i, reg);
                *((float*)&data[i]) = digit;
                i += sizeof(float);
            }
            else
                errno = ErrorCode::ERRCOMMAND;
        }
        else if (sscanf(sp_str[k], "%s [%f]", command, &digit) == 2)
        {
            if (!(flag = strcmp(command, "pop"))) {
                CommandMask(Command::POP, 0, 1, 1);
            }
            else if (!(flag = strcmp(command, "push"))) {
                CommandMask(Command::PUSH, 0, 1, 1);
            }
            else if (!(flag = strcmp(command, "out"))) {
                CommandMask(Command::OUT, 0, 1, 1);
            }
            else
                errno = ErrorCode::ERRCOMMAND;
            if (!flag) {
                *((float*)&data[i]) = digit;
                i += sizeof(float);
            }
        }
        else if (sscanf(sp_str[k], "%s [%s]", command, reg) == 2)
        {
            if (!(flag = strcmp(command, "pop"))) {
                CommandMask(Command::POP, 1, 0, 1);
            }
            else if (!(flag = strcmp(command, "push"))) {
                CommandMask(Command::PUSH, 1, 0, 1);
            }
            else if (!(flag = strcmp(command, "out"))) {
                CommandMask(Command::OUT, 1, 0, 1);
            }
            else
                errno = ErrorCode::ERRCOMMAND;
            if (!flag)
                FillRegField(data, &i, reg);
        }
        else if (sscanf(sp_str[k], "%s %f", command, &digit) == 2)
        {
            if (!(flag = strcmp(command, "push"))) {
                CommandMask(Command::PUSH, 0, 1, 0);
            }
            else if (!(flag = strcmp(command, "out"))) {
                CommandMask(Command::OUT, 0, 1, 0);
            }
            else if (!(flag = strcmp(command, "jmp"))) {
                data[i++] = Command::JMP;
            }
            else
                errno = ErrorCode::ERRCOMMAND;
            if (!flag) {
                *((float*)&data[i]) = digit;
                i += sizeof(float);
            }
        }
        else if (sscanf(sp_str[k], "%s %s", command, reg) == 2)
        {
            int cndtnl_labl = -1;
            if (!(flag = strcmp(command, "pop"))){
                CommandMask(Command::POP, 1, 0, 0);
            }
            else if (!(flag = strcmp(command, "push"))){
                CommandMask(Command::PUSH, 1, 0, 0);
            }
            else if (!(flag = strcmp(command, "out"))) {
                CommandMask(Command::OUT, 1, 0, 0);
            }
            else if (!(cndtnl_labl = strcmp(command, "jmp")))
                data[i++] = Command::JMP;
            else if (!(cndtnl_labl = strcmp(command, "ja")))
                data[i++] = Command::JA;
            else if (!(cndtnl_labl = strcmp(command, "call")))
                data[i++] = Command::CALL;
            else
                errno = ErrorCode::ERRCOMMAND;
            if (!cndtnl_labl){
                for (int sp_str = 0; sp_str < LABELS_ARR_SIZE; sp_str++){
                    if (strlen(data_labels[sp_str]) == 0){
                        FileLogComp("Labels not found\n");
                        errno = ErrorCode::ERRLABEL;
                    }
                    if (!strcmp(reg, data_labels[sp_str])){
                        *((int*)&data[i]) = adress[sp_str];
                        i += sizeof(int);
                        break;
                    }
                }
            }
            if (!flag)
                FillRegField(data, &i, reg);
        }
        else if (sscanf(sp_str[k], "%s", command) == 1){
            if (!strcmp(command, "pop")){
                CommandMask(Command::POP, 0, 0, 0);
            }
            else if (!strcmp(command, "out")){
                CommandMask(Command::OUT, 0, 0, 0);
            }
            else if (!strcmp(command, "add"))
                data[i++] = Command::ADD;
            else if (!strcmp(command, "sub"))
                data[i++] = Command::SUB;
            else if (!strcmp(command, "mul"))
                data[i++] = Command::MUL;
            else if (!strcmp(command, "div"))
                data[i++] = Command::DIV;
            else if (!strcmp(command, "sqrt"))
                data[i++] = Command::SQRT;
            else if (!strcmp(command, "dmp"))
                data[i++] = Command::DMP;
            else if (!strcmp(command, "hlt"))
                data[i++] = Command::HLT;
            else if (!strcmp(command, "ret"))
                data[i++] = Command::RET;
            else if (!strcmp(command, "in"))
                data[i++] = Command::IN;
            else
                errno = ErrorCode::ERRCOMMAND;
        }
        if (errno){
            int n = 0;
            sscanf(sp_str[k], "%[^:]:%n", command, &n);
            if (!n) {
                FileLogComp("Wrong command was getted \"%s\"\n", command);
                return -1;
            }
            else
                lines--;
            errno = 0;
        }
        size = i;
    }
    fwrite(data, sizeof(char), size, dfile);
    free(data);
    return 0;
}

int CompilingFile(char* file_name, char* binary_file){
    FILE* file = fopen(file_name, "rb");
    if (file == NULL){
        FileLogComp("Error file open\n");
        errno = ErrorCode::ERRFILE;
        return -1;
    }

    int syms = CountSymbols(file);
    char* text = (char*)calloc((syms + 1), sizeof(char));

    int lines = GetParametersFile(file, text, syms);
    fclose(file);

    char** sp_str = (char**)calloc(lines, sizeof(char*));
    if (sp_str == NULL){
        FileLogComp("Error memory allocation for **sp_str\n");
        errno = ErrorCode::ERRMEMORY;
        return -1;
    }
    for (int i = 0; i < lines; i++){
        sp_str[i] = (char*)calloc(100, sizeof(char));
        if (sp_str[i] == NULL){
            FileLogComp("Error memory allocation for *sp_str\n");
            errno = ErrorCode::ERRMEMORY;
            return -1;
        }
    }

    SeparateTextToLines(text, sp_str);
    free(text);

    FILE* dfile = fopen(binary_file, "w");
    if (dfile == NULL){
        FileLogComp("Error file open\n");
        errno = ErrorCode::ERRFILE;
        return -1;
    }
    char data_labels[LABELS_ARR_SIZE][LABELS_SIZE] = {};
    int adress[LABELS_ARR_SIZE] = {};

    PreIdentity(sp_str, lines, data_labels, adress);
    BeginingIdentity(dfile, sp_str, lines, data_labels, adress);

    for (int i = 0; i < lines; i++)
        free(sp_str[i]);

    fclose(dfile);
    free(sp_str);
    return 0;
}

void FillRegField(char* data, int* i, char* reg){
    if (!strcmp(reg, "ax"))
        data[(*i)++] = 1;
    else if (!strcmp(reg, "bx"))
        data[(*i)++] = 2;
    else if (!strcmp(reg, "cx"))
        data[(*i)++] = 3;
}


void PreIdentity(char** sp_str, int lines, char data_labels[][10], int adress[]){
    int n = 0;
    char label[LABELS_SIZE];
    char arg_1[COMMAND_SIZE] = {};
    char arg_2[COMMAND_SIZE] = {};
    char arg_3[COMMAND_SIZE] = {};
    float dig = 0;
    int i = 0;
    int n_lab = 0;
    for (int k = 0; k < lines; k++){
        if (sscanf(sp_str[k], "%s [%s + %s]", arg_1, arg_2, arg_3) == 3 || sscanf(sp_str[k], "%s %[^,],%f", arg_1, arg_2, &dig) == 3){
            if (!strcmp(arg_1, "push") || !strcmp(arg_1, "out"))
                i += 5;
            else if (!strcmp(arg_1, "pop"))
                i += 9;
            else if (!strcmp(arg_1, "mov"))
                i += 6;
        }
        else if (sscanf(sp_str[k], "%s %s + %s", arg_1, arg_2, arg_3) == 3)
            i += 9;
        else if (sscanf(sp_str[k], "%s [%f]", arg_1, &dig) == 2 || sscanf(sp_str[k], "%s %f", arg_1, &dig) == 2){
            if (!strcmp(arg_1, "push") || !strcmp(arg_1, "pop") || !strcmp(arg_1, "out"))
                i += 8;
            else if (!strcmp(arg_1, "jmp"))
                i += 5;
        }
        else if (sscanf(sp_str[k], "%s [%s]", arg_1, arg_2) == 2 || sscanf(sp_str[k], "%s %s", arg_1, arg_2) == 2)
            i += 5;
        else if (sscanf(sp_str[k], "%s", arg_1) == 1){
            sscanf(sp_str[k], "%[^:]:%n", label, &n);
            if (n != 0){
                n = 0;
                int sp_str = 0;
                for (sp_str; sp_str < strlen(label); sp_str++)
                    data_labels[n_lab][sp_str] = label[sp_str];
                data_labels[n_lab][++sp_str] = '\0';
                adress[n_lab] = i;
                n_lab++;
            }
            else if (!strcmp(arg_1, "pop") || !strcmp(arg_1, "out"))
                i += 4;
            else
                i++;
        }

    }
}