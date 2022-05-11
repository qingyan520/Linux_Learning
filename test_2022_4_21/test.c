#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXSIZE 100


#define MAXSIZE 100
//学生信息的定义：
typedef struct {
    char no[8];   //8位学号
    char name[20]; //姓名
    int score;     //成绩
}Student;

//顺序表的定义
typedef  struct {
    Student* elem;     //指向数据元素的基地址
    int  length;       //线性表的当前长度
    int size;          //记录当前已经写入的线性表长度
}SqList;


//初始化顺序表
void Init(SqList* l)
{
    l->elem = (Student*)malloc(sizeof(Student) * MAXSIZE);
    if (l->elem == NULL)
    {
        printf("malloc fail\n");
        exit(-1);
    }

    l->length = MAXSIZE;
    l->size = 0;
}


//刚开始时插入
void Push(SqList* l)
{
    printf("请输入要插入学生个数:\n");

    //插入n条数据
    int n = 0;
    scanf("%d", &n);

    //判断线性表是否满了
    if (n+l->size > l->length)
    {
        printf("要插入的数据数量大于线性表总长度\n");
  
    }

    //插入数据
    while (n--)
    {
        printf("请输入第%d位学生学号:\n",l->size+1);
        scanf("%s", l->elem[l->size].no);
        printf("请输入第%d位学生姓名:\n",l->size+1);
        scanf("%s", l->elem[l->size].name);
        printf("请输入第%d位学生成绩:\n",l->size+1);
        scanf("%d", &l->elem[l->size].score);
        printf("\n");
        l->size++;
    }
    printf("已经插入完成！\n");
}

//2.显示学生数据
void Print(SqList* l)
{
    printf("\t\t    学生信息显示\n");
    printf("\t+--------+--------------------+-----+\n");
    printf("\t|%-8s|%-20s|%-5s|\n", "id", "name", "score");
    printf("\t+--------+--------------------+-----+\n");


    //没有学生信息时就不要打印了
    if (l->size == 0)
    {
        printf("\t|            暂无学生信息           |\n");
    }
    else
    {
        //循环打印数据
        for (int i = 0; i < l->size; i++)
        {
            printf("\t|%-8s|%-20s|%-5d|\n", l->elem[i].no, l->elem[i].name, l->elem[i].score);
            if (i != l->size - 1)
            {
                printf("\t+--------+--------------------+-----+\n");
            }
        }
    }
    printf("\t+--------+--------------------+-----+\n");
  
}

//打印单个学生信息
void Print_Person(char* id, char* name, int score)
{
    printf("\t\t    学生信息显示\n");
    printf("\t+--------+--------------------+-----+\n");
    printf("\t|%-8s|%-20s|%-5s|\n", "id", "name", "score");
    printf("\t+--------+--------------------+-----+\n");
    printf("\t|%-8s|%-20s|%-5d|\n",id,name,score);
    printf("\t+--------+--------------------+-----+\n");
  
}



//3.根据姓名进行查找，返回学生的学号和成绩
int Find_Name(SqList* l, char*name)
{
    for (int i = 0; i < l->size; i++)
    {
        if (strcmp(l->elem[i].name, name) == 0)
            return i;
    }
    return -1;
}

void Search(SqList* l)
{
    printf("请输入要查找的姓名:\n");
    char name[20] = { 0 };
    scanf("%s", name);
    int ret = Find_Name(l, name);
    if (ret == -1)
    {
        printf("未找到该学生");
        system("pause");
        system("cls");
        return;
    }
    else
    {
        //打印学生信息
        Print_Person(l->elem[ret].no, l->elem[ret].name, l->elem[ret].score);
    }

}


//4.返回指定位置的学生信息
void Location(SqList* l)
{
    printf("请输入位置下标:\n");
    int n = 0;
    scanf("%d", &n);

    //超出范围
    if (n<1 || n>l->size)
    {
        printf("下标超出范围，无法返回详细信息\n");
        system("pause");
        system("cls");
        return;
    }
    else
    {
        Print_Person(l->elem[n-1].no, l->elem[n-1].name, l->elem[n-1].score);
    }
}


//5.指定位置插入
void Insert(SqList* l)
{
    printf("输入要插入的位置:\n");
    int n;
    scanf("%d", &n);

    //超出范围
    if (n<1 || n>l->size)
    {
        printf("超出范围，无法插入\n");
        system("pause");
        system("cls");
        return;
    }

    if (l->size == l->length)
    {
        printf("线性表已满，无法插入\n");
        system("pause");
        system("cls");
        return;
    }

    n--;

    int i = l->size - 1;
    while (i >= n)
    {
        l->elem[i + 1] = l->elem[i];
        i--;
    }

    printf("请输入要插入的学生学号:\n");
    scanf("%s", l->elem[n].no);
    printf("请输入要插入的学生姓名:\n");
    scanf("%s", l->elem[n].name);
    printf("请输入要插入的学生成绩:\n");
   scanf("%d", &l->elem[n].score);
    l->size++;
    printf("插入完成！");
  
}


//6.删除指定位置的学生
void Pop(SqList*l)
{
    printf("请输入要删除的位置:\n");
    int n;
    scanf("%d", &n);
    if (n<1 || n>l->size)
    {
        printf("要删除位置超出范围\n");
        return;
    }
    n--;
    for (int i = n; i < l->size - 1; i++)
    {
        l->elem[i] = l->elem[i + 1];
    }
    l->size--;
    printf("删除成功\n");
    
}


//7.统计学生个数
int Size(SqList* l)
{
    printf("当前学生个数为:%d\n", l->size);
    
}

void Menu()
{
    printf("\t\t1.显示学生信息\n");
    printf("\t\t2.根据姓名查找学生信息\n");
    printf("\t\t3.查找指定位置学生\n");
    printf("\t\t4.指定位置插入信息\n");
    printf("\t\t5.指定位置删除信息\n");
    printf("\t\t6.统计学生个数\n");
    printf("\t\t0.退出\n");
}



int main()
{
    SqList l;
    Init(&l);
    Push(&l);
    int option = 0;
    while (1)
    {
        Menu();
        printf("请选择对应选项:\n");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            Print(&l);
            break;
        case 2:
            Search(&l);
            break;
        case 3:
            Location(&l);
            break;
        case 4:
            Insert(&l);
            break;
        case 5:
            Pop(&l);
            break;
        case 6:
            Size(&l);
            break;
        case 0:
            exit(0);
            break;
           default:
            printf("输入错误，请重新输入\n");
                break;
        }
    }
    
}


