#include <stdio.h>
#include <string.h>

#include "func_lex.h"
/*********全局变量************/

int state; //整型变量，当前状态指示。
char C; //字符变量，存放当前读入的字符。
char token[MAXLEN_LETTER];// 字符数组，存放当前正在识别的单词字符串。
char buffer[MAX_BUFFER]; // 字符数组，输入缓冲区。2KB大小
int forward;// 字符指针，向前指针。
int lexemebegin; // 字符指针，指向buffer中当前单词的开始位置。
int iskey; // 整型变量，值为 - 1，表示识别出的单词是用户自定义标识符，否则，表示识别出的单词是关键字，其值为关键字的记号。
SqStack stack; //用来处理“{([ ”的栈
char Id_Table[ID_TABLE_SIZE][MAXLEN_LETTER];//存放id符号表
int id_length;
void Init()//初始化全局变量
{
    //初始化buffer
    buffer[MAX_BUFFER/2 - 1] = EOF;
    buffer[MAX_BUFFER - 1] = EOF;

    forward = 0;
    id_length = 0;
    stack.data[0] = '\0';
    stack.top = 0;

}

char* Lex_analysis()
{
    state = 0;
    while (state != STOP)
    {
        switch (state) 
        {
        case 0:      // 初始状态
            token[0] = '\0';
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
                state = 1; 
                break;      //设置标识符状态
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 2; 
                break;      //设置常数状态
            case '<':
                state = 3; 
                break;      //设‘<’状态
            case '>':
                state = 4; 
                break;      //设置‘>’状态
            case '=':
                state = 5; 
                break;      //设置‘=’状态
            case '+':
                state = 6; 
                break;              //返回‘+’的记号
            case '-':
                state = 7; 
                break;              //返回‘-’的记号
            case '*':
                state = 8;  
                break;               //返回‘*’的记号
            case '/':
                state = 9;  
                break;               //返回‘/’的记号
            case '(':
                state = 10;  
                break;                //返回‘（’的记号
            case ')':
                state = 11; 
                break;               //返回‘）’的记号
            case ';':
                state = 12; 
                break;               //返回‘；’的记号
            default: state = 13; break;      //设置错误状态
            };
            break;
        case 1:      // 标识符状态
            cat();
            get_char();
            if (letter() || digit())
                state = 1;
            else
            {
                retract();
                state = 0;
                iskey = reserve();    // 查关键字表

                if (iskey != -1)  // 识别出的是关键字
                    printf("<%s, ->", token);    
                else {    // 识别出的是用户自定义标识符
                    table_insert();     // 该标识符插入符号表
                    printf("<id, %s>", token);
                };
            };
            break;
        case 2: //常数状态
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
                    printf("<num, %s>", token);
                }
                else
                {
                    state == ERROR;//表示数字出错了
                }
            }
            break;
        case 3: //设置‘<’状态
            get_char();
            if (C == '=')
                printf("<relop, LE>\n");
            else
            {
                retract();
                state = 0;

                printf("<relop, LT>\n");
            }
            break;
        case 4: //设置‘>’状态
            get_char();
            if (C == '=')
                printf("<relop, GE>\n");
            else
            {
                retract();
                state = 0;

                printf("<relop, GT>\n");
            }
            break;
        }
    }
}

//把待分析的源代码读入缓冲区
void ReadtoBuffer(char* filename, char* destination, int length)
{
    //destination是目标缓冲区，length是最大读入长度。
    FILE* fp = fopen(filename, "r");
    int i = 0;
    if (fp == NULL)
        printf("Open C soure error!\n");
    else
    {
        destination[i] = fgetc(fp);
        while (i < length && destination[i] != EOF)
        {
            i++;
            destination[i] = fgetc(fp);
        }
    }
    fclose(fp);
}
// 过程，每调用一次，根据forward的指示从buffer中读一个字符，并把它放入变量C中，然后，移动forward，使之指向下一个字符。
void get_char() 
{
    char filename[20] = "input.txt";
    C = buffer[forward++];
    if (buffer[forward] == EOF)
    {
        if (forward == MAX_BUFFER / 2 - 1)//指针在左半区eof
        {
            ReadtoBuffer(filename, buffer + forward + 1, MAX_BUFFER / 2 - 1);
            forward++;
        }
        else if (forward == MAX_BUFFER - 1)
        {
            ReadtoBuffer(filename, buffer, MAX_BUFFER / 2 - 1);
            forward = 0;
        }
        else //到达文件结尾
        {
            state = 13;
        }
    }

}
void cat() // 过程，把C中的字符连接在token中的字符串后面。
{
    int i = 0;
    while (token[i] != '\0') 
    { 
        i++;
    }
    token[i++] = C;

}
bool letter()// 布尔函数，判断C中的字符是否为字母或_， 若是则返回true，否则返回false。
{
    if (('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z') || C == '_')
    {
        return true;
    }
    return false;
}
bool digit()// 布尔函数，判断C中的字符是否为数字，若是则返回true，否则返回false。
{
    if (('0' <= C && C <= '9') || C == '.' || C == 'E' || C == 'e')
        return true;
    return false;
    
}
void retract()// 过程，向前指针forward后退一个字符。
{
    if (forward == 0)//退回到buffer末尾
    {
        forward = MAX_BUFFER - 2;
    }
    else if (forward == MAX_BUFFER / 2)
    {
        forward = MAX_BUFFER / 2 - 2;
    }
    else forward--;
}
int reserve()// 函数，根据token中的单词查关键字表，若token中的单词是关键字，则返回值该关键字的记号，否则，返回值“ - 1”。
{
    char keytable[10][32] = { "if", "else", "switch", "case" };
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        if(!strcmp(token, keytable[i]))
            return 1;
        else return -1;
    }
}
int table_insert()// 函数，将识别出来的标识符（即token中的单词）插入符号表，返回该单词在符号表中的位置指针。
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
}