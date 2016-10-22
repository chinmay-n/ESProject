#include<LPC17XX.h>
unsigned int i,s,h,q,num,j,n=4,digit[4]={0,0,0,0},col,row,flag,sum,count;
unsigned long a[11]={0x00,0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned long b[4][4]={{0,1,2,3},{4,5,6,7},{8,9,0xB,0xA},{0xA,0xA,0xA,0xA}};
unsigned long temp,pos[4]={0x0870,0x08F0,0x0970,0x09F0},X,inter;

void timer(long);
void EINT3_IRQHandler(void);
void scan(void);
void display(void);
void action(int n);

int main(void){
	SystemCoreClockUpdate();
	SystemInit();	
	LPC_GPIO0->FIODIR =0x07800F00;  //SEVEN SEG      //Select lines p0.23-p0.26
	LPC_GPIO1->FIODIR =0x07800000;	//CNB DATA LINES (LOWER)
	LPC_GPIO2->FIODIR =0x00003C00;	//CNB DATA LINES
	LPC_GPIO0->FIOSET =0x00000F00;	//ROWS SET HIGH 
	LPC_GPIOINT->IO0IntEnR=0x000000F0;// GPIO Interrupt on P0.4-0.7
	NVIC_EnableIRQ(EINT3_IRQn);
	count=0;
	while(1){
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
	timer(299);
	while(LPC_TIM0->EMR!=0x00000021){
					LPC_GPIO0->FIOMASK=~(0x07800000);
					for(j=0;j<n;j++){
					LPC_GPIO1->FIOCLR=0x0F<<23;
					LPC_GPIO2->FIOCLR=0x0F<<10;
					LPC_GPIO0->FIOCLRH=0xFFFF;
					temp=a[digit[j]+1];
					LPC_GPIO1->FIOSET=temp<<23;
					LPC_GPIO2->FIOSET=temp<<6;
					LPC_GPIO0->FIOSETH= pos[j];
					for(q=0;q<50;q++);
					LPC_GPIO0->FIOSETH = 0xFFFF;
					for(q=0;q<50;q++);
					}
					LPC_GPIO0->FIOMASK=0x000000;
		}
}

void EINT3_IRQHandler(void){
	LPC_GPIOINT->IO0IntClr=0x0F0;
	for(i=0;i<50;i++);
	inter=LPC_GPIOINT->IO0IntStatR;
	if(inter){				
	if(inter==1<<4){
		col=0;
	}
	else if(inter==1<<5){
		col=1;
	}
	else if(inter==1<<6){
		col=2;
	}
	else if(inter==1<<7){
		col=3;
	}
	for(row=0;row<4;row++)
			{
				LPC_GPIO0->FIOMASK=~(0x00000F00);
				LPC_GPIO0->FIOCLR=0x00000F00; // clear rows
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
					if (flag==1){break;}
				}
				LPC_GPIO0->FIOMASK=0x00;
				LPC_GPIO0->FIOCLR=0x00000F00;
				num=b[row][col];
				action(num);
				LPC_GPIOINT->IO0IntClr=0x0F0;
				LPC_GPIO0->FIOMASK = ~(0x00000F00);
			  LPC_GPIO0->FIOSET = 0x00000F00; //Setting rows back to high
			}
				return;
}

 void scan(void)
 {
 	unsigned long temp3;
	LPC_GPIO0->FIOMASK=~(0x0F0);
	temp3=LPC_GPIO0->FIOPIN;
	 temp3&=0x000000F0;
	if(temp3 != 0x00000000)
	{
				flag = 1;
	}
	LPC_GPIO0->FIOMASK=(0x00);
 }
 
 void action(int n){
	 if(count==0){
			count=n%0x0A;
			return;
	 }
	 switch (n){
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:	
					digit[3]=digit[2];
					digit[2]=digit[1];
					digit[1]=digit[0];
					digit[0]=n;
					break;
		case 0xA:
					count--;
					sum=sum+(digit[1]*0xA)+(digit[2]*0x0A*0x0A)+digit[0];
					digit[0]=digit[1]=digit[2]=digit[3]=0;
					break;
		case 0x0B:
					digit[0]=digit[1];
					digit[1]=digit[2];
					digit[2]=digit[3];
					break;
	 }
	 s=sum,h=0;
	 if(count==0){
			while(s!=0){
				digit[h]=s%10;
				s=s/10;
				h++;
			}
			return;
	 }
	 display();
 }
