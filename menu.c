/*
 * menu.c
 *
 * Created: 04.04.2024 16:23:52
 *  Author: msukd
 */ 

#include "menu.h"

FILE UARTout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);

uint8_t stav=0;
// Definice enum pro tlaèítka
typedef enum {
	SELECT,
	LEFT,
	DOWN,
	UP,
	RIGHT,
	RST,
	NONE
} Button;


void menu(int *data)
{
	stav=1;
	while (stav<10)
	{
		LCD_set_cursor(0,0);
		LCD_clear_row(0);
		LCD_clear_row(1);
		
			switch (stav) {
				case 0:
				stav=15;
				break;
				case 1:
					printf("minuty:");
					if (menu_set(data,1)==1)
					{
						stav++;
					}else
					{
						stav--;
					}
				break;
				case 2:
					printf("hodiny:");
					if (menu_set(data,2)==1)
					{
						stav++;
					}else
					{
						stav--;
					}
				break;
				case 3:
					printf("dny:");
					if (menu_set(data,3)==1)
					{
						stav++;
					}else
					{
						stav--;
					}
				break;
				case 4:
					printf("mesice:");
					if (menu_set(data,4)==1)
					{
						stav++;
					}else
					{
						stav--;
					}
				break;
				case 5:
					printf("hystereze:");
					if (menu_set(data,6)==1)
					{
						stav++;
					}else
					{
						stav--;
					}
				break;
				case 6:
					printf("kalibrace:");
					if (menu_set(data,8)==1)
					{
						stav++;
						
					}else
					{
						stav--;
					}
				break;
				case 7:
				printf("Teplota topeni:");
				if (menu_set(data,10)==1)
				{
					stav++;
				}else
				{
					stav--;
				}
				break;
				case 8:
				printf("Teplota chlazeni:");
				if (menu_set(data,12)==1)
				{
					stav++;
				}else
				{
					stav--;
				}
				break;
				default:
				stav++;
				break;
			}
	}

}

uint8_t menu_set(int *data,uint8_t ind)
{
	Button but=LCD_Button_adc_value(LCD_button());
	while (but!=RIGHT)
	{
			but=LCD_Button_adc_value(LCD_button());
			LCD_set_cursor(1,0);
			if(ind<6)
			{
				printf("  %d        ",data[ind]);
			}else
			{
				printfloat(int_to_float(data[ind],data[ind+1]));
			}
			if (!(PINB & (1 << DDB7)))
				{
					while (!(PINB & (1 << DDB7)))
					{
					}
					numberToMorse(data[ind]);
				}
			
			switch(but) {
				case UP:
					data[ind]++;
				break;
				case DOWN:
				if ((data[ind]-1)>-1)
				{
					data[ind]--;
				}else
				{
					LCD_set_cursor(1,0);
					printf("minimum je 0");
				}
				break;
				case LEFT:
					return 0;
				break;
				default:
				break;
			}
	}
	return 1;
}

void menu_teplota (int *data)
{
	Button but=LCD_Button_adc_value(LCD_button());
	float T1=int_to_float(data[10],data[11]);
	float T2=int_to_float(data[12],data[13]);
	float hystereze=int_to_float(data[6],data[7]);
	switch(but) {
	case LEFT:
		LCD_clear_row(0);
		LCD_clear_row(1);
		printf("Teplota topeni");
		LCD_set_cursor(1,0);
		if ((T1+hystereze-1)>(T2+hystereze))
			{
				data[10]--;
				printfloat(T1);
			}
			else
			{
				printf("zmen hysterezi");
			}
		break;
	case RIGHT:
		data[10]++;
		LCD_clear_row(0);
		LCD_clear_row(1);
		printf("Teplota topeni");
		LCD_set_cursor(1,0);
		printfloat(int_to_float(data[10],data[11]));
		break;
	case UP:
		LCD_clear_row(0);
		LCD_clear_row(1);
		printf("Teplota chlazeni");
		LCD_set_cursor(1,0);
		if ((T1+hystereze-1)>(T2+hystereze))
			{
				data[12]++;
				printfloat(int_to_float(data[12],data[13]));
			}
			else
			{
				printf("zmen hysterezi");
			}
	break;
	case DOWN:
		LCD_clear_row(0);
		LCD_clear_row(1);
		printf("Teplota chlazeni");
		LCD_set_cursor(1,0);
		if ((T2-1)>0)
			{
				data[12]--;
				printfloat(int_to_float(data[12],data[13]));
			}
			else
			{
				data[12]=0;
				printf("minimum");
			}
	break;
	default:
	break;
	}
}

