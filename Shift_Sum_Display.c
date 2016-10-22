#include<LPC17XX.h>
unsigned int i,q,num,j,n=4,digit[4]={0,0,0,0},col,row,flag,sum,count;
unsigned long a[11]={0x00,0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned long b[4][4]={{0,1,2,3},{4,5,6,7},{8,9,0xB,0xA},{0xA,0xA,0xA,0xA}};
unsigned long temp,pos[4]={0xF87FFFFF,0xF8FFFFFF,0xF97FFFFF,0xF9FFFFFF},X,inter;

void timer(long);
void EINT3_IRQHandler(void);
void scan(void);
void display(void);
void action(int n);

int main(void){
	SystemCoreClockUpdate();
	SystemInit();	
	//Seven Segment      
	LPC_GPIO0->FIODIR |=0x0FF0;  //Data lines p0.4-p0.11
	LPC_GPIO1->FIODIR =0x07800000;  //Select lines p1.23-p1.26
	LPC_GPIO0->FIODIR |= 0x18000000; //made output P0.27 to P0.28
	LPC_GPIO2->FIODIR = 0x00000003;		//, and 2.0 to 2.1 (rows)
	LPC_GPIO0->FIOSET=0x18000000; // set rows,
	LPC_GPIO2->FIOSET =0x03;					// as high
	LPC_GPIOINT->IO0IntEnR=0x07800000;// GPIO Interrupt on P0.23-0.26
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
		for(j=0;j<n;j++){
					temp=a[digit[j]+1];
					LPC_GPIO0->FIOPIN = temp<<4;
					LPC_GPIO1->FIOPIN = pos[j];
					for(q=0;q<50;q++);
					LPC_GPIO1->FIOPIN &= 0xFFFFFFFF;
					for(q=0;q<50;q++);
			}
		}
}

void EINT3_IRQHandler(void){
	inter=LPC_GPIOINT->IO0IntStatR;
	LPC_GPIO0->FIOCLR=0x18000000; // Clear Rows
	LPC_GPIO2->FIOCLR =0x03;					
	if(inter==1<<23){
		col=0;
		LPC_GPIOINT->IO0IntClr|=1<<23;
	}
	else if(inter==1<<24){
		col=1;
		LPC_GPIOINT->IO0IntClr|=1<<24;
	}
	else if(inter==1<<25){
		col=2;
		LPC_GPIOINT->IO0IntClr|=1<<25;
	}
	else if(inter==1<<26){
		col=3;
		LPC_GPIOINT->IO0IntClr|=1<<26;
	}
	//NVIC_DisableIRQ(EINT3_IRQn);
	LPC_GPIO0->FIOMASKL=0xFFFF;
	/*for(row=0;row<4;row++)
			{
     		if(row == 0){
				temp = 0x08000000;
				LPC_GPIO0->FIOSET = temp;}
				else if(row == 1){
				temp = 0x10000000;
				LPC_GPIO0->FIOSET = temp;	}
		 		else if(row == 2){
				temp = 0x00000001;
					LPC_GPIO2->FIOSET = temp;
				}
				else if(row == 3){
				temp = 0x00000002;
					LPC_GPIO2->FIOSET = temp;
					}
				flag = 0;
				scan();	
				//NVIC_EnableIRQ(EINT3_IRQn);
					if (flag==1){break;}
				}*/
				row=2;
				LPC_GPIO0->FIOMASKL=0x0000;
				num=b[row][col];
				action(num);
			  LPC_GPIO0->FIOSET = 0x18000000; //Setting rows back to high
				LPC_GPIO2->FIOSET =0x03;
				return;
}


 void scan(void)
 {
 	unsigned long temp3;
	temp3 = LPC_GPIO0->FIOSET;	
	temp3 =LPC_GPIO2->FIOSET;
	 temp3&=0x07800000;
	if(temp3 != 0x00000000)
	{
				flag = 1;
			if (temp3 ==0x00800000)
				col=0;
			else if (temp3==0x01000000)
				col=1;
			else if (temp3==0x02000000)
				col=2;
			else if (temp3==0x04000000)
				col=3;
   
	}
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
					digit[2]=digit[1];
					digit[1]=digit[0];
					break;
	 }
	 display();
 }
