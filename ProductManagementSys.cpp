#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <iostream>
#define MAX_PRODUCTS 100
#define MAX_PAGELINE 16
using namespace std;

typedef struct
{
    int id;        // 编号
    char name[50]; //,名称
    float price;
    int quantity;  // 数量
    char date[20]; // 日期
    bool valid;    // 有效标识
} Product;

Product products[MAX_PRODUCTS];

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
void cui_position_set(int x, int y)
{
    // 获得光标参数
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD position{};
    // 标准输出句柄
    HANDLE handle;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(handle, &csbi);

    // 更改光标数据
    if (x != -1)
        position.X = x;
    else
        position.X = csbi.dwCursorPosition.X + 1;
    if (y != -1)
        position.Y = y;
    else
        position.Y = csbi.dwCursorPosition.Y + 1;

    // 更改数据导入API
    SetConsoleCursorPosition(handle, position);
}
void csy_load_style(int backinfo, int foreninfo)
{
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
void drawBlock(int startx, int starty, int endx, int endy, int backinfo, int foreninfo, int backinfoPAST, int foreinfoPAST)
{
    csy_load_style(backinfo, foreninfo);
    for (int y = 0; y <= endy - starty; y++)
    {
        cui_position_set(startx, starty + y);
        for (int x = 0; x <= endx - startx; x++)
        {
            printf(" ");
        }
    }
    csy_load_style(backinfoPAST, foreinfoPAST);
}
bool OpenANSIControlChar()
{ // 启动控制台ANSI转义序列模块
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStd == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    // 获取控制台模式
    DWORD dwMode = 0;
    if (!GetConsoleMode(hStd, &dwMode))
    {
        return false;
    }
    // 增加控制台模式的选项：启用虚拟终端
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hStd, dwMode))
    {
        return false;
    }
    return true;
}
void showMenu()
{
    printf("\n");
    printf("欢迎使用商品管理系统！\n");
    printf("1. 商品入库\n");
    printf("2. 商品售卖\n");
    printf("3. 流水统计\n");
    printf("4. 数据保存\n");
    printf("5. 商品删除\n");
    printf("0. 退出系统\n");
    printf("请选择操作：");
}
int findProductIndex(int id)
{
    for (int i = 0; i < MAX_PRODUCTS; i++)
    {
        if (products[i].valid && products[i].id == id)
        {
            return i;
        }
    }
    return -1; // 如果找不到指定ID的商品，返回-1
}
void addProduct(int id, const char *name, float price, int quantity, const char *date)
{
    int index = -1;
    for (int i = 0; i < MAX_PRODUCTS; i++)
    {
        if (!products[i].valid)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        products[index].id = id;
        strcpy(products[index].name, name);
        products[index].price = price;
        products[index].quantity = quantity;
        strcpy(products[index].date, date);
        products[index].valid = true;
        printf("商品已成功入库！\n");
    }
    else
    {
        printf("库存已满，无法入库新商品！\n");
    }
}
void getInputAndAddProduct()
{
    int id, quantity;
    float price;
    char name[50], date[26];

    printf("请输入产品ID：  \b\b");
    scanf("%d", &id);
    if (id == -1)
    {
        return;
    }
    printf("请输入产品名称：");
    scanf("%s", name);
    printf("请输入产品单价：");
    scanf("%f", &price);
    printf("请输入产品数量：");
    scanf("%d", &quantity);
    time_t currentTime = time(nullptr);
    struct tm *timeInfo = localtime(&currentTime);
    char timeBuffer[20];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y.%m.%d-%H:%M:%S", timeInfo);

    addProduct(id, name, price, quantity, timeBuffer);
}
void removeProduct(int id)
{
    int index = -1;
    for (int i = 0; i < MAX_PRODUCTS; i++)
    {
        if (products[i].valid && products[i].id == id)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        products[index].valid = false;
    }
    else
    {
        printf("找不到指定ID的商品！\n");
        getchar();
    }
}

void findProduct(int id)
{
    int index = -1;
    for (int i = 0; i < MAX_PRODUCTS; i++)
    {
        if (products[i].valid && products[i].id == id)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        printf("商品ID：%d\n", products[index].id);
        printf("商品名称：%s\n", products[index].name);
        printf("商品价格：%.2f\n", products[index].price);
        printf("商品数量：%d\n", products[index].quantity);
        printf("入库时间：%s\n", products[index].date);
    }
    else
    {
        printf("找不到指定ID的商品！\n");
        getchar();
    }
}

void printInventory()
{
    printf("商品ID  商品名称              商品价格  商品数量  入库时间\n");
    for (int i = 0; i < MAX_PRODUCTS; i++)
    {
        if (products[i].valid)
        {
            printf("%-8d %-20s %-10.2f %-8d %s\n", products[i].id, products[i].name, products[i].price, products[i].quantity, products[i].date);
        }
    }
}

void sellProduct(int id, int quantity)
{
    int index = findProductIndex(id);
    if (index != -1)
    {
        if (products[index].quantity >= quantity)
        {
            products[index].quantity -= quantity;
            if (products[index].quantity == 0)
                removeProduct(id);
            printf("商品已成功售卖！\n");
            getchar();

            FILE *file = fopen("sales.txt", "a");
            if (file == NULL)
            {
                printf("无法打开流水文件！\n");
                getchar();
                return;
            }

            time_t currentTime = time(NULL);
            struct tm *localTime = localtime(&currentTime);
            char timeString[100];
            strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);

            float totalPrice = products[index].price * quantity;

            fprintf(file, "%s|ID：%d|数量：%d|单价：%.2f|总计金额：%.2f\n", timeString, products[index].id, quantity, products[index].price, totalPrice);

            fclose(file);
        }
        else
        {
            printf("商品库存不足，无法售卖！\n");
            getchar();
            getchar();
        }
    }
    else
    {
        printf("找不到指定ID的商品！\n");
        getchar();
    }
}

