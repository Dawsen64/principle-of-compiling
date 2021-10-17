#include <stdio.h>
#include <string.h>

#include "func_lex.h"
/*********ȫ�ֱ���************/

int state; //���ͱ�������ǰ״ָ̬ʾ��
char C; //�ַ���������ŵ�ǰ������ַ���
char token[MAXLEN_LETTER];// �ַ����飬��ŵ�ǰ����ʶ��ĵ����ַ�����
char buffer[MAX_BUFFER]; // �ַ����飬���뻺������2KB��С
int forward;// �ַ�ָ�룬��ǰָ�롣
int lexemebegin; // �ַ�ָ�룬ָ��buffer�е�ǰ���ʵĿ�ʼλ�á�
int iskey; // ���ͱ�����ֵΪ - 1����ʾʶ����ĵ������û��Զ����ʶ�������򣬱�ʾʶ����ĵ����ǹؼ��֣���ֵΪ�ؼ��ֵļǺš�
SqStack stack; //��������{([ ����ջ
char Id_Table[ID_TABLE_SIZE][MAXLEN_LETTER];//���id���ű�
int id_length;
void Init()//��ʼ��ȫ�ֱ���
{
    //��ʼ��buffer
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
        case 0:      // ��ʼ״̬
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
                break;      //���ñ�ʶ��״̬
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 2; 
                break;      //���ó���״̬
            case '<':
                state = 3; 
                break;      //�衮<��״̬
            case '>':
                state = 4; 
                break;      //���á�>��״̬
            case '=':
                state = 5; 
                break;      //���á�=��״̬
            case '+':
                state = 6; 
                break;              //���ء�+���ļǺ�
            case '-':
                state = 7; 
                break;              //���ء�-���ļǺ�
            case '*':
                state = 8;  
                break;               //���ء�*���ļǺ�
            case '/':
                state = 9;  
                break;               //���ء�/���ļǺ�
            case '(':
                state = 10;  
                break;                //���ء������ļǺ�
            case ')':
                state = 11; 
                break;               //���ء������ļǺ�
            case ';':
                state = 12; 
                break;               //���ء������ļǺ�
            default: state = 13; break;      //���ô���״̬
            };
            break;
        case 1:      // ��ʶ��״̬
            cat();
            get_char();
            if (letter() || digit())
                state = 1;
            else
            {
                retract();
                state = 0;
                iskey = reserve();    // ��ؼ��ֱ�

                if (iskey != -1)  // ʶ������ǹؼ���
                    printf("<%s, ->", token);    
                else {    // ʶ��������û��Զ����ʶ��
                    table_insert();     // �ñ�ʶ��������ű�
                    printf("<id, %s>", token);
                };
            };
            break;
        case 2: //����״̬
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
                    state == ERROR;//��ʾ���ֳ�����
                }
            }
            break;
        case 3: //���á�<��״̬
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
        case 4: //���á�>��״̬
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

//�Ѵ�������Դ������뻺����
void ReadtoBuffer(char* filename, char* destination, int length)
{
    //destination��Ŀ�껺������length�������볤�ȡ�
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
// ���̣�ÿ����һ�Σ�����forward��ָʾ��buffer�ж�һ���ַ����������������C�У�Ȼ���ƶ�forward��ʹ֮ָ����һ���ַ���
void get_char() 
{
    char filename[20] = "input.txt";
    C = buffer[forward++];
    if (buffer[forward] == EOF)
    {
        if (forward == MAX_BUFFER / 2 - 1)//ָ���������eof
        {
            ReadtoBuffer(filename, buffer + forward + 1, MAX_BUFFER / 2 - 1);
            forward++;
        }
        else if (forward == MAX_BUFFER - 1)
        {
            ReadtoBuffer(filename, buffer, MAX_BUFFER / 2 - 1);
            forward = 0;
        }
        else //�����ļ���β
        {
            state = 13;
        }
    }

}
void cat() // ���̣���C�е��ַ�������token�е��ַ������档
{
    int i = 0;
    while (token[i] != '\0') 
    { 
        i++;
    }
    token[i++] = C;

}
bool letter()// �����������ж�C�е��ַ��Ƿ�Ϊ��ĸ��_�� �����򷵻�true�����򷵻�false��
{
    if (('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z') || C == '_')
    {
        return true;
    }
    return false;
}
bool digit()// �����������ж�C�е��ַ��Ƿ�Ϊ���֣������򷵻�true�����򷵻�false��
{
    if (('0' <= C && C <= '9') || C == '.' || C == 'E' || C == 'e')
        return true;
    return false;
    
}
void retract()// ���̣���ǰָ��forward����һ���ַ���
{
    if (forward == 0)//�˻ص�bufferĩβ
    {
        forward = MAX_BUFFER - 2;
    }
    else if (forward == MAX_BUFFER / 2)
    {
        forward = MAX_BUFFER / 2 - 2;
    }
    else forward--;
}
int reserve()// ����������token�еĵ��ʲ�ؼ��ֱ���token�еĵ����ǹؼ��֣��򷵻�ֵ�ùؼ��ֵļǺţ����򣬷���ֵ�� - 1����
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
int table_insert()// ��������ʶ������ı�ʶ������token�еĵ��ʣ�������ű����ظõ����ڷ��ű��е�λ��ָ�롣
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