void menu_rem(int data)
{
	LCD_clear_row(0);
	LCD_clear_row(1);
	LCD_set_cursor(0,0);
	printf("end remote pres:");
	LCD_set_cursor(1,0);
	printf("button or ESC");
	fprintf(&UARTout,"\nzruseni nastavene hodnoty:enter\n zruseni remote modu:tlacitko,esc\n");
	fprintf(&UARTout,"format xx\n sekundy:s\n minuty:m\n hodiny:h\n dny:d\n mesice:n\n roky:y\n ");
	fprintf(&UARTout,"format xx:xx\n hystereze:o\n kalibrace:k\n Teplota topeni:T\n teplota chlazeni:t\n");
	fprintf(&UARTout,"Hodnoty se ulozi az v normalnim modu\n");
	char read=USART_interupt_read();
	while (read!=27&&(PINB & (1<<DDB7)))
	{
		read=USART_interupt_read();
		switch (read){
			case 's':
				rem_set(data,0);
			break;
			case 'm':
				rem_set(data,1);
			break;
			case 'h':
				rem_set(data,2);
			break;
			case 'd':
				rem_set(data,3);
			break;
			case 'n':
				rem_set(data,4);
			break;
			case 'y':
				rem_set(data,5);
			break;
			case 'o':
				fprintf(&UARTout,"\ncele cisla ");
				rem_set(data,6);
				fprintf(&UARTout,"\ndesetine cisla ");
				rem_set(data,7);
				rem_t_hys(data);
			break;
			case 'k':
				rem_kal_set(data,14);
				fprintf(&UARTout,"\ncele cisla ");
				rem_set(data,8);
				fprintf(&UARTout,"\ndesetine cisla ");
				rem_set(data,9);
			break;
			case 'T':
				fprintf(&UARTout,"\ncele cisla ");
				rem_set(data,10);
				fprintf(&UARTout,"\ndesetine cisla ");
				rem_set(data,11);
				rem_t_hys(data);
			break;
			case 't':
				fprintf(&UARTout,"\ncele cisla ");
				rem_set(data,12);
				fprintf(&UARTout,"\ndesetine cisla ");
				rem_set(data,13);
				rem_t_hys(data);
			break;
			default:
			break;
		}
	}
	if (!(PINB & (1<<DDB7)))
		{
			while (!(PINB & (1<<DDB7)))
			{}
		}
	USART_interupt_set(' ',-1);
	LCD_set_cursor(1,0);
	printf("wait         ");
	fprintf(&UARTout,"\n X vse nastaveno uzi si den ");
}

int rem_set(int *data,uint8_t ind)
{
	char read=USART_interupt_read();
	int num=0;
	uint8_t i=0;
	fprintf(&UARTout,"   Aktualni:%d\n",data[ind]);
	while (read!=13)
		{
		read=USART_interupt_read();
		if(read>47&&read<58)
			{
				if (i==0)
				{
					num=read-48;
					i++;
					USART_interupt_set(' ',1);
				}
				else
				{
					num=10*num+(read-48);
					data[ind]=num;
					USART_interupt_set(' ',2);
					return 1;
				}
			}
		if (!(PINB & (1<<DDB7))||read==27)
		{
			return 0;
		}
	}
	return 0;
	}
	
void rem_kal_set(int *data,uint8_t ind)
	{
	char read=USART_interupt_read();
	fprintf(&UARTout,"   Zadejte znamenko:");
	while (read!=13)
		{
		read=USART_interupt_read();
		if(read==43)
			{
				data[ind]=1;
				return;
			}
		if(read==45)
			{
				data[ind]=0;
				return;
			}
		if (!(PINB & (1<<DDB7))||read==27)
		{
			return;
		}
	}
	return;
	}
	
void rem_t_hys(int *data)
	{
		float T1=int_to_float(data[10],data[11]);
		float T2=int_to_float(data[12],data[13]);
		float hystereze=int_to_float(data[6],data[7]);
		if((T1-hystereze)<T2)
		{
			fprintf(&UARTout,"Teplota topeni nastavena na minimalni moznou dle Hystereze");
			if(data[13]+data[7]>99)
			{
				data[10]=data[12]+data[6]+1;
				data[11]=data[13]+data[7]-100;
			}else
			{
				data[10]=data[12]+data[6];
				data[11]=data[13]+data[7];
			}
		}
	}
	