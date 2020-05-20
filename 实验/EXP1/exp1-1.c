#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "debug.h"
#include "gpio.h"
#include "hw_types.h"
#include "pin_map.h"
#include "sysctl.h"


#define   FASTFLASHTIME			(uint32_t) 300000
#define   SLOWFLASHTIME			(uint32_t) FASTFLASHTIME*10


void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);
void		PF0_Flash(void);
void		PF1_Flash(void);
int main(void)
{
	uint32_t read_key_value1;
	int flag=0,lastflag=0,num=0;
	uint32_t times;
	uint32_t delay_time;
	S800_GPIO_Init();
	while(1)
  {
						//read the PJ0 key value
		//read_key_value2 = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_1)	;
			if(num==0){
				read_key_value1 = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	;
				if (read_key_value1==0){
					PF0_Flash();
					flag = 1;
				}
				if (flag==1&& read_key_value1==1){
					num++;
				}
	    }
			
		if (num==1){
			read_key_value1 = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	;
			if(flag==1&& read_key_value1==1){
				PF0_Flash();
			}
			if(flag==1&& read_key_value1==0){
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);	
				num++;
				lastflag = 1;
			}
		}
		
		if (num==2){
			read_key_value1 = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	;
			if (read_key_value1==0&&lastflag == 1){
				PF0_Flash();
				
			}
			if (read_key_value1==1){
					lastflag = 0;
				}
			if (read_key_value1==0&&lastflag == 0){
				PF1_Flash();
				num++;
			}		
		}
		
		if (num==3){
			read_key_value1 = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	;
			if(read_key_value1==0&&lastflag == 0){
				PF1_Flash();
			}
			if(read_key_value1==1&&lastflag == 0){
				PF1_Flash();
				lastflag = 1;
				num++;
			}
		}
		
		if (num==4){
			read_key_value1 = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	;
			if (lastflag == 1&&read_key_value1==1){
				PF1_Flash();
			}
			if (read_key_value1==0){
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);	
				num++;
				flag = 1;
			}
		}
		
		if (num==5){
			read_key_value1 = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	;
			if (read_key_value1==1){
				flag = 0;
			}
			if (flag==0&&read_key_value1==0){
				delay_time							= FASTFLASHTIME;	
				Delay(delay_time);
				num = 0;
				lastflag = 0;
			}
		}
   }
}

void PF0_Flash(void)
{
	uint32_t delay_time;
		//if (key_value1	== 0)	{					//USR_SW1-PJ0 pressed
			delay_time							= FASTFLASHTIME;	
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);			// Turn on the LED.
			Delay(delay_time);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);							// Turn off the LED.
		  Delay(delay_time);
		//}
		//else													//USR_SW1-PJ0 released
			//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);							// Turn off the LED.
}


void PF1_Flash(void)
{
	uint32_t delay_time;
		//if (key_value1	== 0)	{					//USR_SW1-PJ0 pressed
			delay_time							= FASTFLASHTIME;	
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);			// Turn on the LED.
			Delay(delay_time);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);							// Turn off the LED.
		  Delay(delay_time);
		//}
		//else													//USR_SW1-PJ0 released
			//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);							// Turn off the LED.
}

void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}


void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);						//Enable PortF
	//函数原型：void SysCtlPeripheralEnable(uint32_t ui32Peripheral)
	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	//函数原型：bool SysCtlPeripheralReady(uint32_t ui32Peripheral)
	//如果指定的外设被使能成功，返回true，否则返回false
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);						//Enable PortJ	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));			//Wait for the GPIO moduleJ ready	
	
	
	
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);			//Set PF0 as Output pin
	//函数原型：void GPIOPinTypeGPIOOutput(uint32_t ui32Port, uint8_t ui8Pins)
	//配置GPIO端口引脚为输出引脚，如果字符型（uint8_t）参数ui8Pins某位为1，则GPIO端口对应位配置为输出引脚
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);	
	
	
	//GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_4 | GPIO_PIN_5);
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1);//Set the PJ0,PJ1 as input pin
	//配置GPIO端口引脚为输入引脚，与GPIOPinTypeGPIOOutput()类似。GPIO_PIN_0 | GPIO_PIN_1 = 00000011b
	
	//GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_4 | GPIO_PIN_5,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	//函数原型：void GPIOPadConfigSet(uint32_t ui32Port, uint8_t ui8Pins, uint32_t ui32Strength, uint32_t ui32PinType)
	//GPIO端口配置。uint32_t ui32Port：GPIO端口基地址
	//ui8Pins：端口引脚位组合表示，如10000001b表示配置端口的D7、D0位
	//ui32Strength：端口的输出驱动能力，对输入设置无效，可选项包括GPIO_STRENGTH_2MA/4MA/8MA/8MA_SC/6MA/10MA/12MA
	//ui32PinType：引脚类型，可选项包括GPIO_PIN_TYPE_STD（推挽）、GPIO_PIN_TYPE_STD_WPU（推挽上拉）、GPIO_PIN_TYPE_STD_WPD（推挽下拉）、
	//GPIO_PIN_TYPE_OD（开漏）、GPIO_PIN_TYPE_ANALOG（模拟）、GPIO_PIN_TYPE_WAKE_HIGH（高电平从冬眠唤醒）、GPIO_PIN_TYPE_WAKE_LOW（低）
}


