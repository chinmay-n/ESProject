#include<LPC17XX.h>
unsigned int i,q,num,j,n=4,digit[4]={0,0,0,0},col,row,flag,dig;
unsigned long a[11]={0x00,0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned long b[4][4]={{0,1,2,3},{4,5,6,7},{8,9}};
unsigned long temp,pos[4]={0x0870,0x08F0,0x0970,0x09F0},X,inter;

void timer(long);
void EINT3_IRQHandler(void);
void scan(void);
void display(void);

int main(void){
	SystemCoreClockUpdate();
	SystemInit();	
	LPC_GPIO0->FIODIR =0x07800F00;  //SEVEN SEG      //Select lines p0.23-p0.26
	LPC_GPIO1->FIODIR =0x07800000;	//CNB DATA LINES (LOWER)
	LPC_GPIO2->FIODIR =0x00003C00;	//CNB DATA LINES
	LPC_GPIO0->FIOSET =0x00000F00;	//ROWS SET HIGH 
	LPC_GPIOINT->IO0IntEnR=0x000000F0;// GPIO Interrupt on P0.4-0.7
	NVIC_EnableIRQ(EINT3_IRQn);
	while(1){
		display();
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
	timer(299999);
	while(LPC_TIM0->EMR!=0x00000021){
					num=b[row][col];
					temp=a[num+1];
					LPC_GPIO1->FIOPIN=temp<<23;
					LPC_GPIO2->FIOPIN=temp<<6;
					LPC_GPIO0->FIOPINH= pos[0];
					for(q=0;q<50;q++);
					LPC_GPIO0->FIOPINH = 0xFFFF;
					for(q=0;q<50;q++);
		}
}

void EINT3_IRQHandler(void){
	inter=LPC_GPIOINT->IO0IntStatR;
	//for(i=0;i<2000;i++);
	LPC_GPIO0->FIOCLR=0x00000F00; // clear rows				
	if(inter==1<<4){
		col=0;
		//LPC_GPIOINT->IO0IntClr|=1<<23;
	}
	else if(inter==1<<5){
		col=1;
		//LPC_GPIOINT->IO0IntClr|=1<<24;
	}
	else if(inter==1<<6){
		col=2;
		//LPC_GPIOINT->IO0IntClr|=1<<25;
	}
	else if(inter==1<<7){
		col=3;
		//LPC_GPIOINT->IO0IntClr|=1<<26;
	}
	NVIC_DisableIRQ(EINT3_IRQn);
	for(row=0;row<4;row++)
			{
     		if(row == 0){
				temp = 0x00000100;
				LPC_GPIO0->FIOSET = temp;}
				else if(row == 1){
				temp = 0x00000200;
				LPC_GPIO0->FIOSET = temp;	}
		 		else if(row == 2){
				temp = 0x00000400;
					LPC_GPIO0->FIOSET = temp;
				}
				else if(row == 3){
				temp = 0x00000800;
					LPC_GPIO0->FIOSET = temp;
					}
				flag = 0;
				scan();	
				//NVIC_EnableIRQ(EINT3_IRQn);
					if (flag==1){break;}
				}
				LPC_GPIOINT->IO0IntClr=0x0F0;
				NVIC_EnableIRQ(EINT3_IRQn);
				display();
				LPC_GPIO0->FIOSET = 0x00000F00; //Setting rows back to high
				return;
}


 void scan(void)
 {
 unsigned long temp3;
	temp3=LPC_GPIO0->FIOPIN;
	 temp3&=0x000000F0;
	if(temp3 != 0x00000000)
	{
				flag = 1;
			if (temp3 ==0x00000010)
				col=0;
			else if (temp3==0x00000020)
				col=1;
			else if (temp3==0x00000040)
				col=2;
			else if (temp3==0x00000080)
				col=3;
   
	}
}
