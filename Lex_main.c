#include <stdio.h>
#include "func_lex.h"
extern char buffer[MAX_BUFFER];
extern int flen;



int main()
{
	Init();
	Lex_analysis();
	Print_num();
	printf("��ӡbuffer����ַ�:\n");
	printf("%s\n", buffer);

	return 0;
}