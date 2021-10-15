#include "func_lex.h"


/*********ȫ�ֱ���************/

int state; //���ͱ�������ǰ״ָ̬ʾ��
char C; //�ַ���������ŵ�ǰ������ַ���
char token[MAXLEN_LETTER];// �ַ����飬��ŵ�ǰ����ʶ��ĵ����ַ�����
char buffer[MAX_BUFFER]; // �ַ����飬���뻺������
int forward;// �ַ�ָ�룬��ǰָ�롣
int lexemebegin; // �ַ�ָ�룬ָ��buffer�е�ǰ���ʵĿ�ʼλ�á�
int iskey; // ���ͱ�����ֵΪ - 1����ʾʶ����ĵ������û��Զ����ʶ�������򣬱�ʾʶ����ĵ����ǹؼ��֣���ֵΪ�ؼ��ֵļǺš�

char* Lex_analysis()
{
    state = 0;
    switch (state) {
    case 0:      // ��ʼ״̬
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
            state = 1; break;      //���ñ�ʶ��״̬
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            state = 2; break;      //���ó���״̬
        case '<': state = 8; break;      //�衮<��״̬
        case '>': state = 9; break;      //���á�>��״̬
        case ':': state = 10; break;      //���á�����״̬
        case '/': state = 11; break;      //���á�/��״̬
        case '=': state = 0; return('EQ'); break;      //���ء�=���ļǺ�
        case '+': state = 0; return('+'); break;              //���ء�+���ļǺ�
        case '-': state = 0; return('-'); break;              //���ء�-���ļǺ�
        case '*': state = 0; return('*'); break;               //���ء�*���ļǺ�
        case '(': state = 0; return('('); break;                //���ء������ļǺ�
        case ')': state = 0; return(')'); break;               //���ء������ļǺ�
        case ';': state = 0; return(';'); break;               //���ء������ļǺ�
        case '\'': state = 0; return('\''); break;              //���ء��䡯�ļǺ�
        default: state = 13; break;      //���ô���״̬
        };
        break;
    case 1:      // ��ʶ��״̬
        cat();
        get_char();
        if (letter() || digit())  state = 1;
        else {
            retract();
            state = 0;
            iskey = reserve();    // ��ؼ��ֱ�
            if (iskey != -1) return ("iskey");    // ʶ������ǹؼ���
            else {    // ʶ��������û��Զ����ʶ��
                identry = table_insert();     // ���ظñ�ʶ���ڷ��ű�����ָ��
                return(ID, identry);
            };
        };
        break;
    }