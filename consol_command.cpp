#define _CRT_SECURE_NO_WARNINGS
#include "consol_command.h"
#include "..\..\Stack_dinamic\Stack_dinamic\stack.h"
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <stdarg.h>

//Count mask of getted indicator number of register, constant and operate memory
#define MaskCount(r, c, m) r*100 + c*10 + m

#define CMD_Push(stack, expression) Push(stack, expression)

//Actions for arithmetic operation
#define CMD_ADD(stack, val1, val2) CMD_Push(stack, val1 + val2)
#define CMD_SUB(stack, val1, val2) CMD_Push(stack, val2 - val1)
#define CMD_MUL(stack, val1, val2) CMD_Push(stack, val2*val1)
#define CMD_DIV(stack, val1, val2) \
if (val1 == 0){\
    FileLogCMD("Division by zero\n");\
    errno = ErrorCode::DIVISIONERROR;\
} else {\
    CMD_Push(stack, val2 / val1);\
}


#define as_float(arg) *((float*)&(arg))
#define as_int(arg) *((int*)&(arg))


//Define operation and call needed define
#define DEF_CMD(stack, name)\
{\
if (CheckMoreTwoElem(stack)) {\
    float fn = 0;\
    float sn = 0;\
    Pop(stack, &fn);\
    Pop(stack, &sn);\
    CMD_##name(stack, fn, sn);\
} else {\
    FileLogCMD("Operation error! Few elements\n");\
}\
}



void FileLogCMD(const char* format, ...)
{
    static FILE* log_file = fopen("cmd_log.txt", "w");
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
}


bool CheckMoreTwoElem(Stack* stack)
{
    return stack->data->size >= 2;
}


void Sqrt(Stack* stack)
{
    float value = 0;
    Pop(stack, &value);
    float sq = (float)sqrt(value);
    Push(stack, sq);
}


void Out(Stack* stack)
{
    float value = 0;
    Pop(stack, &value);
    FileLogCMD("%f\n", value);
}


char SwitchMaskReg(int mask, char* data, int* ind)
{
    switch (mask)
    {
    case Mask::IOO:
    case Mask::IOI:
    case Mask::III:
    case Mask::IIO:
        return data[++*ind];
    }
    return 0;
}


float SwitchMaskVal(int mask, char* data, int* ind)
{
    switch (mask)
    {
    case Mask::III:
    case Mask::IIO:
    case Mask::OIO:
    case Mask::OII:
    {
        return as_float(data[++ * ind]);
    }
    }
    return 0;
}


