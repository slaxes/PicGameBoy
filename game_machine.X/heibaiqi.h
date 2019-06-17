/* 
 * File:   heibaiqi.h
 * Author: yc
 *
 * Created on 2019?3?17?, ??5:11
 */

#ifndef HEIBAIQI_H
#define	HEIBAIQI_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define uchar unsigned char
    
uchar last_table[8][8] = {};   //0->empty,1->black,2->white,3->kuang,4->black+kuang,5->white->kuang
uchar empty[8] = {};
uchar choose_empty[] = {0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff};
uchar first_chess[] = {0x00, 0x3c, 0x7e, 0x7e, 0x7e, 0x7e, 0x3c, 0x00};
uchar second_chess[] = {0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00};
uchar choose_first[] = {0xff, 0x81, 0x99, 0xbd, 0xbd, 0x99, 0x81, 0xff};
uchar choose_second[] = {0xff, 0x81, 0x99, 0xa5, 0xa5, 0x99, 0x81, 0xff};
char display_black[10]=" [B]:";
char display_white[10]=" [W]:";
char towards[8][2] = {{0,1},{0,-1},{1,0},{1,1},{1,-1},{-1,0},{-1,1},{-1,-1}};

unsigned char player;  //1->black,2->white
unsigned char black_num;
unsigned char white_num;
unsigned char empty_num;
unsigned char result;  //0->continue,1->black win,2->white win

void draw_heibaiqi_right(); 
void heibaiqi();  //main function for heibaiqi
void init_heibaiqi();
unsigned char judge_result(unsigned char player);    //(0->continue,1->black win,2->white win)
unsigned char choose_point(char row,char col, unsigned char ply);
unsigned char check_point(char row, char col, unsigned char ply);

#ifdef	__cplusplus
}
#endif

#endif	/* HEIBAIQI_H */

