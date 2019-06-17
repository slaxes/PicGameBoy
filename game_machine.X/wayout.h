/* 
 * File:   wayout.h
 * Author: ASUS
 *
 * Created on 2019?4?1?, ??2:22
 */

#ifndef WAYOUT_H
#define	WAYOUT_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MAX_LEVEL 6   
#define MAX_ROW 8
#define MAX_COL 8
unsigned char level_num;
unsigned char total_step;
char display_step[10]=" Way:";

unsigned char level[MAX_LEVEL][MAX_ROW][MAX_COL] = {
    {
    0, 0, 0, 0, 0, 0, 0, 0, //level 1
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 1, 0, 0, 0,
    0, 0, 2, 2, 2, 0, 0, 0,
    0, 0, 1, 2, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
    },
    
    {
    0, 0, 0, 0, 0, 0, 0, 0, //level 2
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 2, 0, 0, 0, 0,
    0, 0, 2, 1, 2, 0, 0, 0,
    0, 0, 0, 2, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
    },

    {
    0, 0, 0, 0, 0, 0, 0, 0, //level 3
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 0, 0, 0, 0,
    0, 0, 1, 1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
    },
    
    {
    0, 0, 0, 0, 0, 0, 0, 0, //level 4
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 1, 2, 0, 0, 0,
    0, 0, 1, 1, 0, 2, 0, 0,
    0, 0, 2, 0, 2, 2, 0, 0,
    0, 0, 0, 2, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
    }, 
     
    {
    0, 0, 0, 0, 0, 0, 0, 0, //level 5
    0, 0, 0, 0, 0, 2, 2, 0,
    0, 0, 0, 0, 2, 0, 1, 0,
    0, 2, 2, 0, 2, 1, 2, 0,
    0, 1, 1, 0, 0, 0, 0, 0,
    0, 2, 1, 1, 2, 0, 0, 0,
    0, 1, 2, 1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
    },
    
    {
    0, 0, 0, 0, 0, 0, 0, 0, //level 6
    0, 0, 0, 1, 2, 0, 0, 0,
    0, 0, 0, 1, 2, 1, 2, 0,
    0, 0, 2, 2, 0, 1, 2, 0,
    0, 0, 1, 1, 2, 2, 0, 0,
    0, 0, 0, 0, 2, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
    }
};

unsigned char choose_level();
unsigned char choose_level2();
void init_wayout(unsigned char level_num);
unsigned char if_in_table(int i, int j);
void change_near(int k, int l);
unsigned char choose(int i,int j);
void draw_wayout_right(int level);
void wayout();


#ifdef	__cplusplus
}
#endif

#endif	/* WAYOUT_H */

