#include "func_lex.h"


/*********全局变量************/

int state; //整型变量，当前状态指示。
char C; //字符变量，存放当前读入的字符。
char token[MAXLEN_LETTER];// 字符数组，存放当前正在识别的单词字符串。
char buffer[MAX_BUFFER]; // 字符数组，输入缓冲区。
int forward;// 字符指针，向前指针。
int lexemebegin; // 字符指针，指向buffer中当前单词的开始位置。
int iskey; // 整型变量，值为 - 1，表示识别出的单词是用户自定义标识符，否则，表示识别出的单词是关键字，其值为关键字的记号。

char* Lex_analysis()
{
    state = 0;
    switch (state) {
    case 0:      // 初始状态
        token[0] = '\0';
        get_char();
        get_nbc();
        switch (C) {
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
        case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v':
        case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K':
        case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
        case 'W': case 'X': case 'Y': case 'Z':
            state = 1; break;      //设置标识符状态
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            state = 2; break;      //设置常数状态
        case '<': state = 8; break;      //设‘<’状态
        case '>': state = 9; break;      //设置‘>’状态
        case ':': state = 10; break;      //设置‘：’状态
        case '/': state = 11; break;      //设置‘/’状态
        case '=': state = 0; return('EQ'); break;      //返回‘=’的记号
        case '+': state = 0; return('+'); break;              //返回‘+’的记号
        case '-': state = 0; return('-'); break;              //返回‘-’的记号
        case '*': state = 0; return('*'); break;               //返回‘*’的记号
        case '(': state = 0; return('('); break;                //返回‘（’的记号
        case ')': state = 0; return(')'); break;               //返回‘）’的记号
        case ';': state = 0; return(';'); break;               //返回‘；’的记号
        case '\'': state = 0; return('\''); break;              //返回‘′’的记号
        default: state = 13; break;      //设置错误状态
        };
        break;
    case 1:      // 标识符状态
        cat();
        get_char();
        if (letter() || digit())  state = 1;
        else {
            retract();
            state = 0;
            iskey = reserve();    // 查关键字表
            if (iskey != -1) return ("iskey");    // 识别出的是关键字
            else {    // 识别出的是用户自定义标识符
                identry = table_insert();     // 返回该标识符在符号表的入口指针
                return(ID, identry);
            };
        };
        break;
    }