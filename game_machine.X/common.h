/* 
 * File:   common.h
 * Author: yc ACM dalao
 *
 * Created on 2019?3?17?, ??3:30
 */
#ifndef COMMON_H
#define	COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pic16f1786.h>
#include "heibaiqi.h"
#include "wayout.h"
#include <xc.h>
#include <stdint.h>
#include <string.h>


// -- pic&lcd config
#define     E    RB1     //24C02???
#define     RW    RB2     //24C02???
#define     RS    RB3     //24C02???
#define     REST   RB0     //24C02???
#define     Data    PORTA     //24C02???

//low present on
#define     Up    RC3     //24C02???
#define     Down    RC0     //24C02???
#define     Left    RC2     //24C02???
#define     Right   RC1     //24C02???
#define     Confirm RB5
#define     Back    RC5
#define     Buzzer    RC4     //24C02???

// -- game config
#define     BWMapOffset 0 // offset between 0-3

// -- other config
#define     nop()   asm("nop")
    
//music
#define    Do    58
#define    Ri    51
#define    Mi    45
#define    Fa    43
#define    So    37
#define    La    32
#define    Si    30
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC // ECH    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
//#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit cannot be cleared once it is set by software)
//#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR)
#pragma config PLLEN = ON       // Phase Lock Loop enable (4x PLL is always enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)
    
#define GAME_NUM 2
    
//unsigned char display_flag;
unsigned char start;   //sign whether the game starts.(0-none?1-heibaiqi)
unsigned char key;  //record the button value?0-none?1-up?2-down?3-left?4-right?5-confirm?6-back?
unsigned char arrow;

struct cursor   //the position of current cursor
{
    unsigned char row;
    unsigned char col;
    unsigned char value;
}temp_cursor;  

//struct change
//{
//    unsigned char row;
//    unsigned char col;
//    unsigned char value;
//}change_value[64];
//unsigned char change_num;

unsigned char table[8][8] = {};   //0->empty,1->black,2->white.8->kuang,9->black+kuang,10->white->kuang
                                  //use the third bit to sign the kuang

unsigned char screen[6][8]=
                    { {},                                                 //empty
                    {0x00, 0x3c, 0x7e, 0x7e, 0x7e, 0x7e, 0x3c, 0x00},    //black
                    {0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00},    //white
                    {0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff},    //choose_empty
                    {0xff, 0x81, 0x99, 0xbd, 0xbd, 0x99, 0x81, 0xff},    //choose_black
                    {0xff, 0x81, 0x99, 0xa5, 0xa5, 0x99, 0x81, 0xff}     //choose_white
                    };
void int2string(char *a, unsigned char begin, unsigned char num);
void init_cfg();
void init_environment();
void start_video(); 
void run_game();   
void set_menu();   //set the menu
unsigned char read_key();
unsigned char get_key();  //get the button's value
void display();   //show on screen
void delay_100us(int cnt);
void delay_1s(void);
static void check_busy();
void Write_Data(int dat);
void Write_Com(int com);
static uchar readData();
void Display_Chars(uchar x,uchar *string, uchar y);
void Display_Char(uchar x, uchar data, uchar y);
void set_point(uchar point_y, uchar point_x, uchar *value);
uchar readFromTable(uchar x, uchar y, int offset);
int check_Pos(uchar x, uchar y);
uchar get_Y_Addr(uchar x, uchar y);
uchar get_X_Addr(uchar x, uchar y);
void Clear_Screen(int value);
static uchar readData();
void music(int mclass);
void delay_music(int mclass);
void music_delay(int mclass);
void little_star();
#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */

