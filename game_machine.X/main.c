/* 
 * File:   main.c
 * Author: yc
 *
 * Created on 2019?3?17?, ??3:26
 */
#include "common.h"
//int cnt = 0;
void interrupt ISR(void)
{
    if(1==TMR2IF)
    {
//        if(cnt == 0) {
//        Display_Chars(4, "go to interrupt",1); // add test code(?????????????))
        key=get_key(); 
        if(key != 0) {
            delay_100us(1000);
        }
//        }
        TMR2=0;
        TMR2IF=0;
//        cnt--;
    }
//    else if
//    {
//        if(display_flag)
//            display();
        //TMR0=0;
        //TMR0IF=0;
//    }
}

int main(int argc, char** argv) {
    init_cfg();
    
    //level_num=5;
    //init_wayout();
    //while(1);
    init_environment();
    start_video();  
    set_menu();
    while(1)
    {
        run_game();
//        Buzzer = 1;
//        delay_100us(50);
//        Buzzer = 0;
//        delay_100us(50);
    }
    return (EXIT_SUCCESS);
}

