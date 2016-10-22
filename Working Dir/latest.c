#include<LPC17XX.h>
unsigned int i,q,num,j,n=4,digit[4]={0,0,0,0},col,row,flag,dig;
unsigned long a[13]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x01,0x02};
unsigned long b[4][4]={{0,1,2,3},{4,5,6,7},{8,9,0xA,0xA},{0xB,0xB,0xB,0xB}};
unsigned long temp,pos[4]={0xF87FFFFF,0xF8FFFFFF,0xF97FFFFF,0xF9FFFFFF},X,inter;

void timer(long);
void EINT3_IRQHandler(void);
void scan(void);
void display(void);

int main(void){
	SystemCoreClockUpdate();
	SystemInit();	
	LPC_GPIO1->FIODIR =0x07800000;  //SEVEN SEG   //Data lines p0.4-p0.11   //Select lines p1.23-p1.26
	LPC_GPIO0->FIODIR = 0x00780FF0; //made output P0.19 to P0.22(Rows)
	LPC_GPIO0->FIOMASK=0x000000FF0;
	LPC_GPIO0->FIOPIN |= 0x00780000; 
	LPC_GPIOINT->IO0IntEnR=0x00078000;
	NVIC_EnableIRQ(EINT3_IRQn);
	while(1){
		//display();
	}
}	
	
	void timer(long l)
	{
	LPC_TIM0->CTCR=0X00000000;
	LPC_TIM0->TC=0x00;
	LPC_TIM0->MCR=0x02;
	LPC_TIM0->PR=0x00;
	LPC_TIM0->MR0=l;
	LPC_TIM0->EMR=0x00000020;
	LPC_TIM0->TCR=0x01;
}

void display(void){
	timer(2999);
	LPC_GPIO0->FIOMASK=0xFFFFF00F;
	while(LPC_TIM0->EMR!=0x00000021){
					num=b[row][col];
					digit[0]=num;
					dig=a[num];
					LPC_GPIO0->FIOPIN = dig<<4;
					LPC_GPIO1->FIOPIN = pos[0];
					for(q=0;q<50;q++);
					LPC_GPIO1->FIOPINH = 0xFFFF;
					for(q=0;q<50;q++);
		}
}

void EINT3_IRQHandler(void){
	inter=LPC_GPIOINT->IO0IntStatR;
	if(inter==1<<15){col=0;LPC_GPIOINT->IO0IntClr|=1<<15;}
	else if(inter==1<<16){col=1;LPC_GPIOINT->IO0IntClr|=1<<16;}
	else if(inter==1<<17){col=2;LPC_GPIOINT->IO0IntClr|=1<<17;}
	else if(inter==1<<18){col=3;LPC_GPIOINT->IO0IntClr|=1<<18;}
	LPC_GPIOINT->IO0IntEnR=0x00;
	NVIC_DisableIRQ(EINT3_IRQn);
//	LPC_GPIO0->FIODIR = 0x00078000;
	//LPC_GPIO0->FIOCLR = 0x00078000;
	//LPC_GPIO0->FIODIR = 0x00780FF0;
	LPC_GPIO0->FIOCLR |= 0x00780000;
	LPC_GPIO0->FIOMASK= 0xFF87FFFF;
	for(row=0;row<4;row++)
			{
				LPC_GPIO0->FIOCLR |= 0x00780000;
				LPC_GPIO0->FIOMASK= 0xFF87FFFF;
     		if(row == 0){
				temp = 0x00080000;
				LPC_GPIO0->FIOPIN |= temp;
				
				}
				else if(row == 1){
				temp = 0x00100000;
				LPC_GPIO0->FIOPIN |= temp;	

				}
		 		else if(row == 2){
				temp = 0x00200000;
					LPC_GPIO0->FIOPIN |= temp;
					
				}
				else if(row == 3){
				temp = 0x00400000;
					LPC_GPIO0->FIOPIN |= temp;
					}
				flag = 0;
				scan();
				if(flag==1){
					break;
				}
			}
				LPC_GPIO0->FIOMASK=0x007FF000;	
				LPC_GPIOINT->IO0IntEnR=0x00078000;
				NVIC_EnableIRQ(EINT3_IRQn);
				display();
		//			 LPC_GPIO0->FIOPIN |= 0x18000000; //made output P0.27 to P0.28, and 2.0 to 2.1 (rows)
		//LPC_GPIO2->FIOPIN |=0x03;
			
}


 void scan(void)
 {
 	unsigned long temp3;
	temp3 = LPC_GPIO0->FIOPIN;	
	 temp3=temp3&0x00780000;
	if(temp3 != 0x00000000)
	{
					
			if (temp3 ==0x00008000)
			{	col=0; flag = 1;}
			else if (temp3==0x00010000)
			{	col=1;flag = 1;}
			else if (temp3==0x00020000)
			{	col=2; flag = 1;}
			else if (temp3==0x00040000)
			{	col=3; flag = 1;}
   
  }
 
