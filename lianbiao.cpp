//-------------------------------------------------------------------
// ���γ� 2018091605010 github.com/jackpurcell
//-------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#pragma warning(disable : 4996)

//--------------------------------------------------------------------
//��Ʒ���������
//--------------------------------------------------------------------
#define MAX 100
//--------------------------------------------------------------------
//ȫ�ֱ������洢��ǰ��Ʒ������
//--------------------------------------------------------------------
int CurrentCnt = 0;
//--------------------------------------------------------------------
//��Ʒ��Ϣ�ṹ�Ķ���
//--------------------------------------------------------------------
#define MAX_ID_LEN 30
#define MAX_NAME_LEN 30
#define MAX_PRICE_LEN 30
#define MAX_DISCOUNT_LEN 30
typedef struct {
	char		good_id[MAX_ID_LEN];
	char		good_name[MAX_NAME_LEN];
	int  		good_price;
	char		good_discount[MAX_DISCOUNT_LEN];
	int		good_amount;
	int		good_remain;
}GoodInfo;

//--------------------------------------------------------------------
//������Ʒ����Ľ��
//--------------------------------------------------------------------
typedef struct node
{
	GoodInfo data;
	struct node *next;
}GoodList;


//--------------------------------------------------------------------
//�����Ʒ�ļ��Ƿ���ڻ����Ƿ�Ϊ��
//--------------------------------------------------------------------
bool check_nullfile(void)
{

	FILE *fp = fopen("goodinfo.txt", "r");
	if (!fp) {
		printf("��Ʒ��Ϣ��ʼ���ļ������ڣ�����Ϊ���½�һ��.\n");
		FILE *fp = fopen("goodinfo.txt", "w");
		fclose(fp);
		return false;
	}
	else {
		int temp;
		int res = fscanf(fp, "%d", &temp);
		fclose(fp);
		if (res <= 0)
			return false;
		else
			return true;
	}
}

//--------------------------------------------------------------------
//���ж�ȡ�������Ϣ
//--------------------------------------------------------------------
int read_line(char str[], int n)
{
	int ch, i = 0;

	while (isspace(ch = getchar()))
		;
	while (ch != '\n' && ch != EOF) {
		if (i < n)
			str[i++] = ch;
		ch = getchar();
	}
	str[i] = '\0';
	return i;
}
//--------------------------------------------------------------------
//��ӡ������Ʒ����ڵ����Ϣ
//-------------------------------------------------------------
void Goodprint(GoodList *cur) {
	printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("ID:%s\t����:%s\t�۸�:%d\t�ۿ�:%s\t����:%d\tʣ��:%d\n", cur->data.good_id, cur->data.good_name, cur->data.good_price, cur->data.good_discount, cur->data.good_amount, cur->data.good_remain);
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}
//--------------------------------------------------------------------
//�ͷ���Ʒ����
//--------------------------------------------------------------------
void DestroyGoods(GoodList **L) {
	GoodList *prev, *cur;
	prev = (*L);
	cur = prev->next;
	while (cur) {
		prev = cur;
		cur = prev->next;
		(*L)->next = cur;
		free(prev);
	}
	CurrentCnt = 0;
}

//--------------------------------------------------------------------
//ɾ����Ʒ�ļ��Լ���Ʒ�����е�������Ϣ
//--------------------------------------------------------------------
void DelAll(GoodList **L) {
	DestroyGoods(L);	//�����Ʒ�����������еĽڵ㣬���½�һ��������ͷ
	remove("goodinfo.txt");//��մ洢��Ʒ���ݵ��ļ�
	printf("ɾ���ɹ�");
}

