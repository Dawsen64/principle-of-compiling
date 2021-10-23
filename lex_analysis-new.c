#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>

#include "func_lex.h"
/*********ȫ�ֱ���************/

int state; //���ͱ�������ǰ״ָ̬ʾ
char C; //�ַ���������ŵ�ǰ������ַ���

char keywords[34][20] = { "auto","break","case","char","const","continue",
"default","do","double","else","enum","extern","float","for","goto",
"if","int","long ","register","return","short","signed","static",
"sizeof","struct","switch","typedef","unionunsigned","void","volatile","while"};//�ؼ��ּ�����include,define

char token[MAXLEN_LETTER];// �ַ����飬��ŵ�ǰ����ʶ��ĵ����ַ�����
short tokenptr = 0;//ָ��token�ġ�\0���ַ�
char buffer[MAX_BUFFER]; // 
int flen = 0;
int forward;// 
int iskey; // 
char Id_Table[ID_TABLE_SIZE][MAXLEN_LETTER];//
int id_length;
Counter counter;
void Init()//
{
    //��ʼ��buffer
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
                state = 1;//��ʶ����ؼ���
                break;     
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                state = 2;
                break;      //����
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
            case '/': //����/��//��/*
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
            case '!'://!��!=
                state = 14;
                break;
            case '#'://����#include,#define
                state = 15;
                break;
            case ';':
            case '(': case ')': case '[': case ']': case '{': case '}':
            case ',': case '.': case '?': case ':': case '%': case '^': case '~':
                state = 16;
                break;
            case EOF:
                state = STOP; //�ַ���β��״̬��׼������
                break;
            default:
                state = ERROR;//����״̬
                break;      
            };
            break;
        case 1:      // �����ʶ��id
            cat();
            get_char();
            if (letter() || digit())
                state = 1;
            else
            {
                retract();
                state = 0;
                iskey = reserve();    // ���ҹؼ���

                if (iskey == 1)  //
                {
                    counter.keyword_num++;
                    printf("<%s, ->\n", token);
                }
                else {    // ��ʶ��
                    table_insert();     
                    counter.id_num++;
                    printf("<id, %s>\n", token);
                };
            };
            break;
        case 2: //����
            cat();
            get_char();
            if (digit() || C == '.')
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
        case 3: //���á�<,<=,<<��
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
        case 4: //���á�>, >=, >>��
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
        case 5: //���á�==��
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
        case 6: //���á�+��++��
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
        case 7: //���á�-��--��
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
        case 8: //����"*"
            printf("<*, ->\n");
            counter.operator_num++;
            state = 0;
            break;
        case 9: //����/��ע��/**/�� //
            get_char();
            if (C == '/')//
            {
                get_char();
                while (C != '\n')
                {
                    get_char();
                }
                retract();
            }
            else if (C == '*') ///*...*/'
            {
                get_char();
                if (C == '\n')
                    counter.lines++;
                while (state != 0)
                {
                    if (C == '*')
                    {
                        get_char();
                        if (C == '/')
                        {
                            state = 0;
                            //break;
                        }
                        retract();
                    }
                    get_char();
                    if (C == '\n')
                        counter.lines++;
                }
            }
            else
            {
                printf("</, ->\n");
                counter.operator_num++;
            }
            state = 0;
            break;
        case 10://����'�ַ�״̬
            get_char();
            while (C != '\'')
            {
                cat();
                get_char();
            }
            printf("<const char, '%s'>\n", token);
            state = 0;
            break;
        case 11://���á��ַ���״̬
            get_char();
            while (C != '\"')
            {
                cat();
                get_char();
            }
            retract();
            printf("<const char[], \"%s\">\n", token);
            state = 0;
            break;
        case 12: //���� &,&&;
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
        case 13: //����|,||
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
        case 14: //����!, !=
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
                    printf("<#id, %s>\n", s[1]);
                else state = ERROR;
            }
            else
            {
                retract();
                state == ERROR;
            }

            state = 0;
            break;
        case 16: //�������൥������
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
}


void ReadtoBuffer(const char* filename, char* destination, int length)
{
    //��Դ������뻺����
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
//ÿ����һ�Σ�����forward��ָʾ��buffer��    ��һ���ַ����������������C�У�Ȼ���ƶ�forward��ʹ֮    ָ����һ���ַ���
void get_char()
{
    C = buffer[forward++];
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
void cat() // ��C�е��ַ����ᵽtoken��
{
    token[tokenptr++] = C;
    token[tokenptr] = '\0';

}
bool letter()// �����������ж�C�е��ַ��Ƿ�Ϊ��ĸ��    �����򷵻�true�����򷵻�false
{
    if (('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z') || C == '_')
    {
        return true;
    }
    return false;
}
bool digit()// �����������ж�C�е��ַ��Ƿ�Ϊ���֣�    �����򷵻�true�����򷵻�false��
{
    if (('0' <= C && C <= '9') || C == 'E' || C == 'e')
        return true;
    return false;
    
}
void retract()// ��ǰָ��forward����һ���ַ�
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
int reserve()// �����Ƿ��ǹؼ���
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
void get_nbc() //�����հ׷�
{
    while (C == ' ' || C == '\n' || C == '\t')
    {
        if (C == '\n')
            counter.lines++;
        get_char();
    }
}
void Print_num()//�������ͳ������
{
    printf("����:%d\n", counter.lines);
    printf("�ַ�����:%d\n", counter.char_num);
    printf("�ؼ�������:%d\n", counter.keyword_num);
    printf("��ʶ������:%d\n", counter.id_num);
    printf("����:%d\n", counter.digit_num);
    printf("�ֽ��:%d\n", counter.dilimiter_num);
    printf("�����:%d\n", counter.operator_num);
    printf("��ǰ�ļ�ָ��: %d\n");
}