#include <stdio.h>
#include <signal.h> 
#include <unistd.h>
#include "include.h"
#include <sys/time.h>

extern char pos_y,pos_x;

extern struct brick graph[19];
extern struct background square[ROW+1][COLUMN+1];
char row_count;		//用于记录有多少满行
char bottom = ROW-1;		//第一个满行的行号
char top = ROW-4;			//最高的非空行的行号
char stop_flag = 0;     //停止标志位，置1时，从新下落方块
char die_flag = 0;
int score = 0;
extern char block_ID;
extern char next_block_ID;
//删除满行
void eraser_full_row(int row)
{
	int i, j, line_count;
	//把该行清空
	for(i = 2; i < COLUMN; i++)     //清零
	{
		square[row][i].val = 0;
	}
	//把上面的行向下移动
    for(j = bottom; j > 2; j--)
    {
        for(i = 2; i < COLUMN; i++)
        {
            square[j][i].val = square[j-1][i].val;
            square[j][i].color = square[j-1][i].color;
        }
    }
	//分数增加
	score++;
	//显示分数
	printf("\033[%d;%dH\033[0m%03d", 13, COLUMN+11, score);
	fflush(NULL);
}
//对每一行进行检查，是否满了
void check_full_row()
{
	int i, j;
	char full_dot = 0;				//记录每行中值为1的点的个数
    char break_flag = 0;

	for(i = ROW-1; i > 1; i--)
	{
        full_dot = 0;
		for(j = 2; j < COLUMN; j++)
		{
			if(square[i][j].val)
			{
				full_dot++;
			}
            
			if(full_dot == COLUMN-2)
			{
                if(i == 2)
				full_dot = 0;
				bottom = i;
				eraser_full_row(i);
                continue;
			}  
        }
    }
}

//填充背景, 更新数据
//方块固定不动之后，就变成背景
void fill_square()
{
	char i, j, temp;
    //碰到最低行，停止
    for(i = 3; i >= 0; i--)
    {
        for(j = 7; j >= 0; j--)
        {
            if(graph[block_ID].blocks[i][j] == 1 && square[pos_y+i+1][pos_x+j].val == 1)         //假设下移动一位
                stop_flag = 1;
        }
    }
    if(stop_flag == 1)
	{
        stop_flag = 0;
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 8; j++)
			{
				if(graph[block_ID].blocks[i][j])			//更新底板数据并显示
				{
					square[pos_y+i][pos_x+j].val = 1;
					square[pos_y+i][pos_x+j].color = graph[block_ID].color;
					printf("\033[%d;%dH\033[%dm \033[0m", pos_y+i, pos_x+j, graph[block_ID].color);

				}
			}
		}
        fflush(NULL);
		for(i = 2; i < COLUMN-1; i++)
		{
			if(square[2][i].val == 1)		//到顶端，游戏结束
			{
				die_flag = 1;
				break;
			}
		}
        while(1)        //方块随机位置下落
        {
            temp = random()%(COLUMN-7);		//方块随机坐标
            if(temp > (COLUMN -7) || temp < 2 || temp%2==1)
                continue;
			block_ID = next_block_ID;
			get_block_ID();					//方块编号
            break;
        }
  		pos_y = 1;
		pos_x = temp;
	}
}

void set_timer(int ms) 
{
    struct itimerval val;

    val.it_interval.tv_sec = ms/1000;
    val.it_interval.tv_usec = ms%1000*1000; //micro
    val.it_value.tv_sec = ms/1000;  //  
    val.it_value.tv_usec = ms%1000*1000;
    setitimer(ITIMER_REAL, &val, NULL);
}

void sig_func()
{
	pos_y++; 
	fill_square();  //更新底板数据
	clear_block();      
	check_full_row();
    int i, j;
    printf("\n");
	draw_block();
}

