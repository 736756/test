

#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#define random(x) (rand()%x)
using namespace std;

int **map;
char c = 0;
int sign = 0;        //全局变量声明，判断游戏是否结束
void print_regulation()
{
    cout<<"一、贪吃蛇游戏规则："<<endl;
    cout<<"1.分别通过w/s/a/d按键来控制撒上/下/左/右方向"<<endl;
    cout<<"2.撞到边界或者地图部分则游戏结束"<<endl;
    cout<<"3.通过数字按键（1/2/3）来选择不同的地图和难度"<<endl;
}
int choice_map()
{
    int k;
    cout<<"二、请选择您需要的地图(1/2/3/4)"<<endl;
    cout<<"1.基础模型地图"<<endl;
    cout<<"2.“仙”字地图"<<endl;
    cout<<"3.“中”字地图"<<endl;
    cout<<"4.“帅”字地图"<<endl;
    cin>>k;
    return k;
}
int print_difficulty()
{
    int n;
    cout<<"三、请输入您想要的困难程度（1/2/3）"<<endl;
    cout<<"*****1.入门"<<endl<<"*****2.简单"<<endl<<"*****3.困难"<<endl;
    cin>>n;
    return n;
}

char check_snake(char c);

struct snake{    
    int i;
    int j;
    struct snake *next;
    };

struct snake *generateSnake()        //定义一个表示当前蛇位置的函数，并返回指向当前蛇类（结构体）的的指针
{
    struct snake *s;
    s = (struct snake*)malloc(sizeof(struct snake));
    struct snake *p = s;
    int x[6] = {2, 3, 4, 5, 5, 5};
    int y[6] = {2, 2, 2, 2, 3, 4};
    for(int i = 0; i < 6; i++){
        struct snake *node; //节点
        node = (struct snake*)malloc(sizeof(struct snake));
        p->next = node;        
        p = p->next;
        p->i = x[i];
        p->j = y[i];
    }
    p->next = NULL;//系统自定义的一个特殊的0，防野指针 
    return s;
}  // 设置蛇的长度和初始位置

void gotoxy(int x, int y)
{
    COORD Position;
    Position.X = x;
    Position.Y = y;
    //调用API改变字体位置
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
}

// 绘制地图
void draw_map(int **map, int height, int weight,int k){

    for(int i = 0; i < height ; ++i){
        for(int j = 0; j < weight ; ++j){
            if(i == 0  || i == height - 1  ){  // 表示在第一层和最后一层  
                map[i][j] = 1;    // 四边形的四边作为墙，强我们用1表示，表示位置已占
            }
            else if ( j == 0 ||  j == weight - 1 ){ // 表示在最左层和最右层
                map[i][j] = 1;
            }
            else{
                map[i][j] = 0 ; // 除墙以外其他位置为0 表示该位置为空
            }
        }
        for(int i = 0; i < height ; ++i){
        for(int j = 0; j < weight ; ++j)
        {
            if(k==2){
                if(((i==4||i==5)&&j==5)||(i==6&&j==4)||(i==7&&j==3)            //“仙”字地图绘制k=2
                ||(i==8&&j==2)||(j==4&&(i<=15&&i>=7)))
                {map[i][j]=1;
                map[10][5]=0;}
                else if(((j==7&&i>=7&&i<=15)||(j==15&&i>=7&&i<=15)||
                (j==11&&i>=4&&i<=15)||(i==15&&j>=7&&j<=15)))
                {map[i][j]=1;
                 map[10][8]=0;
                 map[10][9]=0;
                 map[10][10]=0;
                 map[10][16]=0;
                 map[10][17]=0;
                 map[10][18]=0;
                 map[4][12]=0;
                 map[5][12]=0;}}
            else if(k==1){
                if((j==weight-8 && (i==height-15 || i==height-16 ||            //基础地图绘制k = 1
                i==height-17 || i==height-18 || i==height-19)))
                {map[i][j]=1;}
                else if((i==height-15)&& (j==weight-2 || j==weight-3
                ||j==weight-4))
                {map[i][j]=1;}
                else if((i==height-10)&&(j<=5||j>=13||(j<=11&&j>=7)))
                {map[i][j]=1;}
            }
            else if(k==3){                                                    //“中”字绘制地图k = 3
                if((j==4&&(i>=6&&i<=10))||(j==16&&(i>=6&&i<=10))||(i==11&&j>=4&&j<=16)
                    ||(i==6&&j>=5&&j<=15)||(j==10&&i>=3&&i<=16))
                {map[i][j]=1;
                map[9][4]=0;
                map[6][9]=0;
                map[6][11]=0;
                map[9][16]=0;}
            }
            else if(k=4){
                if((j==3&&i>=7&&i<=11)||(j==6&&i>=4&&i<=13)||(j==9&&i>=6&&i<=12)||//“帅”字地图绘制k = 4 
                    (j==17&&i>=6&&i<=12)||(i==6&&j>=10&&j<=16)||(j==13&&i>=4&&i<=16))
                {map[i][j]=1;
                 map[14][5]=1;
                 map[15][4]=1;}
            }
        }}
        cout << "\n";
    }
    return;
}
// 图形形状
void draw(int **map,int *star, int height, int weight){
    system("cls");//清空屏幕
    gotoxy(0, 0);
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < weight; ++j){
            if(map[i][j] == 1){
             cout << "■";
            }
            else if (map[i][j] == 2)
            {
                cout << "●";
            }
            else if ( i == star[0] && j == star[1])
            {
                cout << "★";
            }
            else{
             cout << "  ";
            }
        }
        cout << '\n';
    }
    return;
}

