
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "include.h"
#include <sys/time.h>
int main(int argc, char *argv[])
{
    square_init();
	signal(SIGALRM, sig_func);
    set_timer(500);     //500ms    

    srand(time(NULL));
		
    system("stty -echo -icanon");
    printf("\033[?25l");     //隐藏光标   
	//显示下一个图形
	get_block_ID();	
	//------------主循环-------
	get_dir();
	//--------------------------
	printf("\033[0m");
    system("stty echo icanon");
    printf("\033[?25h");     //显示光标
    printf("\033[%d;%dH", ROW+1, 1); 
    return 0;
}
