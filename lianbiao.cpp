//-------------------------------------------------------------------
// 梁嘉城 2018091605010 github.com/jackpurcell
//-------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#pragma warning(disable : 4996)

//--------------------------------------------------------------------
//商品的最大数量
//--------------------------------------------------------------------
#define MAX 100
//--------------------------------------------------------------------
//全局变量，存储当前商品的数量
//--------------------------------------------------------------------
int CurrentCnt = 0;
//--------------------------------------------------------------------
//商品信息结构的定义
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
//定义商品链表的结点
//--------------------------------------------------------------------
typedef struct node
{
	GoodInfo data;
	struct node *next;
}GoodList;


//--------------------------------------------------------------------
//检查商品文件是否存在或者是否为空
//--------------------------------------------------------------------
bool check_nullfile(void)
{

	FILE *fp = fopen("goodinfo.txt", "r");
	if (!fp) {
		printf("商品信息初始化文件不存在！程序将为您新建一个.\n");
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
//以行读取输入的信息
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
//打印单个商品链表节点的信息
//-------------------------------------------------------------
void Goodprint(GoodList *cur) {
	printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("ID:%s\t名称:%s\t价格:%d\t折扣:%s\t数量:%d\t剩余:%d\n", cur->data.good_id, cur->data.good_name, cur->data.good_price, cur->data.good_discount, cur->data.good_amount, cur->data.good_remain);
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}
//--------------------------------------------------------------------
//释放商品链表
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
//删除商品文件以及商品链表中的所有信息
//--------------------------------------------------------------------
void DelAll(GoodList **L) {
	DestroyGoods(L);	//清空商品链表，销毁已有的节点，并新建一个空链表头
	remove("goodinfo.txt");//清空存储商品内容的文件
	printf("删除成功");
}

//--------------------------------------------------------------------
//读取商品文件goodinfo.txt的内容,并建立链表L
//--------------------------------------------------------------------
void info_init(GoodList **L) {
	int i = 0, j = 0;
	FILE *fp = fopen("goodinfo.txt", "r");

	GoodList *new_node, *cur;
	cur = (*L);
	while (check_nullfile() && !feof(fp)) {		//判断goodinfo.txt是否存在，不存在则新建一个goodinfo.txt
																	//在未达到文件尾的情况下，从文件中读取商品信息
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

	printf("商品的链表文件已建立，有%d个商品记录\n", CurrentCnt);
}

//--------------------------------------------------------------------
//将当前商品链表中的内容存入商品文件goodinfo.txt，存盘后销毁链表L
//--------------------------------------------------------------------
void info_flush(GoodList **L) {
	if (CurrentCnt == 0) {
		printf("无商品信息，自动退出!");
		exit(0);
	}
	FILE *fp;
	fp = fopen("goodinfo.txt", "w");
	GoodList *new_node = (*L)->next;
	int savecount = 0;
	while (new_node != NULL) {//将链表内容写入到goodinfo.txt
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
		printf("%d个商品信息存入Goodinfo.txt文件\n", savecount);
	}
	else {
		printf("未写入任何商品信息到Goodinfo.txt文件\n");
	}
	DestroyGoods(L);//销毁链表
	CurrentCnt = 0;
	fclose(fp);
	exit(0);
}

//--------------------------------------------------------------------
//在屏幕上输出所有商品信息
//--------------------------------------------------------------------
void OutputAll(GoodList *L) {
	if (CurrentCnt == 0) {
		printf("无商品信息，自动退出!");
		return;
	}
	GoodList *cur;
	cur = L->next;
	for (; cur != NULL; cur = cur->next) {//对链表内容进行读取，然后输出商品信息
		Goodprint(cur);
	}
}

//--------------------------------------------------------------------
//添加一条商品记录
//--------------------------------------------------------------------
void info_insert(GoodList **L) {

	GoodList *new_node, *cur, *prev;
	int i;

	new_node = (GoodList*)malloc(sizeof(GoodList));
	if (new_node == NULL) {
		printf("Database is full; can't add more parts.\n");
		return;
	}
	printf("输入新的商品信息: ");
	printf("商品ID: ");
	read_line(new_node->data.good_id, MAX_ID_LEN);
	for (
		cur = (*L)->next, prev = NULL;
		cur != NULL && strcmp(new_node->data.good_id, cur->data.good_id);
		prev = cur, cur = cur->next)
		;
	if (cur != NULL && !strcmp(new_node->data.good_id, cur->data.good_id)) {
		printf("商品已存在!\n");//ID号在插入的时候进行查重，如果已经有该ID号了，提示商品已存在!
		free(new_node);
		return;
	}

	printf("商品名称: ");
	read_line(new_node->data.good_name, MAX_NAME_LEN);
	printf("商品价格: ");
	scanf("%d", &new_node->data.good_price);
	printf("商品折扣: ");
	read_line(new_node->data.good_discount, MAX_DISCOUNT_LEN);
	printf("商品数量: ");
	scanf("%d", &new_node->data.good_amount);
	printf("商品剩余: ");
	scanf("%d", &new_node->data.good_remain);
	printf("输入数字表明你要插入的商品位置: 0.商品列表尾部 1.商品列表头部 i.商品列表中第i号位置");
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
				printf("无法在该位置插入!自动插入到尾部!");
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
	printf("插入成功！");
}

//--------------------------------------------------------------------
//删除一条商品记录
//--------------------------------------------------------------------
void info_dele(GoodList **L) {
	if (CurrentCnt == 0) {
		printf("无商品信息，自动退出!");
		return;
	}
	char id[MAX_ID_LEN];
	GoodList  *prev, *cur;
	prev = NULL;
	char elect;
	printf("输入需要删除的商品ID(-1退出删除): ");
	scanf("%s", &id);
	if (!strcmp(id, "-1\0")) {
		printf("已退出!");
		return;
	}
	for (prev = (*L), cur = (*L)->next; strcmp(id, cur->data.good_id) && cur->next != NULL; prev = cur, cur = cur->next);
	if (strcmp(id, cur->data.good_id)) {
		printf("商品不存在！");
	}
	else {
		printf("是否删除该商品(Y/N)");
		scanf(" %c", &elect);
		if (elect == 'Y' || elect == 'y') {
			if (cur->next != NULL) {
				prev->next = cur->next;
			}
			else {
				prev->next = NULL;
			}
			free(cur);//释放对应指针所指向的内存
			cur = NULL;//将该指针赋值为空
			printf("\n删除成功ID为%s的商品成功\n", id);
			printf("当前剩余商品%d个", --CurrentCnt);
		}
		else {
			return;
		}
	}

}

//--------------------------------------------------------------------
//查找一条商品记录
//--------------------------------------------------------------------
void info_search(GoodList *L) {
	if (CurrentCnt == 0) {
		printf("无商品信息，自动退出!");
		return;
	}
	char name[MAX_NAME_LEN];
	GoodList *prev, *cur;
	printf("输入需要查找的商品名称(-1退出查找): ");
	scanf("%s", &name);
	if (!strcmp(name, "-1\0")) {
		printf("已退出!");
		return;
	}
	prev = NULL;
	for (cur = L->next; strcmp(name, cur->data.good_name) && cur->next != NULL; cur = cur->next);
	if (strcmp(name, cur->data.good_name)) {
		printf("商品不存在！");
	}
	else {
		Goodprint(cur);
	}
}

//--------------------------------------------------------------------
//修改一条商品记录
//--------------------------------------------------------------------
void info_change(GoodList **L) {
	if (CurrentCnt == 0) {
		printf("无商品信息，自动退出!");
		return;
	}
	char id[MAX_ID_LEN];
	GoodList  *cur;
	printf("输入需要修改的商品ID(-1退出修改): ");
	scanf("%s", &id);
	if (!strcmp(id, "-1\0")) {
		printf("已退出!");
		return;
	}
	for (cur = (*L)->next; strcmp(id, cur->data.good_id) && cur->next != NULL; cur = cur->next)
		;
	if (strcmp(id, cur->data.good_id)) {
		printf("商品不存在！");
	}
	else {
		Goodprint(cur);
		printf("输入新的商品信息：");
		printf("商品ID: ");
		read_line(cur->data.good_id, MAX_ID_LEN);
		printf("商品名称: ");
		read_line(cur->data.good_name, MAX_NAME_LEN);
		printf("商品价格: ");
		scanf("%d", &cur->data.good_price);
		printf("商品折扣: ");
		read_line(cur->data.good_discount, MAX_DISCOUNT_LEN);
		printf("商品数量: ");
		scanf("%d", &cur->data.good_amount);
		printf("商品剩余: ");
		scanf("%d", &cur->data.good_remain);
		printf("修改成功！");
	}
}

//--------------------------------------------------------------------
//冒泡排序实现对链表节点的排序
//--------------------------------------------------------------------
void bubble_sort(GoodList **L) {
	if (CurrentCnt == 0) {
		printf("无商品信息，自动退出!");
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

	printf("当前有%d个商品:", N);
	OutputAll(*L);
}

//--------------------------------------------------------------------
//主函数
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
			"***梁嘉城 2018091605010 github.com/jackpurcell***\n"
			"***欢迎访问超市管理系统***\n"
			"\033[33m**************************************\n\033[0m"
			"\033[32m1.显示所有商品的信息:\n\033[0m"
			"\033[32m2.修改某个商品的信息:\n\033[0m"
			"\033[32m3.插入某个商品的信息:\n\033[0m"
			"\033[32m4.删除某个商品的信息:\n\033[0m"
			"\033[32m5.查找某个商品的信息:\n\033[0m"
			"\033[32m6.商品存盘并退出系统:\n\033[0m"
			"\033[32m7.对商品价格进行排序:\n\033[0m"
			"\033[32m8.(慎用)删除所有内容:\n\033[0m"
			"\033[32m其他.不存盘并退出系统:\n\033[0m"
			"\033[33m**************************************\n\033[0m"
			"输入你的选择:");

		scanf(" %c", &code);
		while (getchar() != '\n')
			;
		switch (code) {
		case '1': OutputAll(L);//完成商品信息的的输出功能
			break;
		case '2': info_change(&L);//完成商品信息的修改功能
			break;
		case '3': info_insert(&L);//完成商品信息的插入功能。
			break;
		case '4': info_dele(&L);//完成商品信息的删除功能
			break;
		case '5': info_search(L);//完成商品信息的搜索功能
			break;
		case '6': info_flush(&L);//该函数把所有商品信息（链表中的所有商品）写入到已有的商品信息文件中
			break;
		case '7': bubble_sort(&L);//用冒泡法实现商品信息按照价格从低到高进行排序展示
			break;
		case '8':DelAll(&L);//删除系统所有内容
			break;
		default:exit(0);
		}
		printf("\n");
	}
}