// 蛇
void drawSnake(int **map, struct snake *s){
    struct snake *p = s->next;
    map[p->i][p->j] = 2;    // 蛇头
    p = p->next;
    while(p){
        map[p->i][p->j] = 1;
        p = p->next;
    }
    return;
}


void draw_star(int **map,int *star, int height, int weight){
    int sum = 0; // 空格子的个数
    int index = 0; // 剩下的格子

    // 算出空格子的个数
    for(int i = 0; i < height ; ++i){
        for(int j = 0; j < weight; ++j){
            if(map[i][j] == 0)
            {
                sum += 1;
            }
        }
    }
    index = random(sum) + 1; // 在1到sum 中随机的位置生成星星
    //生成星星
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < weight; ++j){
            if(map[i][j] == 0){
                index -= 1;
            }
            if(index == 0){
                star[0] = i;
                star[1] = j;
                return ;
            }
        }
    }
    return;
}

void update_snake(char c,struct snake *s,int *star)
{
    snake *newsnake;
    newsnake = (snake*)malloc(sizeof(snake));
    if(c == 'w'){
        // 相当于在s 头插一个新节点
        newsnake->i = s->next->i - 1;
        newsnake->j = s->next->j; 

        if(map[newsnake->i][newsnake->j] == 1){
            // 头插的节点的位置 原本 map[i][j] == 1 表示该位置是墙或者是蛇身了
             sign = 3;  
        }
        else if(newsnake->i == star[0] && newsnake->j == star[1]){
            newsnake->next = s->next; 
            s->next = newsnake; 
            struct  snake *q = s; 
                while(q->next->next != NULL){
                    gotoxy(q->i, q->j);
                    printf("■");  // 恢复蛇身
                    q = q->next;
                }
            sign = 2;
        }
        else{
            newsnake->next = s->next; 
            s->next = newsnake; 
            struct  snake *q = s;   
            while(q->next->next != NULL){
                gotoxy(q->i, q->j);
                printf("■"); 
                q = q->next;
            }
            gotoxy(q->i, q->j);
            printf("■");
            map[q->next->i][q->next->j] = 0; 
            free(q->next);        
            q->next = NULL;
        }
    }
    else if( c == 's'){
        newsnake->i = s->next->i + 1;
        newsnake->j = s->next->j;
        if(map[newsnake->i][newsnake->j] == 1){
            sign = 3;  
        }
        if(newsnake->i == star[0] && newsnake->j == star[1]){
            newsnake->next = s->next; 
            s->next = newsnake;
            struct  snake *q = s; 
            while(q->next->next != NULL){
                gotoxy(q->i, q->j);
                printf("■"); 
                q = q->next;
            }
            sign = 2;
        }
        else{
            newsnake->next = s->next; 
            s->next = newsnake; 
            struct  snake *q = s;   
            while(q->next->next != NULL){
                gotoxy(q->i, q->j);
                printf("■"); 
                q = q->next;
            }
            gotoxy(q->i, q->j);
            printf("■");
            map[q->next->i][q->next->j] = 0; 
            free(q->next);        
            q->next = NULL;
        }
    }
    else if( c == 'a'){
        newsnake->i = s->next->i;
        newsnake->j = s->next->j - 1;
        if(map[newsnake->i][newsnake->j] == 1){
            sign = 3;  
        }        
        if(newsnake->i == star[0] && newsnake->j == star[1]){
            newsnake->next = s->next; 
            s->next = newsnake; 
            struct  snake *q = s;
            while(q->next->next != NULL){
                gotoxy(q->i, q->j);
                printf("■"); 
                q = q->next;
            }
            sign = 2;
        }
        else{
            newsnake->next = s->next; 
            s->next = newsnake; 
            struct  snake *q = s;   
            while(q->next->next != NULL){
                gotoxy(q->i, q->j);
                printf("■"); 
                q = q->next;
            }
            gotoxy(q->i, q->j);
            printf("■");
            map[q->next->i][q->next->j] = 0;
            free(q->next);         
            q->next = NULL;
        } 
    }       
    else{
        newsnake->i = s->next->i;
        newsnake->j = s->next->j + 1;
        if(map[newsnake->i][newsnake->j] == 1){
            sign = 3; 
        }        
        if(newsnake->i == star[0] && newsnake->j == star[1]){
            newsnake->next = s->next; 
            s->next = newsnake;
            struct  snake *q = s;
            while(q->next->next != NULL){
                gotoxy(q->i, q->j);
                printf("■"); 
                q = q->next;
            }
            sign = 2;
        }
        else{
            newsnake->next = s->next; 
            s->next = newsnake; 
            struct  snake *q = s;   
            while(q->next->next != NULL){
                gotoxy(q->i, q->j);
                printf("■"); 
                q = q->next;
            }
            gotoxy(q->i, q->j);
            printf("■");
            map[q->next->i][q->next->j] = 0; 
            free(q->next);           
            q->next = NULL;
        }  
    }            
}

