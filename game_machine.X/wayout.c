#include "common.h"

void draw_wayout_right(int level) {
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
        else if(i < 32 && i >= 16) {
            Write_Com(0x80+i);
            Write_Com(0x84);
            Write_Data(0b01111000);
        }
        else if(i < 48 && i >= 32) {
            Write_Com(0x80+(i-32));
            Write_Com(0x8C);
            Write_Data(0b01111000);
        }
        else {
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
    int2string(display_step,5,total_step);
    Display_Chars(2,display_step,5);
    if(level == 1)
        Display_Chars(3," Min:01",5);
    if(level == 2)
        Display_Chars(3," Min:02",5);
    if(level == 3)
        Display_Chars(3," Min:03",5);
    if(level == 4)
        Display_Chars(3," Min:05",5);
    if(level == 5)
        Display_Chars(3," Min:07",5);
    if(level == 6)
        Display_Chars(3," Min:04",5);
}

//judge if (i, j) is on table
unsigned char if_in_table(int i, int j){
    if(i >= 0 && i <= MAX_ROW)
        if(j >= 0 && j <= MAX_COL)
            return 1; //(i, j) not on the table
    return 0; //(i, j) is on the table
}

//when you choose (i, j), the change on point (k, l), (k, l) must be the closest block 
void change_near(int k, int l){
    if(if_in_table(k, l)){ //change (k, l)
        if(table[k][l] == 0) //(i, j) is empty.
            ;
        else if(table[k][l]== 1 || table[k][l] == 2){
            table[k][l] = 3 - table[k][l];
            set_point(k,l,screen[table[k][l]]);
        }
        else{
            ;
        }
    }
}

//choose block (i,j)
unsigned char choose(int i, int j){
    if(!if_in_table(i, j))
        return 0;
    else{
        if((table[i][j] & 0x03) == 1 || (table[i][j] & 0x03) == 2){
            //change (i, j)2 
            table[i][j] = (3 - (table[i][j] & 0x03)) | 0x08;
            set_point(i,j,screen[table[i][j]-5]);
            change_near(i-1, j);
            change_near(i+1, j);
            change_near(i, j-1);
            change_near(i, j+1);
            return 1;
        }
        else{
            return 0;
        }
    }
}

//check state
unsigned char check_if_complete(){
    for(unsigned char i=0; i<MAX_ROW; i++)
        for(unsigned char j=0;j<MAX_COL;j++)
        // change 2 into every possible number preventing state from win
            if(table[i][j] == 2)
                return 0;
    return 1;
}

unsigned char choose_level()
{
    Clear_Screen(0x00);
    temp_cursor.row=0;
    temp_cursor.col=0;
    set_point(temp_cursor.row,temp_cursor.col,screen[3]);
    while(1)
    {
        if(key==1)  //up
        {
            set_point(temp_cursor.row,temp_cursor.col,screen[0]);
            temp_cursor.row=(temp_cursor.row+MAX_LEVEL-1)%MAX_LEVEL;
            set_point(temp_cursor.row,temp_cursor.col,screen[3]);
        }
        else if(key==2)  //down
        {
            set_point(temp_cursor.row,temp_cursor.col,screen[0]);
            temp_cursor.row=(temp_cursor.row+1)%MAX_LEVEL;
            set_point(temp_cursor.row,temp_cursor.col,screen[3]);
        }
        else if(key==5)  //confirm
            return (temp_cursor.row+1);
        key=0;
    }
}

void wayout_display1()
{
   Display_Chars(1, "level 1",3);
   Display_Chars(2, "level 2",3);
   Display_Chars(3, "level 3",3);  
   Display_Chars(4, "...",7);  
}
void wayout_display2()
{
   Display_Chars(1, " ...",7);
   Display_Chars(2, "level 4",3);
   Display_Chars(3, "level 5",3);
   Display_Chars(4, "level 6",3);
}

unsigned char choose_level2()
{
    Clear_Screen(0x00);
    temp_cursor.row=0;
    temp_cursor.col=0;
    char max_level3=8;
    char step =2;
    int tag = 0;
    int offset = 0;
//    set_point(temp_cursor.row,temp_cursor.col,screen[3]);
    arrow = 1;
    Display_Char(arrow, 0x1A, 1);    
    wayout_display1();
    while(1)
    {
        if(key == 1)  //up
        {
            key=0;
            if(arrow != 1) {
                if(arrow == 4) {
                    Clear_Screen(0x00);
                    wayout_display1();
                    tag = 0;
                    arrow--;     
                    offset = arrow;
                    Display_Char(offset, 0x1A, 1);  
                } else {
                    arrow--;
                    offset = tag ? arrow-2 : arrow;
                    Display_Chars(offset+1, " ", 1);  
                    Display_Char(offset, 0x1A, 1);  
                }
            }
        }
        else if(key == 2)  //down
        {
            key=0;
            if(arrow != 6) {
                if(arrow == 3) {
                    Clear_Screen(0x00);
                    wayout_display2();
                    arrow++;
                    tag = 1;
                    offset = arrow -2;
                    Display_Char(offset, 0x1A, 1);  
                } else {
                    arrow++;
                    offset = tag ? arrow-2 : arrow;
                    Display_Chars(offset-1, " ", 1);  
                    Display_Char(offset, 0x1A, 1);  
                }
            }
        }
        else if(key == 5)  //confirm
        {
            key=0;
            return arrow;
        }
        else if (key == 6)   //back
        {
            key=0;
            arrow=0; // reset arrow
            init_environment();
            set_menu();
            return 0;
        }
    }
}

void init_wayout(unsigned char level_num)
{
    Clear_Screen(0x00);

    for(int i=0;i<MAX_ROW;i++)
        for(int j=0;j<MAX_COL;j++)
        {    
            table[i][j]=level[level_num-1][i][j];
            if(table[i][j]>0)
                set_point(i,j,screen[table[i][j]]);
        }
    temp_cursor.col=0;
    temp_cursor.row=0;
    table[0][0] = table[0][0] | 0x08;
    set_point(0,0,screen[table[0][0]-5]);
    total_step=0;
    draw_wayout_right(level_num);
}

void wayout()
{
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
            if(choose(temp_cursor.row,temp_cursor.col))
            {        
                total_step++;
                int2string(display_step,5,total_step);
                Display_Chars(2,display_step,5);
            }
            else
                break;
             
            if(check_if_complete())
            {
 
                
                Clear_Screen(0x00);
                Display_Chars(1, "Success",1);
                Display_Chars(2, "Press back to exit",1);
                while(key!=6)
                    ;
                key = 0;     //back to wayout menu
                start = 2;
                level_num = choose_level2();
                if (level_num !=0)
                {
                    init_wayout(level_num);
                }
                else
                {
                    start =0;
                }
            }
            else
                break;
        case 6:  //back to wayout menu
            key = 0;
            start = 2;
            level_num = choose_level2();
            if (level_num !=0)
            {
                init_wayout(level_num);
            }
            else
            {
                start =0;
            }
            break;
    }
}