//--------------------------------------------------------------------
//��ȡ��Ʒ�ļ�goodinfo.txt������,����������L
//--------------------------------------------------------------------
void info_init(GoodList **L) {
	int i = 0, j = 0;
	FILE *fp = fopen("goodinfo.txt", "r");

	GoodList *new_node, *cur;
	cur = (*L);
	while (check_nullfile() && !feof(fp)) {		//�ж�goodinfo.txt�Ƿ���ڣ����������½�һ��goodinfo.txt
																	//��δ�ﵽ�ļ�β������£����ļ��ж�ȡ��Ʒ��Ϣ
		new_node = (GoodList*)malloc(sizeof(GoodList));
		new_node->next = NULL;
		fscanf(fp, "%s", &new_node->data.good_id);
		fscanf(fp, "\t%s", &new_node->data.good_name);
		fscanf(fp, "\t%d", &new_node->data.good_price);
		fscanf(fp, "\t%s", &new_node->data.good_discount);
		fscanf(fp, "\t%d", &new_node->data.good_amount);
		fscanf(fp, "\t%d\n", &new_node->data.good_remain);

		cur->next = new_node;
		if ((*L)->next == NULL) {
			(*L)->next = cur->next;
		}
		cur = cur->next;
		CurrentCnt++;
	}
	if (CurrentCnt != 0) {
		fclose(fp);
	}

	printf("��Ʒ�������ļ��ѽ�������%d����Ʒ��¼\n", CurrentCnt);
}

//--------------------------------------------------------------------
//����ǰ��Ʒ�����е����ݴ�����Ʒ�ļ�goodinfo.txt�����̺���������L
//--------------------------------------------------------------------
void info_flush(GoodList **L) {
	if (CurrentCnt == 0) {
		printf("����Ʒ��Ϣ���Զ��˳�!");
		exit(0);
	}
	FILE *fp;
	fp = fopen("goodinfo.txt", "w");
	GoodList *new_node = (*L)->next;
	int savecount = 0;
	while (new_node != NULL) {//����������д�뵽goodinfo.txt
		fprintf(fp, "%s\t", new_node->data.good_id);
		fprintf(fp, "%s\t", new_node->data.good_name);
		fprintf(fp, "%d\t", new_node->data.good_price);
		fprintf(fp, "%s\t", new_node->data.good_discount);
		fprintf(fp, "%d\t", new_node->data.good_amount);
		fprintf(fp, "%d\n", new_node->data.good_remain);
		new_node = new_node->next;
		savecount++;
	}
	if (savecount != 0) {
		printf("%d����Ʒ��Ϣ����Goodinfo.txt�ļ�\n", savecount);
	}
	else {
		printf("δд���κ���Ʒ��Ϣ��Goodinfo.txt�ļ�\n");
	}
	DestroyGoods(L);//��������
	CurrentCnt = 0;
	fclose(fp);
	exit(0);
}

//--------------------------------------------------------------------
//����Ļ�����������Ʒ��Ϣ
//--------------------------------------------------------------------
void OutputAll(GoodList *L) {
	if (CurrentCnt == 0) {
		printf("����Ʒ��Ϣ���Զ��˳�!");
		return;
	}
	GoodList *cur;
	cur = L->next;
	for (; cur != NULL; cur = cur->next) {//���������ݽ��ж�ȡ��Ȼ�������Ʒ��Ϣ
		Goodprint(cur);
	}
}

//--------------------------------------------------------------------
//���һ����Ʒ��¼
//--------------------------------------------------------------------
void info_insert(GoodList **L) {

	GoodList *new_node, *cur, *prev;
	int i;

	new_node = (GoodList*)malloc(sizeof(GoodList));
	if (new_node == NULL) {
		printf("Database is full; can't add more parts.\n");
		return;
	}
	printf("�����µ���Ʒ��Ϣ: ");
	printf("��ƷID: ");
	read_line(new_node->data.good_id, MAX_ID_LEN);
	for (
		cur = (*L)->next, prev = NULL;
		cur != NULL && strcmp(new_node->data.good_id, cur->data.good_id);
		prev = cur, cur = cur->next)
		;
	if (cur != NULL && !strcmp(new_node->data.good_id, cur->data.good_id)) {
		printf("��Ʒ�Ѵ���!\n");//ID���ڲ����ʱ����в��أ�����Ѿ��и�ID���ˣ���ʾ��Ʒ�Ѵ���!
		free(new_node);
		return;
	}

	printf("��Ʒ����: ");
	read_line(new_node->data.good_name, MAX_NAME_LEN);
	printf("��Ʒ�۸�: ");
	scanf("%d", &new_node->data.good_price);
	printf("��Ʒ�ۿ�: ");
	read_line(new_node->data.good_discount, MAX_DISCOUNT_LEN);
	printf("��Ʒ����: ");
	scanf("%d", &new_node->data.good_amount);
	printf("��Ʒʣ��: ");
	scanf("%d", &new_node->data.good_remain);
	printf("�������ֱ�����Ҫ�������Ʒλ��: 0.��Ʒ�б�β�� 1.��Ʒ�б�ͷ�� i.��Ʒ�б��е�i��λ��");
	scanf("%d", &i);
	if (i == 0) {
		cur = (*L)->next, prev = NULL;
		if (cur == NULL) {
			cur = new_node;
			(*L)->next = new_node;
			new_node->next = NULL;
		}
		else {
			while (NULL != cur->next) {
				cur = cur->next;
			}
			cur->next = new_node;
			new_node->next = NULL;
		}
	}
	else if (i == 1) {
		cur = (*L)->next, prev = new_node;
		if (cur == NULL) {
			cur = new_node;
			new_node->next = NULL;
			(*L)->next = cur;
		}
		else {
			(*L)->next = prev;
			prev->next = cur;
		}
	}
	else {
		i--;
		prev = (*L);
		cur = (*L)->next;
		while (i != 0) {
			if (cur == NULL) {
				printf("�޷��ڸ�λ�ò���!�Զ����뵽β��!");
				break;
			}
			prev = cur;
			cur = cur->next;
			i--;
		}
		prev->next = new_node;
		new_node->next = cur;
	}
	CurrentCnt++;
	printf("����ɹ���");
}

//--------------------------------------------------------------------
//ɾ��һ����Ʒ��¼
//--------------------------------------------------------------------
void info_dele(GoodList **L) {
	if (CurrentCnt == 0) {
		printf("����Ʒ��Ϣ���Զ��˳�!");
		return;
	}
	char id[MAX_ID_LEN];
	GoodList  *prev, *cur;
	prev = NULL;
	char elect;
	printf("������Ҫɾ������ƷID(-1�˳�ɾ��): ");
	scanf("%s", &id);
	if (!strcmp(id, "-1\0")) {
		printf("���˳�!");
		return;
	}
	for (prev = (*L), cur = (*L)->next; strcmp(id, cur->data.good_id) && cur->next != NULL; prev = cur, cur = cur->next);
	if (strcmp(id, cur->data.good_id)) {
		printf("��Ʒ�����ڣ�");
	}
	else {
		printf("�Ƿ�ɾ������Ʒ(Y/N)");
		scanf(" %c", &elect);
		if (elect == 'Y' || elect == 'y') {
			if (cur->next != NULL) {
				prev->next = cur->next;
			}
			else {
				prev->next = NULL;
			}
			free(cur);//�ͷŶ�Ӧָ����ָ����ڴ�
			cur = NULL;//����ָ�븳ֵΪ��
			printf("\nɾ���ɹ�IDΪ%s����Ʒ�ɹ�\n", id);
			printf("��ǰʣ����Ʒ%d��", --CurrentCnt);
		}
		else {
			return;
		}
	}

}

//--------------------------------------------------------------------
//����һ����Ʒ��¼
//--------------------------------------------------------------------
void info_search(GoodList *L) {
	if (CurrentCnt == 0) {
		printf("����Ʒ��Ϣ���Զ��˳�!");
		return;
	}
	char name[MAX_NAME_LEN];
	GoodList *prev, *cur;
	printf("������Ҫ���ҵ���Ʒ����(-1�˳�����): ");
	scanf("%s", &name);
	if (!strcmp(name, "-1\0")) {
		printf("���˳�!");
		return;
	}
	prev = NULL;
	for (cur = L->next; strcmp(name, cur->data.good_name) && cur->next != NULL; cur = cur->next);
	if (strcmp(name, cur->data.good_name)) {
		printf("��Ʒ�����ڣ�");
	}
	else {
		Goodprint(cur);
	}
}

//--------------------------------------------------------------------
//�޸�һ����Ʒ��¼
//--------------------------------------------------------------------
void info_change(GoodList **L) {
	if (CurrentCnt == 0) {
		printf("����Ʒ��Ϣ���Զ��˳�!");
		return;
	}
	char id[MAX_ID_LEN];
	GoodList  *cur;
	printf("������Ҫ�޸ĵ���ƷID(-1�˳��޸�): ");
	scanf("%s", &id);
	if (!strcmp(id, "-1\0")) {
		printf("���˳�!");
		return;
	}
	for (cur = (*L)->next; strcmp(id, cur->data.good_id) && cur->next != NULL; cur = cur->next)
		;
	if (strcmp(id, cur->data.good_id)) {
		printf("��Ʒ�����ڣ�");
	}
	else {
		Goodprint(cur);
		printf("�����µ���Ʒ��Ϣ��");
		printf("��ƷID: ");
		read_line(cur->data.good_id, MAX_ID_LEN);
		printf("��Ʒ����: ");
		read_line(cur->data.good_name, MAX_NAME_LEN);
		printf("��Ʒ�۸�: ");
		scanf("%d", &cur->data.good_price);
		printf("��Ʒ�ۿ�: ");
		read_line(cur->data.good_discount, MAX_DISCOUNT_LEN);
		printf("��Ʒ����: ");
		scanf("%d", &cur->data.good_amount);
		printf("��Ʒʣ��: ");
		scanf("%d", &cur->data.good_remain);
		printf("�޸ĳɹ���");
	}
}

//--------------------------------------------------------------------
//ð������ʵ�ֶ�����ڵ������
//--------------------------------------------------------------------
void bubble_sort(GoodList **L) {
	if (CurrentCnt == 0) {
		printf("����Ʒ��Ϣ���Զ��˳�!");
		return;
	}
	int N = 0;
	GoodList *p, *prev, *cur, *t;

	p = (*L)->next;
	prev = (*L);
	cur = (*L)->next;
	while (p)
	{
		p = p->next;
		N++;
	}
	for (int j = 0; j < N - 1; j++)
	{
		prev = (*L);
		cur = (*L)->next;
		for (int k = 0; k < N - j - 1; k++) {

			if (cur->data.good_price > cur->next->data.good_price) {
				t = cur;
				cur = cur->next;
				t->next = cur->next;
				prev->next = cur;
				cur->next = t;
			}
			if (cur->next->next != NULL) {
				prev = cur;
				cur = cur->next;
			}
			else {
				break;
			}
		}
	}

	printf("��ǰ��%d����Ʒ:", N);
	OutputAll(*L);
}

//--------------------------------------------------------------------
//������
//--------------------------------------------------------------------
int main()
{
	char code;
	GoodList *L;
	L = (GoodList*)malloc(sizeof(GoodList));
	L->next = NULL;
	info_init(&L);
	for (;;) {
		printf(
			"\n"
			"***���γ� 2018091605010 github.com/jackpurcell***\n"
			"***��ӭ���ʳ��й���ϵͳ***\n"
			"\033[33m**************************************\n\033[0m"
			"\033[32m1.��ʾ������Ʒ����Ϣ:\n\033[0m"
			"\033[32m2.�޸�ĳ����Ʒ����Ϣ:\n\033[0m"
			"\033[32m3.����ĳ����Ʒ����Ϣ:\n\033[0m"
			"\033[32m4.ɾ��ĳ����Ʒ����Ϣ:\n\033[0m"
			"\033[32m5.����ĳ����Ʒ����Ϣ:\n\033[0m"
			"\033[32m6.��Ʒ���̲��˳�ϵͳ:\n\033[0m"
			"\033[32m7.����Ʒ�۸��������:\n\033[0m"
			"\033[32m8.(����)ɾ����������:\n\033[0m"
			"\033[32m����.�����̲��˳�ϵͳ:\n\033[0m"
			"\033[33m**************************************\n\033[0m"
			"�������ѡ��:");

		scanf(" %c", &code);
		while (getchar() != '\n')
			;
		switch (code) {
		case '1': OutputAll(L);//�����Ʒ��Ϣ�ĵ��������
			break;
		case '2': info_change(&L);//�����Ʒ��Ϣ���޸Ĺ���
			break;
		case '3': info_insert(&L);//�����Ʒ��Ϣ�Ĳ��빦�ܡ�
			break;
		case '4': info_dele(&L);//�����Ʒ��Ϣ��ɾ������
			break;
		case '5': info_search(L);//�����Ʒ��Ϣ����������
			break;
		case '6': info_flush(&L);//�ú�����������Ʒ��Ϣ�������е�������Ʒ��д�뵽���е���Ʒ��Ϣ�ļ���
			break;
		case '7': bubble_sort(&L);//��ð�ݷ�ʵ����Ʒ��Ϣ���ռ۸�ӵ͵��߽�������չʾ
			break;
		case '8':DelAll(&L);//ɾ��ϵͳ��������
			break;
		default:exit(0);
		}
		printf("\n");
	}
}