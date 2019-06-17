#include "common.h"

void draw_heibaiqi_right() {
    uchar i,j;
    Write_Com(0x01); // clear DDRAM display
    delay_100us(100);
	Write_Com(0x34); // extend instruction set: close drawing display
    for(i=0; i<64; i++) {
        if(i < 16) {
            Write_Com(0x80+i);
            Write_Com(0x84);
            Write_Data(0x7f);
            for(j=1; j<8; j++) {
                Write_Data(0xff);   
            }
        }
//        else if(i < 32 && i >= 16) {
//            Write_Com(0x80+i);
//            Write_Com(0x84);
//            Write_Data(0b01111000);
//        }
//        else if(i < 48 && i >= 32) {
//            Write_Com(0x80+(i-32));
//            Write_Com(0x8C);
//            Write_Data(0b01111000);
//        }
        else if(i >= 48) {
            Write_Com(0x80+(i-32));
            Write_Com(0x8C);
            Write_Data(0x7f);
            for(j=1; j<8; j++) {
                Write_Data(0xff);   
            }
        }
    }
	Write_Com(0x36); // extend function set: open drawing display
	Write_Com(0x30); // change to basic instruction set and close drawing display
    
    int2string(display_black,5,black_num);
    Display_Chars(2,display_black,5);
    int2string(display_white,5,white_num);
    Display_Chars(3,display_white,5);
}

void init_heibaiqi()
{
    Clear_Screen(0x00);
    player=1;
    result=0;
    black_num=2;
    white_num=2;
    empty_num=60;
    temp_cursor.row=3;
    temp_cursor.col=3;
    //set initial table
    memset(table,0,sizeof(table));
    table[3][3] = 1 | 0x08;
    table[4][4] = 1;
    table[3][4] = 2;
    table[4][3] = 2;
//    change_value[change_num].row=3;
//    change_value[change_num].col=3;
//    change_value[change_num].value=4;
//    change_num++;
//    change_value[change_num].row=4;
//    change_value[change_num].col=4;
//    change_value[change_num].value=1;
//    change_num++;
//    change_value[change_num].row=3;
//    change_value[change_num].col=4;
//    change_value[change_num].value=2;
//    change_num++;
//    change_value[change_num].row=4;
//    change_value[change_num].col=3;
//    change_value[change_num].value=2;
//    change_num++;
    set_point(3, 3, choose_first);
    set_point(4, 4, first_chess);
    set_point(3, 4, second_chess);
    set_point(4, 3, second_chess);
    
    draw_heibaiqi_right();
}

unsigned char check_point(char row, char col, unsigned char ply)
{
    unsigned char result = 0;
    char add_row, add_col;
    unsigned char oppo = 3- ply;
    unsigned char temp = 0b10000000;
    //if(table[row][col] & 0x03)
    //    return 0;
    for(unsigned char j=0; j<8; j++){
        add_row = towards[j][0];
        add_col = towards[j][1];
        unsigned char flag=0;
        char i=1;
//        char next_col = col+i*add_col;
//        if(next_col < 0)
//            break;
//        else if(next_col >= 8)
//            break;
//        char next_row = row+i*add_row;
//        if(next_row< 0)
//            break;
//        else if(next_row >= 8)
//            break;
        // flag=0:not judge
        // flag=1:the block beside is oppo's
        // flag=2:get the final block which is ply's
        while(0 <= (char)(row+i*add_row) && (char)(row+i*add_row) < 8 && 0 <= (char)(col+i*add_col) && (char)(col+i*add_col) < 8){ //if block is in table
            if(flag == 0){
                if(table[row+i*add_row][col+i*add_col] == oppo)
                    flag = 1;
                else{
                    flag = 0;
                    break;
                }
            }
            else if(flag == 1){
               if(table[row+i*add_row][col+i*add_col] == oppo)
                    flag = 1;
                else if(table[row+i*add_row][col+i*add_col] == 0){
                    flag = 0;
                    break;
                }
                else{
                    flag = 2;
                    break;
                }
            }
            i += 1;
        }
        if(flag == 2){
            result |= temp; 
        }
        temp = temp >> 1;
    }
    return result;
}

unsigned char choose_point(char row, char col, unsigned char ply)
{
    if(table[row][col] & 0x03)
        return 0;
    unsigned char result = check_point(row, col, ply);
    if(!result)
        return 0;
    char right, up;
    unsigned char temp = 0b10000000;
    table[row][col] = ply | 0x08;
//    display_flag = 0;
//    change_value[change_num].row=row;
//    change_value[change_num].col=col;
//    change_value[change_num].value=ply+3;
//    change_num++;
//    display_flag = 1;
    if(ply==1)
        black_num++;
    else if(ply==2)
        white_num++;
    empty_num--;
    for(unsigned char i=0; i<8; i++){
        char j = 1;
        if(result & temp){
            right = towards[i][0];
            up = towards[i][1];
            while(0 <= (char)(col+j*up) && (char)(col+j*up) < 8 && 0 <= (char)(row+j*right) && (char)(row+j*right) < 8){
                if(table[row+j*right][col+j*up] != ply){
                    table[row+j*right][col+j*up] = ply;
//                    display_flag = 0;
//                    change_value[change_num].row=row+j*right;
//                    change_value[change_num].col=col+j*up;
//                    change_value[change_num].value=ply;
//                    change_num++;
//                    display_flag = 1;
                    set_point(row+j*right,col+j*up,screen[ply]);
                    if(ply==1)
                    {
                        black_num++;
                        white_num--;
                    }
                    else if(ply==2)
                    {
                        black_num--;
                        white_num++;
                    }
                    j++;
                }
                else 
                    break;
            }
        }
        temp = temp >> 1;
    }
    set_point(row,col,screen[ply+3]);
    int2string(display_black,5,black_num);
    Display_Chars(2,display_black,5);
    int2string(display_white,5,white_num);
    Display_Chars(3,display_white,5);
    return 1;
}

