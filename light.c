/*
 * File:   light.c
 * Author: EVERQ
 *
 * Created on 2019?3?4?, ??5:32
 */


#include <xc.h>
#include <stdint.h>
#include <pic16f1786.h>

#define     LCD12864_E    RA2     //24C02???
#define     LCD12864_RW    RA1     //24C02???
#define     LCD12864_RS    RA0     //24C02???
#define     LCD12864_REST   RA3     //24C02???
#define     LCD12864_Data    PORTC     //24C02???
#define     Buzzer    PORTC     //24C02???

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = ECH    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
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

void delay_100us(int cnt)
{
//    TRISB = 0b00000000;
//    TRISB = 0b00000000;
//    TRISB = 0b00000000;
//    TRISB = 0b00000000;
//    TRISB = 0b00000000;
    int h,i,j;
    for(h=0;h<6*cnt;h++) {
//        for(i=0;i<5;i++) {
//            for(j=0;j<117;j++) {
//                for(k=0;k<215;k++) {
        i++;
//                }
//            }
//        }
    }
}

void delay_1s(void){
    int h,i,j,k;
    for(h=0;h<6;h++) {
        for(i=0;i<10;i++) {
            for(j=0;j<117;j++) {
//                for(k=0;k<215;k++) {
                    TRISA = 0b00000000;
//                }
            }
        }
    }
}

//?12864?????
void LCD12864_Write_Data(int dat)
{
    LCD12864_E = 0;
    LCD12864_RW = 0;//choose write mode
    LCD12864_RS = 1;//choose data mode

    // ready for data
    LCD12864_Data = dat;

    LCD12864_E = 1;//start to write data

    delay_100us(1);
}


void LCD12864_Write_Com(int com)
{
    
    //Chip select
    LCD12864_E = 0;
    LCD12864_RW = 0;//choose write mode
    LCD12864_RS = 0;//choose order mode

    // ready for data
    LCD12864_Data = com;

    LCD12864_E = 1;//start to write data

    delay_100us(1);
}


void LCD12864_Init()
{
//    //LCD reset
//    LCD12864_REST = 0;
//    delay_ms(30);
//    LCD12864_REST = 1;
//    delay_ms(20);
    TRISA = 0b00000000;
    LATA=0;
    ANSELA=0;
    TRISC = 0b00000000;
    OSCCON = 0b01111010;  
    LATC=0;
    
    //LCD reset
    LCD12864_REST = 0;
    delay_100us(2);
    LCD12864_REST = 1;
    delay_100us(2);
    /* start init step */
    LCD12864_Write_Com(0x30);
    delay_100us(50);
    LCD12864_Write_Com(0x08);
    delay_100us(50);
    LCD12864_Write_Com(0x01);
    delay_100us(50);
    LCD12864_Write_Com(0x06);
    delay_100us(50);
    LCD12864_Write_Com(0x0c);
    delay_100us(50);
}


void LCD12864_Clear_Screen(int value)
{
    int i,j;

    LCD12864_Write_Com(0x34);
    LCD12864_Write_Com(0x34);

    for(i=0;i<64;i++){
        if(i<32){
            LCD12864_Write_Com(0x80+i);
            LCD12864_Write_Com(0x80);
        } else {
            LCD12864_Write_Com(0x80+(i-32));
            LCD12864_Write_Com(0x88);
        }

        for(j=0;j<16;j++)
            LCD12864_Write_Data(value);
    }
    
    LCD12864_Write_Com(0x36);
    LCD12864_Write_Com(0x30);
    
    delay_100us(10000);
    
}

void LCD12864_Display_Char(){

    unsigned char addresses[] = {0x80,0x90,0x88,0x98};
    LCD12864_Write_Com(0x30);
    LCD12864_Write_Com(0x06);

    //write address
    LCD12864_Write_Com(0x82);

    //write data
    LCD12864_Write_Data(0x03);

}

void LCD12864_Display_Chars(unsigned char x,unsigned char y,unsigned char dat){

    unsigned char addresses[] = {0x80,0x90,0x88,0x98};
    int i = 0;
    LCD12864_Write_Com(0x30);
    LCD12864_Write_Com(0x06);

    //write address
    LCD12864_Write_Com(addresses[x-1]+(y-1));

    //write data
//    while(dat[i] != '\0'){
//        LCD12864_Write_Data(*dat);
//        dat++;
//    }
} 


void main(void)
{
    int i, j;
    
    unsigned char *datas1 ="XTEST";
    unsigned char *datas2 ="XHAHA";
    LCD12864_Init();
    LCD12864_Clear_Screen(0x00);
    while(1) {
//        LCD12864_Clear_Screen(0xFF);
//        LCD12864_Display_Char();
//        LCD12864_Display_Chars(2,3,datas1);
        LCD12864_Write_Com(0x30);
        LCD12864_Write_Com(0x06);
        LCD12864_Write_Com(0x82);
        LCD12864_Write_Com(0x88);
        LCD12864_Write_Data('S');
        LCD12864_Write_Data('E');
        LCD12864_Write_Data('E');
        LCD12864_Write_Data('D');
    }
    return;
}

