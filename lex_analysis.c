#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>

#include "func_lex.h"
/*********全局变量************/

int state; //整型变量，当前状态指示
char C; //字符变量，存放当前读入的字符。

char keywords[34][20] = { "auto","break","case","char","const","continue",
"default","do","double","else","enum","extern","float","for","goto",
"if","int","long ","register","return","short","signed","static",
"sizeof","struct","switch","typedef","unionunsigned","void","volatile","while","include","define"};//关键字加上了include,define

char token[MAXLEN_LETTER];// 字符数组，存放当前正在识别的单词字符串。
short tokenptr = 0;//指向token的‘\0’字符
char buffer[MAX_BUFFER]; // 
int forward;// 
int lexemebegin; // 
int iskey; // 
char Id_Table[ID_TABLE_SIZE][MAXLEN_LETTER];//
int id_length;
int flen = 0;
Counter counter;
void Init()//
{
    //初始化buffer
    buffer[MAX_BUFFER/2 - 1] = EOF;
    buffer[MAX_BUFFER - 1] = EOF;

    ReadtoBuffer(FILENAME, buffer, MAX_BUFFER / 2 - 1);
    forward = 0;
    id_length = 0;

}

char* Lex_analysis()
{
    state = 0;
    while (state != STOP)
    {
        switch (state)
        {
        case 0:      // 
            tokenptr = 0;
            get_char();
            get_nbc();
            switch (C) {
            case '_':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
            case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v':
            case 'w': case 'x': case 'y': case 'z':
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K':
            case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
            case 'W': case 'X': case 'Y': case 'Z':
                state = 1;//标识符或关键字
                break;     
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 2;
                break;      //数字
            case '<':
                state = 3;
                break;      //<,<=, <<
            case '>':
                state = 4;
                break;      //>, >=, >>
            case '=': //==
                state = 5;
                break;     
            case '+':
                state = 6;
                break;              
            case '-':
                state = 7;
                break;            
            case '*':
                state = 8;
                break;               
            case '/': //设置/，//，/*
                state = 9;
                break;   
            case '\'':
                state = 10;
                break;
            case '\"':
                state = 11;
            case '&': //&,&&
                state = 12;
                break;
            case '|'://|,||
                state = 13;
                break;
            case '!'://!或!=
                state = 14;
                break;
            case '#'://处理#include,#define
                state = 15;
                break;
            case ';':
            case '(': case ')': case '[': case ']': case '{': case '}':
            case ',': case '.': case '?': case ':': case '%': case '^': case '~':
                state = 16;
                break;
            case EOF:
                state = STOP; //字符结尾，状态机准备结束
                break;
            default:
                state = ERROR;//错误状态
                break;      
            };
            break;
        case 1:      // 处理标识符id
            cat();
            get_char();
            if (letter() || digit())
                state = 1;
            else
            {
                retract();
                state = 0;
                iskey = reserve();    // 查找关键字

                if (iskey == 1)  //
                {
                    counter.keyword_num++;
                    printf("<%s, ->\n", token);
                }
                else {    // 标识符
                    table_insert();     
                    counter.id_num++;
                    printf("<id, %s>\n", token);
                };
            };
            break;
        case 2: //数字
            cat();
            get_char();
            if (digit())
                state = 2;
            else
            {
                retract();
                state = 0;

                if (is_digit())
                {
                    counter.digit_num++;
                    printf("<num, %s>\n", token);
                }
                else
                {
                    state = ERROR;//
                }
            }
            break;
        case 3: //设置“<,<=,<<”
            get_char();
            state = 0;
            if (C == '=')
                printf("<<=, ->\n");
            else if (C == '<')
                printf("<<<, ->\n");
            else
            {
                retract();
                printf("<<, ->\n");
            }
            counter.operator_num++;
            break;
        case 4: //设置“>, >=, >>”
            get_char();
            state = 0;
            if (C == '=')
                printf("<>=, ->\n");
            else if (C == '>')
                printf("<>>, ->\n");
            else
            {
                retract();
                printf("<>, ->\n");
            }
            counter.operator_num++;
            break;
        case 5: //设置“==”
            get_char();
            state = 0;
            if (C == '=')
                printf("<==, ->\n");
            else
            {
                retract();
                printf("<=, ->\n");
            }
            counter.operator_num++;
            break;
        case 6: //设置“+，++”
            get_char();
            state = 0;
            if (C == '+')
                printf("<++, ->\n");
            else
            {
                retract();
                printf("<+, ->\n");
            }
            counter.operator_num++;
            break;
        case 7: //设置“-，--”
            get_char();
            if (C == '-')
                printf("<--, ->\n");
            else
            {
                retract();
                printf("<-, ->\n");
            }
            counter.operator_num++;
            state = 0;
            break;
        case 8: //设置"*"
            printf("<*, ->\n");
            counter.operator_num++;
            state = 0;
            break;
        case 9: //处理/和注释/**/， //
            get_char();
            if (C == '/')//
            {
                get_char();
                while (C != '\n')
                {
                    get_char();
                }
            }
            else if (C == '*') ///*...*/'
            {
                get_char();
                while (state != 0)
                {
                    if (C == '*')
                    {
                        get_char();
                        if (C == '/')
                        {
                            state = 0;
                            break;
                        }
                        retract();
                    }
                    get_char();
                }
            }
            else
            {
                printf("</, ->\n");
                counter.operator_num++;
            }
            state = 0;
            break;
        case 10://设置'字符状态
            get_char();
            while (C != '\'')
            {
                cat();
                get_char();
            }
            printf("<const char, '%s'>\n", token);
            state = 0;
            break;
        case 11://设置“字符串状态
            get_char();
            while (C != '\"')
            {
                cat();
                get_char();
            }
            printf("<const char[], \"%s\">\n", token);
            state = 0;
            break;
        case 12: //设置 &,&&;
            get_char();
            if (C == '&')
                printf("<&&, ->\n");
            else
            {
                retract();
                printf("<&, ->\n");
            }
            counter.operator_num++;
            state = 0;
            break;
        case 13: //设置|,||
            get_char();
            if (C == '|')
                printf("<||, ->\n");
            else
            {
                retract();
                printf("<|, ->\n");
            }
            counter.operator_num++;
            state = 0;
            break;
        case 14: //设置!, !=
            get_char();
            if (C == '=')
                printf("<!=, ->\n");
            else
            {
                retract();
                printf("<!, ->\n");
            }
            counter.operator_num++;
            state = 0;
            break;
        case 15: //#include, #define
            get_nbc();
            get_char();
            char s[2][10] = { "include", "define" };
            if (C == 'i')
            {
                cat();
                get_char();
                while (letter())
                {
                    cat();
                    get_char();
                }
                if (!strcmp(s[0], token))
                    printf("<id, %s>\n", s[0]);
                else state = ERROR;

            }
            else if (C == 'd')
            {
                cat();
                get_char();
                while (letter())
                {
                    cat();
                    get_char();
                }
                retract();
                if (!strcmp(s[1], token))
                    printf("<id, %s>\n", s[1]);
                else state = ERROR;
            }
            else state == ERROR;

            state = 0;
            break;
        case 16: //各种其余单个符号
            printf("<%c, ->\n", C);
            if (C == ';')
                counter.dilimiter_num++;
            else counter.operator_num++;
            state = 0;
            break;
        default: 
            printf("Warning! This line exit error!\n");
            state = 0;


        }
    }
    counter.char_num = flen;
    Print_num();
}


