#include "common.h"

void int2string(char *a, unsigned char begin, unsigned char num)
{
    int i=begin;
    a[i]=num/10+'0';
    a[i+1]=num%10+'0';
    a[i+2]='\0';      
}

void delay_100us(int cnt){
    int i,h;
    for(h=0;h<6*cnt;h++) {
        i++;
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

static void check_busy() {
    while(1) {
        RS=0;
        RW=1;
        E = 0;
        PORTC = 0xFF;
        E=1;
        
        TRISC = 0xff;
        if((PORTC&0x80)==0) {
            break;
        }
    }
    TRISC = 0;
}

void Write_Data(int dat){
//    wait_busy();
    E = 0;
    RW = 0;//choose write mode
    RS = 1;//choose data mode
    // ready for data
    E = 1;//start to write data
    Data = dat;
    E = 0;
    delay_100us(1);
}

void Write_Com(int com){
//    wait_busy();
    E = 0;
    RW = 0;//choose write mode
    RS = 0;//choose data mode
    // ready for data
    E = 1;//start to write data
    Data = com;
    E = 0;
    delay_100us(1);
}

static uchar readData(){
	uchar data;
	check_busy();
    
    TRISC = 0xff; // set PORTC to input mode before read its state
    
	RS=1;
	RW=1;
    E=1;
    data=PORTC;
    E=0;
    
    TRISC = 0; // set PORTC back to output mode.
	return data;
}

void init_cfg()    //init pic setting
{
    // port reset
    TRISA = 0b00000000;
    LATA=0;
    ANSELA=0;
    TRISB = 0b00000000;
    ANSELB=0;
    TRISC = 0b00000000;
    LATC=0;
    OSCCON = 0b01111010;  
    
    // interuption init
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
//    INTCONbits.TMR0IE = 1;
//    // Timer0
//    OPTION_REGbits.TMR0CS = 0; // clk = Fosc/4
//    OPTION_REGbits.PSA = 0; 
//    OPTION_REGbits.PS = 0b111; // fenpingbi 1:16 -> 011
    // Timer2
    PIE1 = 2; // ?????????()
    PIR1 = 0; // ?????????
    PR2 = 0b01111111;
    T2CONbits.T2OUTPS = 0b1111; // after output fenpingbi 1:16
    T2CONbits.TMR2ON = 1;
    T2CONbits.T2CKPS = 0b11; // pre fenpingbi 64
    
    // LCD reset
    REST = 0;
    delay_100us(20);
    REST = 1;
    delay_100us(20);
    /* start init step */
    Write_Com(0x30);
    delay_100us(100);
    Write_Com(0x30);
    delay_100us(100);
    Write_Com(0x0c);
    delay_100us(100);
    Write_Com(0x01);
    delay_100us(100);
}

void Clear_Screen(int value){ 
    uchar i,j;
    Write_Com(0x01); // clear DDRAM display
    delay_100us(100);
	Write_Com(0x34); // extend instruction set: close drawing display
	for(i=0;i<32;i++)			  
	{
		Write_Com(0x80+i);
		Write_Com(0x80);	
		for(j=0;j<32;j++)
		{
			Write_Data(value);	  
		}
	}
	Write_Com(0x36); // extend function set: open drawing display
	Write_Com(0x30); // change to basic instruction set and close drawing display
}

void Display_Chars(uchar x,uchar *string, uchar y){
    uchar addresses[] = {0x80,0x90,0x88,0x98};
    int i = 0;
    Write_Com(0x30);
    Write_Com(0x06);

    //write address
    Write_Com(addresses[x-1]+(y-1));

    //write data
    while(string[i] != '\0'){
        Write_Data(*string);
        string++;
    }
} 

// display one char by using 0x02-0x7f]
void Display_Char(uchar x, uchar data, uchar y) {
    uchar addresses[] = {0x80,0x90,0x88,0x98};
    int i = 0;
    Write_Com(0x30);
    Write_Com(0x06);

    //write address
    Write_Com(addresses[x-1]+(y-1));
    
    Write_Data(data);
}


// -- game logic -- //
// == Game One : black and white chess == //

// 0 <= point_x & point_y <= 7 
// value -> the pictrue of one point (8*8)
uchar get_Y_Addr(uchar x, uchar y){
    uchar addresses[] = {0x80,0x88};
    uchar init_Addr, addr, addr_Offset;
    init_Addr = (x<4) ? addresses[0] : addresses[1];
    addr_Offset = (y%2 == 0) ? y/2 : (y-1)/2;
    addr = init_Addr + addr_Offset;

    return addr;
}

uchar get_X_Addr(uchar x, uchar y){
    uchar addr;
    if(x == 0 || x == 4) addr = 0x80;
    else if(x == 1 || x == 5) addr = 0x88;
    else if(x == 2 || x == 6) addr = 0x90;
    else if(x == 3 || x == 7) addr = 0x98;
    return addr;
}

int check_Pos(uchar x, uchar y){
    int result = 0;
    if(y%2 != 0) result = 1;
    return result;
}

uchar readFromTable(uchar x, uchar y, int offset){
//    if(table[x][y] == 0) return empty[offset];
//    else if(table[x][y] == 1) return first_chess[offset];
//    else return second_chess[offset];
    if(table[x][y] & 0x08)
        return screen[table[x][y]-5][offset];
    else
        return screen[table[x][y]][offset];
}

void set_point(uchar point_x, uchar point_y, uchar *value){
    int i,j;
    int is_Offset_Pos;
    uchar init_X_Addr, init_Y_Addr;
    init_X_Addr = get_X_Addr(point_x, point_y);
    init_Y_Addr = get_Y_Addr(point_x, point_y);
    is_Offset_Pos = check_Pos(point_x, point_y);
    
	Write_Com(0x34); // extend instruction set: close drawing display
    
    for(i=0;i<8;i++){
        Write_Com(init_X_Addr+i); // hang addr(Y zhou) 0x80-0x98 32
        Write_Com(init_Y_Addr); // lie addr(X zhou) 0x80-0x8F 16
        
        if(is_Offset_Pos == 0) Write_Data(*value);
        else {
            Write_Data(readFromTable(point_x,point_y-1,i));
            Write_Data(*value);
        }
        value++;
    }
	Write_Com(0x36); // extend function set: open drawing display
	Write_Com(0x30); // change to basic instruction set and close drawing display
    
//    delay_100us(1000);
    
//    Write_Com(0x82); // hang addr(Y zhou) 0x80-0x98 32
//    Write_Com(0x8F); // lie addr(X zhou) 0x80-0x8F 16
//    for(j=0;j<2;j++)
//    {
//        Write_Data(value);	  
//    }
}

void write_table_from_setPoint(uchar *table){ //init table
    for(uchar i=0; i<8; i++){
        for(uchar j=0; j<8; j++){
            if(*(table + i*8 + j) == 0){
                if(1)
                    set_point(j, i, empty);
                else
                    set_point(j, i, choose_empty);
            }
            else if(*(table + i*8 + j) == 1){
                if(1)
                    set_point(j, i, first_chess);
                else
                    set_point(j, i, choose_first);
            }
            else{
                if(1)
                    set_point(j, i, second_chess);
                else
                    set_point(j, i, choose_second);
            }
        }
    }
}

void init_environment()   //init chushi menu
{
    start=0;
    key=0;
    arrow=1; // indicate the pos of the arrow
//    display_flag = 0;
    temp_cursor.row=0;
    temp_cursor.col=0;
//    change_num=0;
    memset(table,0,sizeof(table));
    Clear_Screen(0x00);
}

void delay_music(int mclass)
{
   int a=1;
   for (int i=0;i<mclass;i++)
   {
    a++;
   }
}

void start_video() 
{
    Display_Chars(1, "Welcome to",1);
    Display_Chars(2, "Xiaobawang",1);
    Display_Chars(3, "Game Machine",1);
//    little_star();
    //need clear screen here
    Clear_Screen(0x00);
}
void music_delay(int mclass)
{
    for (int i=0;i<5000*6/mclass;i++)
    {
        delay_music(mclass);
        RC4 = 0;
        delay_music(mclass);
        RC4 = 1;
        // Display_Chars(1,1,datas1);
    }
    for (int i=0;i<5000*3/mclass;i++)
    {
        delay_music(mclass);
        RC4 = 0;
        delay_music(mclass);
        RC4 = 1;
        // Display_Chars(1,1,datas1);
    }
    for (int i=0;i<500;i++)
    {
        delay_100us(1);
    }
}
void little_star()
{
    music(Do); music(Do);music(So);music(So);music(La);music(La);music_delay(So); delay_100us(750);
    music(Fa);music(Fa);music(Mi);music(Mi);music(Ri);music(Ri);music_delay(Do); delay_100us(750);
    music(So);music(So);music(Fa);music(Fa);music(Mi);music(Mi);music_delay(Ri); delay_100us(750);
    music(So);music(So);music(Fa);music(Fa);music(Mi);music(Mi);music_delay(Ri); delay_100us(750);
}
void music(int mclass)
{
    for (int i=0;i<5000*6/mclass;i++)
    {
        delay_music(mclass);
        RC4 = 0;
        delay_music(mclass);
        RC4 = 1;
        // Display_Chars(1,1,datas1);
    }
    for (int i=0;i<500;i++)
    {
        delay_100us(1);
    }
}

void set_menu()    //set menu after change the position of cursor 
{
    Display_Chars(1, "heibaiqi",2);
    Display_Chars(2,"wayout",2);
    // init arrow
    Display_Char(1, 0x1A, 1);
    // set_point(0,0,screen[3]);
}

unsigned char read_key()
{
//    TRISB = 0b00111111;
//    WPUB = 0b00111111;
    // ??????????????????????
    TRISBbits.TRISB5 = 1;
    WPUBbits.WPUB5 = 1;
    TRISC = 0b00101111;
    WPUC = 0b00101111;
    OPTION_REGbits.nWPUEN = 0;
    if(Up==0)
        return 1;
    else if(Down==0)
        return 2;
    else if(Left==0)
        return 3;
    else if(Right==0)
        return 4;
    else if(Confirm==0)
        return 5;
    else if(Back==0)
        return 6;
    else 
        return 0;
}

unsigned char temp_key = 0;
unsigned char get_key() //get the button value
{
    unsigned char last_key = temp_key;
    temp_key = read_key();
    if(temp_key == 0)
        last_key = 0;
    if(last_key == temp_key && temp_key != 0) {
        delay_100us(200);
        if(temp_key != 0) return temp_key;
    }
    return 0;
//    if(last_key ==0 && temp_key > 0)
//    //if(temp_key > 0)
//    return temp_key;
////    return 0;
}

//void display()
//{
//    for(int i=0;i<change_num;i++)
//    {
//        if(change_value[i].value==0)
//            set_point(change_value[i].row,change_value[i].col,empty);
//        else if(change_value[i].value==1)
//            set_point(change_value[i].row,change_value[i].col,first_chess);
//        else if(change_value[i].value==2)
//            set_point(change_value[i].row,change_value[i].col,second_chess);
//        else if(change_value[i].value==3)
//            set_point(change_value[i].row,change_value[i].col,choose_empty);
//        else if(change_value[i].value==4)
//            set_point(change_value[i].row,change_value[i].col,choose_first);
//        else if(change_value[i].value==5)
//            set_point(change_value[i].row,change_value[i].col,choose_second);        
//    }
//    memset(change_value,0,change_num*sizeof(struct change_value));
//    change_num=0;
//}

void run_game()   
{
    
    if(!start)   
    {
        // if(Up == 0)
        if(key==1)  //up
        {
            key=0;
            if(arrow != 1) {
                Display_Chars(arrow, " ", 1);
                Display_Char(--arrow, 0x1A, 1);                
            }
//            set_point(temp_cursor.row*2,temp_cursor.col,screen[0]);
//            temp_cursor.row=(temp_cursor.row+GAME_NUM-1)%GAME_NUM;
//            set_point(temp_cursor.row*2,temp_cursor.col,screen[3]);
        }
        // else if(Down == 0)
        else if(key==2)  //down
        {
            key=0;
            if(arrow != 2) {
                Display_Chars(arrow, " ", 1);
                Display_Char(++arrow, 0x1A, 1);
            }
//            set_point(temp_cursor.row*2,temp_cursor.col,screen[0]);
//            temp_cursor.row=(temp_cursor.row+1)%GAME_NUM;
//            set_point(temp_cursor.row*2,temp_cursor.col,screen[3]);
        }
        // else if(Confirm == 0)
        else if(key==5)  //confirm
        {    
            key=0;
//            start=temp_cursor.row+1;   //the game starts now
            start=arrow;
            temp_cursor.row=0;
            temp_cursor.col=0;
            if(start==1)
                init_heibaiqi();
            if(start==2)
            {
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
        }
//        set_point(2,2,choose_first);
    }
    else if(start==1)   //heibaiqi
    {
        arrow = 1; // reset arrow
        heibaiqi();
    }
    else if(start==2)
    {
        arrow = 1; // reset arrow
        wayout();
    }
}