unsigned char judge_result(unsigned char player)
{
    if(black_num==0)
        return 2;
    else if(white_num==0)
        return 1;
    else if(empty_num==0)
    {
        if(black_num>white_num)
            return 1;
        else if(black_num<white_num)
            return 2;
        else if(black_num==white_num)
            return 3;
    }
    else
    {
        for(char row=0;row<8;row++)
            for(char col=0;col<8;col++)
            {
                unsigned char result = check_point(row, col, 3-player);
                if(result)
                    return 0;
                else
                    continue;
            }
        if(black_num>white_num)
            return 1;
        else if(black_num<white_num)
            return 2;
        else if(black_num==white_num)
            return 3;
    }
        
}
//
//unsigned char score(unsigned char ply)
//{
//    unsigned char num = 0;
//    for(unsigned char i=0; i<8; i++){
//        for(unsigned char j=0; j<8; j++){
//            if(table[i][j] == ply)
//                num++;
//        }
//    }
//    return num;
//}

void heibaiqi()
{
//    unsigned char last_row=temp_cursor.row;
//    unsigned char last_col=temp_cursor.col;
    if(player == 1) {
        Display_Char(2, 0x2A, 5);
        Display_Chars(3, " ", 5);
    }
    else if(player == 2) {
        Display_Char(3, 0x2A, 5);
        Display_Chars(2, " ", 5);
    }
    switch(key)
    {
        case 1:   //up
            key=0;
            table[temp_cursor.row][temp_cursor.col] = table[temp_cursor.row][temp_cursor.col] & 0x03;
            set_point(temp_cursor.row,temp_cursor.col,screen[table[temp_cursor.row][temp_cursor.col]]);
            temp_cursor.row=(temp_cursor.row+MAX_ROW-1)%MAX_ROW;
            table[temp_cursor.row][temp_cursor.col] = table[temp_cursor.row][temp_cursor.col] | 0x08;
            set_point(temp_cursor.row,temp_cursor.col,screen[table[temp_cursor.row][temp_cursor.col]-5]);
            break;
        case 2:  //down
            key=0;
            table[temp_cursor.row][temp_cursor.col] = table[temp_cursor.row][temp_cursor.col] & 0x03;
            set_point(temp_cursor.row,temp_cursor.col,screen[table[temp_cursor.row][temp_cursor.col]]);
            temp_cursor.row=(temp_cursor.row+1)%MAX_ROW;
            table[temp_cursor.row][temp_cursor.col] = table[temp_cursor.row][temp_cursor.col] | 0x08;
            set_point(temp_cursor.row,temp_cursor.col,screen[table[temp_cursor.row][temp_cursor.col]-5]);
            break;
        case 3:  //left
            key=0;
            table[temp_cursor.row][temp_cursor.col] = table[temp_cursor.row][temp_cursor.col] & 0x03;
            set_point(temp_cursor.row,temp_cursor.col,screen[table[temp_cursor.row][temp_cursor.col]]);
            temp_cursor.col=(temp_cursor.col+MAX_COL-1)%MAX_COL;
            table[temp_cursor.row][temp_cursor.col] = table[temp_cursor.row][temp_cursor.col] | 0x08;
            set_point(temp_cursor.row,temp_cursor.col,screen[table[temp_cursor.row][temp_cursor.col]-5]);
            break;
        case 4:  //right
            key=0;
            table[temp_cursor.row][temp_cursor.col] = table[temp_cursor.row][temp_cursor.col] & 0x03;
            set_point(temp_cursor.row,temp_cursor.col,screen[table[temp_cursor.row][temp_cursor.col]]);
            temp_cursor.col=(temp_cursor.col+1)%MAX_COL;
            table[temp_cursor.row][temp_cursor.col] = table[temp_cursor.row][temp_cursor.col] | 0x08;
            set_point(temp_cursor.row,temp_cursor.col,screen[table[temp_cursor.row][temp_cursor.col]-5]);
            break;
        case 5:  //confirm
            key=0;
            if(!choose_point(temp_cursor.row,temp_cursor.col,player))
                break;
            //set the table,and calculate the result
            result = judge_result(player);
            if(result==0)
            {
                player=3-player;
                break; 
            }
            else if(result==1)   //black win
            {
                Display_Chars(1, "Black Win",1);
                Display_Chars(2, "Press back to exit",1);
            }
            else if(result==2)   //white win
            {
                Display_Chars(1, "White Win",1);
                Display_Chars(2, "Press back to exit",1);                
            }
            else if(result==3)   //ping ju
            {
                Display_Chars(1, "ping ju",1);
                Display_Chars(2, "Press back to exit",1);    
            }  
            while(key!=6)
                ;
        case 6:  //back
            init_environment();
            set_menu();
            break;
    }
//    if(key>=1&&key<=4)
//    {
//        display_flag = 0;
//        change_value[change_num].row=last_row;
//        change_value[change_num].col=last_col;
//        change_value[change_num].value=table[last_row][last_col];
//        change_num++;
//        change_value[change_num].row=temp_cursor.row;
//        change_value[change_num].col=temp_cursor.col;
//        change_value[change_num].value=table[temp_cursor.row][temp_cursor.col]+3;
//        change_num++;
//        display_flag = 1;
//        set_point(last_row,last_col,screen[table[last_row][last_col]]);
//        set_point(temp_cursor.row,temp_cursor.col,screen[table[temp_cursor.row][temp_cursor.col]+3]);
//    }
}