void readInventoryFromFile()
{
    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL)
    {
        printf("未查找到库存文件！若是第一次使用请忽略该提示,程序已自动创建库存文件\n");
        getchar();
        return;
    }

    int id, quantity;
    char name[100], date[100];
    float price;

    while (fscanf(file, "%d %s %f %d %s", &id, name, &price, &quantity, date) != EOF)
    {
        addProduct(id, name, price, quantity, date);
    }

    fclose(file);
}
void writeInventoryToFile()
{
    FILE *file = fopen("inventory.txt", "w");
    if (file == NULL)
    {
        printf("无法打开文件！\n");
        getchar();
        return;
    }

    for (int i = 0; i < MAX_PRODUCTS; i++)
    {
        if (products[i].valid)
        {
            fprintf(file, "%d %s %.2f %d %s\n", products[i].id, products[i].name, products[i].price, products[i].quantity, products[i].date);
        }
    }

    fclose(file);
}
void calculateSalesStats()
{
    FILE *file = fopen("sales.txt", "r");
    if (file == NULL)
    {
        printf("无法打开文件！\n");
        getchar();
        return;
    }

    int salesCount = 0;
    float totalSalesAmount = 0.0;

    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        salesCount++;

        float amount;
        if (sscanf(line, "%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^0-9.]%f", &amount) != 1)
        {
            printf("无法解析金额！\n");
            getchar();
            continue;
        }
        totalSalesAmount += amount;
    }

    fclose(file);

    printf("售卖次数：%d\n", salesCount);
    printf("销售总金额：%.2f\n", totalSalesAmount);
}
inline void SetConsoleWindowPosition(int x, int y)
{ // 基于Windows API设置窗口位置 ,参数为-1时为居中显示
    HWND hwnd = GetConsoleWindow();
    RECT r;
    GetWindowRect(hwnd, &r);

    int width = r.right - r.left;
    int height = r.bottom - r.top;

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    if (x == -1)
        x = (screen_width - width) / 2;
    if (y == -1)
        y = (screen_height - height) / 2;

    MoveWindow(hwnd, x, y, width, height, TRUE);
}
int main(void)
{
    SetConsoleTitle("ProductManagementSystem.exe");
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    SetWindowLongPtrA(GetConsoleWindow(),
                      GWL_STYLE,
                      GetWindowLongPtrA(GetConsoleWindow(),
                                        GWL_STYLE) &
                          ~WS_SIZEBOX &
                          ~WS_MAXIMIZEBOX &
                          ~WS_MINIMIZEBOX);
    SetConsoleWindowPosition(-1, -1);

    readInventoryFromFile();

    int choice;
    int pageIndex = 1;
    const int pageMax = MAX_PRODUCTS / MAX_PAGELINE + (MAX_PRODUCTS % MAX_PAGELINE > 0 ? 1 : 0);

    while (true)
    {
        system("cls");
        system("color bd");
        time_t currentTime;
        time(&currentTime);
        struct tm *localTime = localtime(&currentTime);
        int year = localTime->tm_year + 1900;
        int month = localTime->tm_mon + 1;
        int day = localTime->tm_mday;
        int hour = localTime->tm_hour;
        int minute = localTime->tm_min;
        int second = localTime->tm_sec;
        system("mode con cols=110 lines=18");
        printf("\t\t\t商品管理系统 V1.1");
        printf("    %d-%d-%d-%d:%d:%d \t %d\n", year, month, day, hour, minute, second, pageIndex);
        drawBlock(30, 1, 105, 16, 0x3, 0xe, 0x3, 0xe);
        cui_position_set(30, 1);
        printf("商品ID  商品名称              商品价格  商品数量   入库时间");
        cui_position_set(30, 2);
        int productIndex = (pageIndex - 1) * MAX_PAGELINE;
        for (int i = 0; i < MAX_PAGELINE, productIndex < MAX_PRODUCTS; i++, productIndex = (pageIndex - 1) * MAX_PAGELINE + i)
        {

            if (products[productIndex].valid)
            {
                printf(" %-8d %-20s %-10.2f %-8d %s", products[productIndex].id, products[productIndex].name, products[productIndex].price, products[productIndex].quantity, products[productIndex].date);
                cui_position_set(30, -1);
            }
        }
        csy_load_style(0xb, 0x5);
        cui_position_set(0, 0);
        showMenu();

        choice = getch() - '0'; // 使用getche函数获取用户输入的选择
        std::cout << choice;

        switch (choice)
        {
        case 1:
            printf("\n");
            getInputAndAddProduct();
            break;
        case 2:
            int id, quantity;
            printf("\n请输入要出售的商品ID：");
            scanf("%d", &id);
            printf("请输入要出售的商品数量：");
            scanf("%d", &quantity);
            sellProduct(id, quantity);
            break;
        case 3:
            printf("\n");
            calculateSalesStats();
            getchar();
            break;
        case 4:
            writeInventoryToFile();
            break;
        case 5:
            int ID;
            printf("\n请输入出库商品的ID");
            scanf("%d", &ID);
            removeProduct(ID);
            break;
        case 0:
            printf("\n感谢使用商品管理系统，再见！\n");
            goto outofloop;
        case 176:
        {

            choice = getch() - '0';
            cout << choice;
            if (choice == 27 && pageIndex > 1)
                pageIndex--;
            else if (choice == 29 && pageIndex < pageMax)
                pageIndex++;
        }
        break;
        default:
            printf("\n无效的选择，请重新输入。\n");
            getchar();
            break;
        }

        printf("\n");
    }
outofloop:
    writeInventoryToFile();
    return 0;
}