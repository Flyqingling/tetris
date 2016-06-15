#include <stdio.h>
#include "include.h"
#include "time.h"
char ch;
int pos_x = 10, pos_y = 2;
char x_record, y_recore;
char forbid_flag = 0;
char rotate_flag = 1;
char temp = 0;
extern char die_flag;	//游戏结束标志
extern char stop_flag; 
extern char block_ID;
extern struct brick graph[19];
extern struct background square[ROW+1][COLUMN+1];
//改变方块编号ID
//这里要用到指针，否真不能改变传入参数的值
//或者使用下面的返回值的形式，再赋给传入的参数
void change_brick(char *ID)
{
    switch (*ID)
	{
		case 4:				//L
    		*ID = 0;
    		break;
		case 8:				//J
			*ID = 4;
			break;
		case 12:			//T
			*ID = 8;
			break;
		case 14:
    		*ID = 12;
			break;
		case 16:
			*ID = 14;
			break;
		case 18:
			*ID = 16;
			break;
        case 19:
            *ID = 18;
            break;
   	}
}
//清除方块,从新刷新背景 
void clear_block()
{
	int x, y;
    for(y = ROW; y > 0; y--)
    {
        for(x = COLUMN; x > 0; x--)
        {
			printf("\033[%d;%dH", y, x);
			if(square[y][x].val)
			{
                printf("\033[%dm \033[0m", square[y][x].color);
			}
		    else
				printf("\033[0m ");
        }
    }
}
//画方块
void draw_block()
{
	int x, y;	   
    for(y = 0; y < 4 ; y++)
    {
        for(x = 0; x < 8; x++)
        {
            printf("\033[%d;%dH",pos_y +y,pos_x +x);
            if(graph[block_ID].blocks[y][x])
            {
                printf("\033[%dm ", graph[block_ID].color);
            }
        }
    }
	fflush(NULL);
}

void get_dir()
{
	int i, j;
	while(1)
	{
		//如果死了就退出
		if(die_flag)
		{
			printf("\033[%d;%dH\033[0m\033[31mGAME OVER\n", 9, COLUMN+8);
			break;
		}
		//获取输入
    	ch = getchar();
    	if(ch == 'q')
		{
			break;
		}
    	if(ch != '\033')
        	continue;
    	if(getchar() != '[')
        	continue;
    	ch = getchar();
		clear_block();
    	switch (ch)
    	{	
        	case 'A':
				/*  假设旋转之后的图形 */
				temp = block_ID+1;
				//改变编号
				change_brick(&temp);
				//判断旋转之后方块是否会和背景上的图形想重叠
                for(i = 0; i < 4; i++)
                {
                    for(j = 0; j < 8; j++)
                    {
						//如果重叠就不进行旋转
                        if(graph[temp].blocks[i][j] ==1 && square[pos_y+i][pos_x+j].val == 1)
                        {
                            rotate_flag = 0;
                        }
                    }
                    if(rotate_flag == 0)
                    {
                        break;
                    }    
                }
				//如果假设成立，就不进行旋转，退出
                if(rotate_flag == 0)
                {
                    rotate_flag = 1;
                    break;
                }
				//假设不成立，就进行旋转
                block_ID++;
	  			change_brick(&block_ID);
               	break;
	        case 'B':
    	            pos_y+=1;       //down
        	    break;
        	case 'C':
				for(i = 0; i < 4; i++)
				{
					for(j = 0; j < 8; j++)
					{
						if(graph[block_ID].blocks[i][j]==1 && square[pos_y+i][pos_x+j+1].val==1)		//假设右移一位
						{
							forbid_flag = 1;
						}
					}
				}
				if(forbid_flag == 1)
				{
					forbid_flag = 0;
					pos_x += 0;
				}
				else 
					pos_x += 2;
				break;
	        case 'D':
				for(i = 0; i < 4; i++)
				{
					for(j = 0; j < 8; j++)
					{
						if(graph[block_ID].blocks[i][j]==1 && square[pos_y+i][pos_x+j-1].val==1)	//假设左移了一位
						{
							forbid_flag = 1;
						}
					}
				}
				if(forbid_flag == 1)
				{
					forbid_flag = 0;
					pos_x -= 0;	//left
				}
				else
					pos_x -= 2;
				break;
	    }
        fill_square();
		draw_block();
        check_full_row();
	}
}

