#ifndef _FUNC_LEX_H_
#define _FUNC_LEX_H_

#define MAXLEN_LETTER 256
#define MAX_BUFFER 2048 //2KB
#define STACK_SIZE 100
#define STOP -1
#define ERROR -1
#define ID_TABLE_SIZE 1000
typedef struct sta //用来处理{（【 的栈
{
	char data[STACK_SIZE];
	int top;
}SqStack;



void get_nbc(); // 过程，检查C中的字符是否为空格，若是，则反复调用过程get_char，直到C中进入一个非空字符为止。
void get_char(); // 过程，每调用一次，根据forward的指示从buffer中读一个字符，并把它放入变量C中，然后，移动forward，使之指向下一个字符。
void cat(); // 过程，把C中的字符连接在token中的字符串后面。
bool letter();// 布尔函数，判断C中的字符是否为字母， 若是则返回true，否则返回false。
bool digit();// 布尔函数，判断C中的字符是否为数字，若是则返回true，否则返回false。
bool is_digit();//判断token里的是否是实数
void retract();// 过程，向前指针forward后退一个字符。
int reserve();// 函数，根据token中的单词查关键字表，若token中的单词是关键字，则返回值该关键字的记号，否则，返回值“ - 1”。
int SToI();// 过程，将token中的字符串转换成整数。
int SToF();// 过程，将token中的字符串转换成浮点数。
int DTB();// 过程，它将token中的数字串转换成二进制的数值表示。
int table_insert();// 函数，将识别出来的标识符（即token中的单词）插入符号表，返回该单词在符号表中的位置指针。
// error;// 过程，对发现的错误进行相应的处理。
//return;// 过程，将识别出来的单词的记号返回给调用程序。
char* Lex_analysis(); //词法分析程序
void Init();//初始化全局变量的函数
void ReadtoBuffer(char* filename, char* destination, int length);//把待分析的源代码读入缓冲区
#endif