void ReadtoBuffer(const char* filename, char* destination, int length)
{
    //把源代码读入缓冲区
    FILE* fp = fopen(filename, "r");
    int i = 0;
    if (fp == NULL)
        printf("Open C soure error!\n");
    else
    {
        fseek(fp, flen, SEEK_SET);
        destination[i] = fgetc(fp);
        while (i < length && destination[i] != EOF)
        {
            i++;
            destination[i] = fgetc(fp);
        }
    }
    flen = ftell(fp);
    fclose(fp);
}
//每调用一次，根据forward的指示从buffer中    读一个字符，并把它放入变量C中，然后，移动forward，使之    指向下一个字符。
void get_char()
{
    C = buffer[forward++];
    if (C == '\n')
    {
        counter.lines++;
    }
    //printf("%c", C);
    if (buffer[forward] == EOF)
    {
        if (forward == MAX_BUFFER / 2 - 1)//
        {
            ReadtoBuffer(FILENAME, buffer + forward + 1, MAX_BUFFER / 2 - 1);
            forward++;
        }
        else if (forward == MAX_BUFFER - 1)
        {
            ReadtoBuffer(FILENAME, buffer, MAX_BUFFER / 2 - 1);
            forward = 0;
        }
    }

}
void cat() // 把C中的字符连结到token上
{
    token[tokenptr++] = C;
    token[tokenptr] = '\0';

}
bool letter()// 布尔函数，判断C中的字符是否为字母，    若是则返回true，否则返回false
{
    if (('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z') || C == '_')
    {
        return true;
    }
    return false;
}
bool digit()// 布尔函数，判断C中的字符是否为数字，    若是则返回true，否则返回false。
{
    if (('0' <= C && C <= '9') || C == '.' || C == 'E' || C == 'e')
        return true;
    return false;
    
}
void retract()// 向前指针forward后退一个字符
{
    if (forward == 0)//
    {
        forward = MAX_BUFFER - 2;
    }
    else if (forward == MAX_BUFFER / 2)
    {
        forward = MAX_BUFFER / 2 - 2;
    }
    else forward--;
}
int reserve()// 查找是否是关键字
{
    
    int i = 0;
    for (i = 0; i < 32; i++)
    {
        if(!strcmp(token, keywords[i]))
            return 1;
    }
    return -1;
}
void table_insert()// 
{
    strcpy(Id_Table[id_length++], token);
}
bool is_digit()
{
    short point = 0, e_number = 0, e_flag = 0;
    short i = 0;
    while (token[i] != '\0')
    {
        if (token[i] == '.')
            point++;
        else if (token[i] == 'e' || token[i] == 'E')
        {
            e_number++;
            e_flag = 1;
        }
        
        if (e_flag == 1 && ('0' <= token[i] && token[i] <= '9'))
            e_flag = 1;
        else e_flag = 0;
        
        if (point > 1 && e_number > 1 && e_flag == 0)
            return false;
        i++;
    }
    return true;
}
void get_nbc() //跳过空白符
{
    while (C == ' ' || C == '\n' || C == '\t')
    {
        get_char();
    }
}
void Print_num()//输出各种统计数量
{
    printf("行数:%d\n", counter.lines);
    printf("字符总数:%d\n", counter.char_num);
    printf("关键字总数:%d\n", counter.keyword_num);
    printf("标识符数量:%d\n", counter.id_num);
    printf("数字:%d\n", counter.digit_num);
    printf("分界符:%d\n", counter.dilimiter_num);
    printf("运算符:%d\n", counter.operator_num);
}