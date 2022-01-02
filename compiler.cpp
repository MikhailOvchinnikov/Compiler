#include "compiler.h"
//Header from the SortOnegin project
#include "..\..\Sort_Onegin\Sort_Onegin\sort_text.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

#define CommandMask(command, b1, b2, b3)\
data_ptr[i++] = command; data_ptr[i++] = b1;\
data_ptr[i++] = b2; data_ptr[i++] = b3;


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


typedef struct Label{
    char name[LABELS_SIZE] = {};
    int position = 0;
}Label;


void FileLogComp(const char* format, ...){
    static FILE* log_file = fopen("comp_log.txt", "w");
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
}


int ParseLine(char* data_ptr, char** line, Label* labels, int lines){
    char command[COMMAND_SIZE] = {};
    float digit = 0;
    char reg[REG_SIZE] = {};
    int n_lab = 0;
    int i = 0;
    for (int k = 0; k < lines; k++)
    {
        bool flag = 1;
        if (sscanf(line[k], "%s [%s + %f]", command, reg, &digit) == 3) {
            if (!(flag = strcmp(command, "pop"))) {
                if (!data_ptr)
                    i += 9;
                else {
                    CommandMask(Command::POP, 1, 1, 1);
                }
            }
            else if (!(flag = strcmp(command, "push"))) {
                if (!data_ptr)
                    i += 5;
                else {
                    CommandMask(Command::PUSH, 1, 1, 1);
                }
            }
            else if (!(flag = strcmp(command, "out"))) {
                if (!data_ptr)
                    i += 5;
                else {
                    CommandMask(Command::OUT, 1, 1, 1);
                }
            }
            else
                errno = ErrorCode::ERRCOMMAND;
            if (!flag && data_ptr) {
                FillRegField(data_ptr, &i, reg);
                *((float*)&data_ptr[i]) = digit;
                i += sizeof(float);
            }
        }
        else if (sscanf(line[k], "%s %s + %f", command, reg, &digit) == 3)
        {
            if (!data_ptr)
                i += 9;
            else if (!(flag = strcmp(command, "push"))) {
                CommandMask(Command::PUSH, 1, 1, 0);
            }
            else if (!(flag = strcmp(command, "out"))) {
                CommandMask(Command::OUT, 1, 1, 0);
            }
            else
                errno = ErrorCode::ERRCOMMAND;
            if (!flag && data_ptr) {
                FillRegField(data_ptr, &i, reg);
                *((float*)&data_ptr[i]) = digit;
                i += sizeof(float);
            }
        }
        else if (sscanf(line[k], "%s %[^,],%f", command, reg, &digit) == 3)
        {
            if (!data_ptr)
                i += 6;
            else if (!strcmp(command, "mov")) {
                data_ptr[i++] = Command::MOV;
                FillRegField(data_ptr, &i, reg);
                *((float*)&data_ptr[i]) = digit;
                i += sizeof(float);
            }
            else
                errno = ErrorCode::ERRCOMMAND;
        }
        else if (sscanf(line[k], "%s [%f]", command, &digit) == 2)
        {
            if (!data_ptr)
                i += 8;
            else if (!(flag = strcmp(command, "pop"))) {
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
            if (!flag && data_ptr) {
                *((float*)&data_ptr[i]) = digit;
                i += sizeof(float);
            }
        }
        else if (sscanf(line[k], "%s [%s]", command, reg) == 2)
        {
            if (!data_ptr)
                i += 5;
            else if (!(flag = strcmp(command, "pop"))) {
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
            if (!flag && data_ptr)
                FillRegField(data_ptr, &i, reg);
        }
        else if (sscanf(line[k], "%s %f", command, &digit) == 2)
        {
            if (!(flag = strcmp(command, "push"))) {
                if (!data_ptr)
                    i += 8;
                else {
                    CommandMask(Command::PUSH, 0, 1, 0);
                }
            }
            else if (!(flag = strcmp(command, "out"))) {
                if (!data_ptr)
                    i += 8;
                else {
                    CommandMask(Command::OUT, 0, 1, 0);
                }
            }
            else if (!(flag = strcmp(command, "jmp"))) {
                if (!data_ptr)
                    i += 5;
                else {
                    data_ptr[i++] = Command::JMP;
                }
            }
            else
                errno = ErrorCode::ERRCOMMAND;
            if (!flag && data_ptr) {
                *((float*)&data_ptr[i]) = digit;
                i += sizeof(float);
            }
        }
        else if (sscanf(line[k], "%s %s", command, reg) == 2)
        {
            int cndtnl_labl = -1;
            if (!data_ptr)
                i += 5;
            else if (!(flag = strcmp(command, "pop"))) {
                CommandMask(Command::POP, 1, 0, 0);
            }
            else if (!(flag = strcmp(command, "push"))) {
                CommandMask(Command::PUSH, 1, 0, 0);
            }
            else if (!(flag = strcmp(command, "out"))) {
                CommandMask(Command::OUT, 1, 0, 0);
            }
            else if (!(cndtnl_labl = strcmp(command, "jmp")))
                data_ptr[i++] = Command::JMP;
            else if (!(cndtnl_labl = strcmp(command, "ja")))
                data_ptr[i++] = Command::JA;
            else if (!(cndtnl_labl = strcmp(command, "call")))
                data_ptr[i++] = Command::CALL;
            else
                errno = ErrorCode::ERRCOMMAND;
            if (!cndtnl_labl && data_ptr) {
                for (int sp_str = 0; sp_str < LABELS_ARR_SIZE; sp_str++) {
                    if (strlen(labels[sp_str].name) == 0) {
                        FileLogComp("Labels not found\n");
                        errno = ErrorCode::ERRLABEL;
                    }
                    if (!strcmp(reg, labels[sp_str].name)) {
                        *((int*)&data_ptr[i]) = labels[sp_str].position;
                        i += sizeof(int);
                        break;
                    }
                }
            }
            if (!flag && data_ptr)
                FillRegField(data_ptr, &i, reg);
        }
        else if (sscanf(line[k], "%s", command) == 1) {
            char label[LABELS_SIZE] = {};
            int n = 0;
            sscanf(line[k], "%[^:]:%n", label, &n);
            if (!data_ptr && n) {
                strcpy(labels[n_lab].name, label);
                labels[n_lab].position = i;
                n_lab++;
            }
            else if (!strcmp(command, "pop")) {
                if (!data_ptr)
                    i += 4;
                else {
                    CommandMask(Command::POP, 0, 0, 0);
                }
            }
            else if (!strcmp(command, "out")) {
                if (!data_ptr)
                    i += 4;
                else {
                    CommandMask(Command::OUT, 0, 0, 0);
                }
            }
            else if (!data_ptr)
                i++;
            else if (!strcmp(command, "add"))
                data_ptr[i++] = Command::ADD;
            else if (!strcmp(command, "sub"))
                data_ptr[i++] = Command::SUB;
            else if (!strcmp(command, "mul"))
                data_ptr[i++] = Command::MUL;
            else if (!strcmp(command, "div"))
                data_ptr[i++] = Command::DIV;
            else if (!strcmp(command, "sqrt"))
                data_ptr[i++] = Command::SQRT;
            else if (!strcmp(command, "dmp"))
                data_ptr[i++] = Command::DMP;
            else if (!strcmp(command, "hlt"))
                data_ptr[i++] = Command::HLT;
            else if (!strcmp(command, "ret"))
                data_ptr[i++] = Command::RET;
            else if (!strcmp(command, "in"))
                data_ptr[i++] = Command::IN;
            else
                errno = ErrorCode::ERRCOMMAND;
        }
    }
    return i;
}


int Identity(FILE* dfile, char** sp_str, int lines){
    char* data = (char*)calloc(lines * COMMAND_SYMS, sizeof(char));

    if (data == NULL){
        FileLogComp("Error memory allocation for *data\n");
        errno = ErrorCode::ERRMEMORY;
        return -1;
    }

    Label* labels = (Label*)calloc(LABELS_ARR_SIZE, sizeof(Label));
    if (labels == NULL){
        FileLogComp("Error memory allocation for *labels\n");
        errno = ErrorCode::ERRMEMORY;
        return -1;
    }
    ParseLine(NULL, sp_str, labels, lines);
    int size = ParseLine(data, sp_str, labels, lines);
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
    Identity(dfile, sp_str, lines);

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
