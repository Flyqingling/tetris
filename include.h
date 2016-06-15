
#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#define ROW 24		//只能是偶数
#define COLUMN 26	//只能是偶数

#define close 0 
#define black 40
#define red		41
#define green	42
#define yellow	43
#define blue	44
#define purple	45
#define dark_green	46
#define white	47
//定时器信号处理函数
void sig_func();
void fill_square();
void frame();
//删除慢行
void eraser_full_row(int row);
//设定定时器的时间
void set_timer(int);
//禁止方块移动，例如到边框，或者边上有东西档住
void forbid_move();
//旋转的时候，改变方块的编号
void get_block_ID();

struct background{
	char val;
	char color;
};
//方块结构体
struct brick{
        char blocks[4][8]; //方块
        char color;        //颜色
		char num;		   //编号
};

#endif
