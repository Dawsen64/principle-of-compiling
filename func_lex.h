#ifndef _FUNC_LEX_H_
#define _FUNC_LEX_H_

#define MAXLEN_LETTER 256
#define MAX_BUFFER 2048 //2KB
#define STACK_SIZE 100
#define STOP -1
#define ERROR -1
#define ID_TABLE_SIZE 1000
typedef struct sta //��������{���� ��ջ
{
	char data[STACK_SIZE];
	int top;
}SqStack;



void get_nbc(); // ���̣����C�е��ַ��Ƿ�Ϊ�ո����ǣ��򷴸����ù���get_char��ֱ��C�н���һ���ǿ��ַ�Ϊֹ��
void get_char(); // ���̣�ÿ����һ�Σ�����forward��ָʾ��buffer�ж�һ���ַ����������������C�У�Ȼ���ƶ�forward��ʹ֮ָ����һ���ַ���
void cat(); // ���̣���C�е��ַ�������token�е��ַ������档
bool letter();// �����������ж�C�е��ַ��Ƿ�Ϊ��ĸ�� �����򷵻�true�����򷵻�false��
bool digit();// �����������ж�C�е��ַ��Ƿ�Ϊ���֣������򷵻�true�����򷵻�false��
bool is_digit();//�ж�token����Ƿ���ʵ��
void retract();// ���̣���ǰָ��forward����һ���ַ���
int reserve();// ����������token�еĵ��ʲ�ؼ��ֱ���token�еĵ����ǹؼ��֣��򷵻�ֵ�ùؼ��ֵļǺţ����򣬷���ֵ�� - 1����
int SToI();// ���̣���token�е��ַ���ת����������
int SToF();// ���̣���token�е��ַ���ת���ɸ�������
int DTB();// ���̣�����token�е����ִ�ת���ɶ����Ƶ���ֵ��ʾ��
int table_insert();// ��������ʶ������ı�ʶ������token�еĵ��ʣ�������ű����ظõ����ڷ��ű��е�λ��ָ�롣
// error;// ���̣��Է��ֵĴ��������Ӧ�Ĵ���
//return;// ���̣���ʶ������ĵ��ʵļǺŷ��ظ����ó���
char* Lex_analysis(); //�ʷ���������
void Init();//��ʼ��ȫ�ֱ����ĺ���
void ReadtoBuffer(char* filename, char* destination, int length);//�Ѵ�������Դ������뻺����
#endif