int IdentifyData(Stack* stack, char* data, int size, float* memory, float* registers)
{
    if (stack == NULL)
    {
        errno = ErrorCode::NULLPTR;
        return -1;
    }
    Stack* stack_ret = CreateStack("ReturnIndexes");
    for (int i = 0, k = 0; i < size; i++, k++)
    {
        printf("errno %d DATA %d i %d\n", errno, data[i], i);
        int dsplmt = sizeof(float) - 1;
        switch (data[i])
        {
        case Command::PUSH:
            {
                bool reg = data[++i];
                bool constant = data[++i];
                bool operate_memory = data[++i];

                int mask = MaskCount(reg, constant, operate_memory);

                char reg_value = SwitchMaskReg(mask, data, &i);
                float value = SwitchMaskVal(mask, data, &i);

                
                switch (mask)
                {
                case Mask::IOO:
                    Push(stack, registers[reg_value - 1]);
                    break;
                case Mask::IOI:
                {
                    int mem_index = (int)registers[reg_value - 1];
                    Push(stack, memory[mem_index]);
                    break;
                }
                case Mask::III:
                {
                    int mem_index = (int)registers[reg_value - 1] + (int)value;
                    Push(stack, memory[mem_index]);
                    i += dsplmt;
                    break;
                }
                case Mask::IIO:
                    Push(stack, registers[reg_value - 1] + value);
                    i += dsplmt;
                    break;
                case Mask::OIO:
                    Push(stack, value);
                    i += dsplmt;
                    break;
                case Mask::OII:
                    Push(stack, memory[(int)value]);
                    i += dsplmt;
                    break;
                }
                break;
            }
        case Command::POP:
            {
                bool reg = data[++i];
                bool constant = data[++i];
                bool operate_memory = data[++i];

                int mask = MaskCount(reg, constant, operate_memory);

                char reg_value = SwitchMaskReg(mask, data, &i);
                float value = SwitchMaskVal(mask, data, &i);

                switch (mask)
                {
                case Mask::OOO:
                    Pop(stack, NULL);
                case Mask::IOO:
                    Pop(stack, &registers[reg_value - 1]);
                    break;
                case Mask::IOI:
                {
                    int mem_index = (int)registers[reg_value - 1];
                    Pop(stack, &memory[mem_index]);
                    break;
                }
                case Mask::III:
                {
                    int mem_index = (int)registers[reg_value - 1] + (int)value;
                    Pop(stack, &memory[mem_index]);
                    i += dsplmt;
                    break;
                }
                case Mask::OII:
                    Pop(stack, &memory[(int)value]);
                    i += dsplmt;
                    break;
                }
                
                break;
            }
        case Command::OUT:
        {
            bool reg = data[++i];
            bool constant = data[++i];
            bool operate_memory = data[++i];

            int mask = MaskCount(reg, constant, operate_memory);

            char reg_value = SwitchMaskReg(mask, data, &i);
            float value = SwitchMaskVal(mask, data, &i);

            switch (mask)
            {
            case Mask::OOO:
            {
                float stk_val = 0;
                Pop(stack, &stk_val);
                FileLogCMD("%f\n", stk_val);
                break;
            }
            case Mask::IOO:
                FileLogCMD("%f\n", registers[reg_value - 1]);
                break;
            case Mask::OIO:
                FileLogCMD("%f\n", value);
                i += dsplmt;
                break;
            case Mask::IOI:
            {
                int mem_index = (int)registers[reg_value - 1];
                FileLogCMD("%f\n", memory[mem_index]);
                break;
            }
            case Mask::OII:
                FileLogCMD("%f\n", memory[(int)value]);
                i += dsplmt;
                break;
            case Mask::III:
            {
                int mem_index = (int)registers[reg_value - 1] + (int)value;
                FileLogCMD("%f\n", memory[mem_index]);
                i += dsplmt;
                break;
            }
            case Mask::IIO:
            {
                float reg_const_val = registers[reg_value - 1] + value;
                FileLogCMD("%f\n", reg_const_val);
                i += dsplmt;
                break;
            }
            }
            break;
        }
        case Command::ADD:
            {
                DEF_CMD(stack, ADD);
                break;
            }
        case Command::SUB:
            {
                DEF_CMD(stack, SUB);
                break;
            }
        case Command::MUL:
            {
                DEF_CMD(stack, MUL);
                break;
            }
        case Command::DIV:
            {
                DEF_CMD(stack, DIV);
                if (errno)
                {
                    return -1;
                }
                break;
            }
        case Command::SQRT:
            {
                Sqrt(stack);
                break;
            }
        case Command::DMP:
            {
                DumpStk(stack);
                break;
            }
        case Command::HLT:
            {
                FileLogCMD("The end of the programm\n");
                return -1;
            }
        case Command::MOV:
            {
                char reg_value = data[++i];
                registers[reg_value - 1] = as_float(data[++i]);
                i += dsplmt;
                break;
            }
        case Command::JMP:
            {
                i = as_int(data[++i]) - 1;
                break;
            }
        case Command::JA:
            {
                int ja_comm = as_int(data[++i]);
                float f_value = 0;
                float s_value = 0;
                Pop(stack, &f_value);
                Pop(stack, &s_value);
                if (f_value > s_value)
                {
                    i = ja_comm - 1;
                }
                else
                {
                    i += dsplmt;
                }
                break;
            }
        case Command::CALL:
            {
                Push(stack_ret, (float)i);
                i = (int)data[++i] - 1;
                break;
            }
        case Command::RET:
            {
                float fn = 0;
                Pop(stack_ret, &fn);
                i = (int)fn + dsplmt + 1;
                break;
            }
        case Command::IN:
        {
            float inval = 0;
            scanf("%f", &inval);
            Push(stack, inval);
            break;
        }
        default:
            {
                FileLogCMD("Unknown command %d\n", data[i]);
                errno = ErrorCode::COMMANDERROR;
                return -1;
            }
        }
    }
    return 0;
}

int GetParametersFileInt(FILE* file, int* text, int syms)
{
    if (text == NULL)
    {
        FileLogCMD("Error memory allocation for *text\n");
        errno = ErrorCode::MEMORYERROR;
        return 0;
    }
    if (fread(text, sizeof(int), syms, file) != syms)
    {
        FileLogCMD("Error reading");
        errno = ErrorCode::FILEERROR;
        return 0;
    }
    int lines = 0;
    for (int i = 0; i < syms; i++)
    {
        lines += text[i] == '\n';
    }
    lines++;

    return lines;
}

