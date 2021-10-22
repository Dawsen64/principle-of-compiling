#ifndef _FUNC_LEX_H_
#define _FUNC_LEX_H_

#include <stdbool.h>

#define MAXLEN_LETTER 256
#define MAX_BUFFER 2048 //2KB
#define STOP -1
#define ERROR -2
#define ID_TABLE_SIZE 1000
#define FILENAME "input.txt"
typedef struct counter //统计源程序中的各种字符数量
{
	int lines;//行数
	int char_num;//字符总数
	int keyword_num;//关键字数量
	int id_num;//标识符数量
	int digit_num;//数字总数
	int dilimiter_num;//分界符数量
	int operator_num;//运算符数量
}Counter;



void get_nbc(); // 检查C中的字符是否为空格，若是，则反复调    用过程get_char，直到C中进入一个非空字符为止
void get_char(); // 每调用一次，根据forward的指示从buffer中    读一个字符，并把它放入变量C中，然后，移动forward，使之    指向下一个字符
void cat(); // 把C中的字符连接在token中的字符串后面
bool letter();//判断C中的字符是否为字母
bool digit();// 判断C中的字符是否为数字，
bool is_digit();//判断token中的是否为数字
void retract();// forward回退一个
int reserve();// 判断是否为保留字
int SToI();// 
int SToF();//
int DTB();// 
void table_insert();// 
// error;// 
//return;// 
char* Lex_analysis(); //�
void Init();// 初始化全局变量
void ReadtoBuffer(const char* filename, char* destination, int length);// 把文件读入缓冲区
void Print_num();//输出各种统计数量
#endif