// 蛇 移动方向
// 上 119 wd
// 下 115 s
// 左 97  a
// 右 100 d
void action_snake(struct snake *s,int *star){
        switch(c)
        {
            //向上
            case 'w':
            update_snake(c, s,star);
            break;
            //向下
            case 's':
            update_snake(c, s,star);
            break;
            //向左
            case 'a':
            update_snake(c, s,star);
            break;
            //向右
            case 'd':
            update_snake(c, s,star);
            break;
        }
}

void check_snake(int n){
    DWORD time = n; // 1表示一毫秒 1000表示一秒  要改变速度可以修改time的时间
    DWORD time_start = GetTickCount();  // 获取当前时间
    char t;
    while(true){
        if(_kbhit()){
            char ch = _getch(); // 当前键位
            if(ch == 97 || ch == 100 || ch == 115 || ch == 119){
                t = ch;  
                if( t == 97 && c == 'd')
                {
                    c = 'd';
                }
                else if( t == 100 && c =='a'){
                    c ='a';
                }
                else if(t == 115 && c == 'w'){
                    c = 'w';
                }
                else if(t == 119 && c == 's'){
                    c = 's';
                }
                else{
                    c = ch;
                }
            }
        }
        DWORD time_end = GetTickCount(); // 获取键位后的时间
        if(time_end - time_start > time){
            time_start = time_end; 
            break;
        }
    }
    return;
}

int main()
{
    system("chcp 65001");
    int n,k;
    print_regulation();
    cout<<'\n';
    k=choice_map();
    switch(print_difficulty())
    {
    case 1:n=500;break;
    case 2:n=300;break;
    case 3:n=200;break;
    }
    int height = 20; // 地图高度  map.height = 20
    int weight = 20; // 地图宽度  map.weight = 20

    struct snake *s = generateSnake();     //声明一个指向蛇类（结构体）指针s，并访问表示当前蛇位置的函数，将表示蛇坐标的结构体地址赋给s
    map = (int **) malloc(sizeof(int*)*height);//申请一个动态空间用来保存地图参数（高）
    for(int i = 0; i < height; ++i)
    {
        map[i] = (int *) malloc(sizeof(int)*weight);//循环申请动态内存来保存地图的宽
    }    
    draw_map(map,height,weight,k); //调用绘制地图的函数，并传递地图参数
    drawSnake(map,s);             //绘制蛇，并传递蛇的位置
    int star[2] = {0,0};
    draw_star(map,star,height,weight);//随机生成星星的位置
    draw(map,star,height,weight);
    while(1)                    //循环操作AWSD来控制蛇的移动，循环生成地图
    {
        if( sign == 3)            //如果撞到墙，或蛇身全局变量sign会被置为3，此时则结束循环
        {
            break;
        }
        check_snake(n);
        action_snake(s,star);

        draw_map(map,height,weight,k);
        drawSnake(map,s);
        if(sign == 2)            //若没有撞到墙和蛇身，则循环继续生成地图和蛇
        {
            draw_star(map,star,height,weight);
            sign = 0;
        }
        draw(map,star,height,weight);
        gotoxy(0, 0);
    }

    system("cls");
    gotoxy(height/2,weight/2);
    cout << "游戏结束 " ;
    system("pause");
    return 0;
}
