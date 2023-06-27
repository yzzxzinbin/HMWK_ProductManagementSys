#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
using namespace std;
typedef struct {
	int id;
	char name[100];
	int quantity;
	time_t time;
	struct tm timeStruct;
} Product;

typedef struct {
	int front;
	int rear;
	int size;
	Product *products;
} Queue;

//ADT相关函数
void initQueue(Queue *queue, int size) ;
int isEmpty(Queue *queue);
int isFull(Queue *queue) ;
void enqueue(Queue *queue, Product product);
Product dequeue(Queue *queue);
void clearInputBuffer() ;
time_t getCurrentTime();
void stockIn(Queue *queue);
void stockOut(Queue *queue) ;
void queryQuantity(Queue *queue) ;
void printProducts(Queue *queue);
//临时编写的工具函数 
int strToTime(const char *str, struct tm *timeStruct);
void convertToTimeStr(time_t timestamp, char *timeStr);
void SetCurPos(int x, int y);
void drawBlock(int startx, int starty, int endx, int endy, int backinfo, int foreninfo);
//过去编写的工具函数
void cui_basic_line(int x, int y, int longdata, wchar_t symbol, int direct);//起始点横纵坐标,长度,符号,方向
void cui_position_set(int x, int y);//移动光标至指定位置
void cui_square_frame(int x, int y, int lenth, int height, int model); //起始点横纵坐标,长度,高度,模式
void cui_basic_fill(int x, int y, int lenth, int height, wchar_t ch);//起始点横纵坐标,长度,高度,字符
double cct_time_proc(char model);
void csy_load_style(int backinfo, int foreinfo); //背景色,前景色
void csy_load_window(int x, int y, const char info[30], int backinfo, int foreinfo,int backinfoPAST, int foreinfoPAST); //长度,高度,标题,背景色,前景色


// 初始化队列
void initQueue(Queue *queue, int size) {
	queue->products = (Product *)malloc(size * sizeof(Product));
	queue->front = 0;
	queue->rear = -1;
	queue->size = size;
}

// 判断队列是否为空
int isEmpty(Queue *queue) {
	return queue->rear < queue->front;
}

// 判断队列是否已满
int isFull(Queue *queue) {
	return queue->rear >= queue->size - 1;
}

// 入队
void enqueue(Queue *queue, Product product) {
	if (isFull(queue)) {
		printf("队列已满，无法入队\n");
		return;
	}
	queue->products[++queue->rear] = product;
}

// 出队
Product dequeue(Queue *queue) {
	if (isEmpty(queue)) {
		printf("队列为空，无法出队\n");
		Product emptyProduct;
		emptyProduct.id = -1;
		return emptyProduct;
	}
	return queue->products[queue->front++];
}

// 清除输入缓冲区
void clearInputBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

// 商品入库
void stockIn(Queue *queue) {
	Product product;
	printf("请输入商品编号：");
	scanf("%d", &product.id);
	clearInputBuffer();
	printf("请输入商品名称：");
	fgets(product.name, sizeof(product.name), stdin);
	product.name[strcspn(product.name, "\n")] = '\0';  // 移除换行符
	printf("请输入商品数量：");
	scanf("%d", &product.quantity);
	clearInputBuffer();
	char timestr[50];
	printf("请输入入库日期(YYYY-MM-DD)：");
	scanf("%s", timestr);
	memset(&(product.timeStruct), 0, sizeof(struct tm));
	strToTime(timestr, &(product.timeStruct));
	product.time = mktime(&(product.timeStruct));
	enqueue(queue, product);
	printf("SUC>>商品入库成功\n");
	getchar();
}

// 商品出库
//void stockOut(Queue *queue) {
//	if (isEmpty(queue)) {
//		printf("Error:库存为空，无法出库\n");
//		getchar();
//		return;
//	}
//	Product product = dequeue(queue);
//	printf("SUC>>商品出库成功\n");
//	printf("商品编号：%d\n", product.id);
//	printf("商品名称：%s\n", product.name);
//	printf("商品数量：%d\n", product.quantity);
//	char timeStr[100];
//	strftime(timeStr, 11, "%Y.%m.%d", &product.timeStruct);
//	printf("商品入库时间：%s", timeStr);
//}
void stockOut(Queue *queue) {
	printf("请输入商品编号：");
	int id;
	scanf("%d", &id);
	clearInputBuffer();
	Queue tempQueue;
	int found = 0;
	while (!isEmpty(queue)) {
		Product product = dequeue(queue);
		if (product.id == id) {
			found = 1;
			printf("商品名称：%s\n", product.name);
			printf("商品数量：%d\n", product.quantity);
			char timeStr[11];
//			convertToTimeStr(product.timestamp, timeStr);
			printf("商品入库时间：%s\n", timeStr);
			printf("请输入商品出库数量：");
			int quantity;
			scanf("%d", &quantity);
			clearInputBuffer();
			if (quantity > product.quantity) {
				printf("出库数量大于库存数量，无法出库\n");
				enqueue(&tempQueue, product);
			} else {
				product.quantity -= quantity;
				enqueue(&tempQueue, product);
				printf("商品出库成功\n");
			}
		} else {
			enqueue(&tempQueue, product);
		}
	}
	if (!found) {
		printf("未找到编号为%d的商品\n", id);
	}
	while (!isEmpty(&tempQueue)) {
		enqueue(queue, dequeue(&tempQueue));
	}
}
//查询商品数量
void queryQuantity(Queue *queue) {
	if (isEmpty(queue)) {
		printf("Error>>库存为空\n");
		getchar();
		return;
	}
	printf("当前库存商品数量：%d\n", queue->rear - queue->front + 1);
}
void printProducts(Queue *queue)

// 打印所有商品的编号、名称、数量和时间
{
	if (isEmpty(queue)) {
		printf("Error>>库存为空\n");
		getchar();
		return;
	}
	printf("============================================================\n");
	printf("商品编号\t商品名称\t商品数量\t商品入库时间\n");

	for (int i = queue->front; i <= queue->rear; i++) {
		SetCurPos(1, i + 9);
		printf("%d", queue->products[i].id);
		SetCurPos(16, i + 9);
		printf("%s", queue->products[i].name);
		SetCurPos(32, i + 9);
		printf("%d", queue->products[i].quantity);
		char timeStr[100] = {};
		strftime(timeStr, 11, "%Y.%m.%d", (&queue->products[i].timeStruct));
		SetCurPos(49, i + 9);
		printf("%s", timeStr);
		//printf("商品入库时间：%s", ctime(&queue->products[i].time));
		printf("\n");
	}
	getchar();
}
// 将字符串转换为时间结构体
int strToTime(const char *str, struct tm *timeStruct) {
	if (str == NULL || strlen(str) != 10) {
		return 0;
	}

	char yearStr[5];
	char monthStr[3];
	char dayStr[3];

	strncpy(yearStr, str, 4);
	strncpy(monthStr, str + 5, 2);
	strncpy(dayStr, str + 8, 2);

	yearStr[4] = '\0';
	monthStr[2] = '\0';
	dayStr[2] = '\0';

	int year = atoi(yearStr);
	int month = atoi(monthStr);
	int day = atoi(dayStr);

	timeStruct->tm_year = year - 1900;
	timeStruct->tm_mon = month - 1;
	timeStruct->tm_mday = day;
	timeStruct->tm_hour = 0;
	timeStruct->tm_min = 0;
	timeStruct->tm_sec = 0;

	return 1;
}
void convertToTimeStr(time_t timestamp, char *timeStr) {
	struct tm *tm = localtime(&timestamp);
	strftime(timeStr, 11, "%Y.%m.%d", tm);
}

void SetCurPos(int x, int y) {

	//获得光标参数
	COORD position{};
	//标准输出句柄
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//更改系统光标位置
	position.X = x;
	position.Y = y;
	//更改数据导入API
	SetConsoleCursorPosition(handle, position);

}
void sellProduct(Queue *queue) {
	int productId;
	printf("请输入售卖商品的编号:");
	scanf("%d", &productId);
	clearInputBuffer();
	int productIndex = -1;

	// 查找商品编号对应的商品在队列中的索引
	for (int i = queue->front; i <= queue->rear; i++) {
		if (queue->products[i].id == productId) {
			productIndex = i;
			break;
		}
	}

	if (productIndex == -1) {
		printf("Error>>没有该商品\n");
		getchar();
		return;
	}

	// 将对应商品的数量减一
	queue->products[productIndex].quantity--;

	if (queue->products[productIndex].quantity == 0) {
		// 商品数量为零，将该商品从队列中移除
		for (int i = productIndex; i < queue->rear; i++) {
			queue->products[i] = queue->products[i + 1];
		}
		queue->rear--;
		queue->size--;
	}

	printf("SUC>>成功售出\n");
	getchar();
}

//未做功能
//1.出库功能重写
//2.文件输出销售流水
//3.计算总营收所得
//4.product结构体增加单价和类型属性
//5.根据类型进行搜索
//6.通过文件导入导出库存信息
void cui_position_set(int x, int y) {
	//作用等同于SetCurPos()

	//获得光标参数
	COORD position{};
	//标准输出句柄
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//更改光标数据
	position.X = x;
	position.Y = y;
	//更改数据导入API
	SetConsoleCursorPosition(handle, position);
}

void cui_basic_line(int x, int y, int longdata, wchar_t symbol, int direct) {
	/*
		使用前提:
		本函数调用时候如果传入宽字符参数,必须事先使用
		setlocale(LC_ALL, "zh-CN");
		进行本地化语言设置

		direct参数可选项:
		0:竖向输出
		1:横向输出
	*/

	//定位至目标位置
	register COORD position{};
	register HANDLE handle;

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(handle, position);

	//判断工作模式
	if (direct) {
		for (register int i = 1; i <= longdata; i++)
			putwchar(symbol);
	} else {
		for (register int n = 1; n <= longdata; n++, y++) {
			position.Y = y;
			SetConsoleCursorPosition(handle, position);
			wcout << symbol;
		}
	}
}

void cui_square_frame(int x, int y, int lenth, int height, int model) {
	/*
	窗口参数:
	模式0外部识别区: x,y ---------- x+lenth-1,y
					 |                 |
			   x,y+height-1 ----- x+lenth-1,y+height-1


	*/
	cui_position_set(x, y);

	switch (model) {
		case 0:
			cui_basic_line(x, y, lenth, '=', 1);
			cui_basic_line(x, y + height - 1, lenth, '=', 1);
			cui_basic_line(x, y + 1, height - 2, L'║', 0);
			cui_basic_line(x + lenth - 2, y + 1, height - 2, L'║', 0);
			break;
	}

	switch (model) {
		case 0:
			cui_position_set(x, y);
			wcout << L"╔";
			cui_position_set(x + lenth - 2, y);
			wcout << L"╗";
			cui_position_set(x, y + height - 1);
			wcout << L"╚";
			cui_position_set(x + lenth - 2, y + height - 1);
			wcout << L"╝";
	}
}

void cui_basic_fill(int x, int y, int lenth, int height, wchar_t ch) {
	COORD position{};
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	position.X = x;
	position.Y = y;

	for (int h = 1; h <= height; h++) {

		position.Y = y + h - 1;
		SetConsoleCursorPosition(handle, position);
		for (int l = 1; l <= lenth; l++)
			wcout << ch;
	}
}

double cct_time_proc(char model) {
	/*
	精确时控自定义API
	工作模式:f 获取频率
			 s 起始计数器
			 e 末尾计数器
			 d 计算差值
			 z 以FPS显示
			 i 静态量重置
	*/
	LARGE_INTEGER time_union;
	static double time_freq, time_st, time_end, time_diff;

	switch (model) {
		case 'f':
			QueryPerformanceFrequency((LARGE_INTEGER*)&time_union);
			return time_freq = (double)time_union.QuadPart;
		case 's':
			QueryPerformanceCounter((LARGE_INTEGER*)&time_union);
			return time_st = (double)time_union.QuadPart;
		case 'e':
			QueryPerformanceCounter((LARGE_INTEGER*)&time_union);
			return time_end = (double)time_union.QuadPart;
		case 'd':
			return time_diff = (double)(time_end - time_st) / time_freq; //单位 ms
		case 'z':
			return  (1000.0 / (time_diff * 1000.0));
		case 'i':
			return time_freq = time_st = time_end = time_diff = 1;
		default :
			return 0;
	}
}

void csy_load_style(int backinfo, int foreninfo) {
	/*
	CMD中关于颜色的数学表示
	   0 = 黑色       8 = 灰色
	   1 = 蓝色       9 = 淡蓝色
	   2 = 绿色       A = 淡绿色
	   3 = 浅绿色     B = 淡浅绿色
	   4 = 红色       C = 淡红色
	   5 = 紫色       D = 淡紫色
	   6 = 黄色       E = 淡黄色
	   7 = 白色       F = 亮白色
	*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 16 * backinfo + foreninfo);
}

void csy_load_window(int x, int y, const char pageinfo[30], int backinfo, int foreinfo) {
	char cmd[100];
	sprintf(cmd, "mode con cols=%d lines=%d && title BSIF∣%s ", x, y, pageinfo);

	system(cmd);
	csy_load_style(backinfo, foreinfo);
	system("cls");
}

void  drawBlock(int startx, int starty, int endx, int endy, int backinfo, int foreninfo,int backinfoPAST, int foreinfoPAST) {
	csy_load_style(backinfo, foreninfo);
	for (int y = 0; y <= endy - starty; y++) {
		cui_position_set(startx, starty+y);
		for (int x = 0; x <= endx - startx; x++) {
			printf(" ");
		}
	}
	csy_load_style(backinfoPAST, foreinfoPAST);
}

int main() {
	Queue queue;
	int size;
	printf("请输入商品库存大小：");
	scanf("%d", &size);
	clearInputBuffer();
	initQueue(&queue, size);
	
	int choice;
	char ch[10];
	while (1) {
		system("cls");
		SetCurPos(0, 0);
		printf("1. 商品入库\n");
		printf("2. 商品出库\n");
		printf("3. 查询商品数量\n");
		printf("4. 打印所有商品\n");
		printf("6. 售卖商品\n");
		printf("5. 退出\n");
		printf("请输入您的选择：");
		ch[0] = getche();
		ch[1] = '\0';
		choice = atoi(ch);
		printf("\n");
		
		switch (choice) {
		case 1:
			stockIn(&queue);
			break;
		case 2:
			stockOut(&queue);
			break;
		case 3:
			queryQuantity(&queue);
			break;
		case 4:
			printProducts(&queue);
			break;
		case 5:
			printf("谢谢使用，再见！\n");
			exit(0);
		case 6:
			sellProduct(&queue);
			break;
		default:
			printf("Error>>无效选择，请重新输入\n");
			getchar();
			break;
		}
	}
	
	return 0;
}
