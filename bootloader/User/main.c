/**
  ******************************************************************************
  * @file    main.c
  * $Author: DSF $
  * $Revision: 66 $
  * $Date:: 2017-3-29 09:48:49 +0800 #$
  * @brief   主函数.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "interface.h"
#include "stm32_iap.h"
#include <stm32f10x.h>
#include <core_cm3.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void reboot()
{
	__set_FAULTMASK(1);     // 关闭所有中断
	NVIC_SystemReset();			// 复位
}


/**
  * @brief  串口打印输出
  * @param  None
  * @retval None
  */
int main(void)
{
	unsigned short data = 0;
	uint32_t	*p_VectorValue;
	SystemInit();//系统时钟初始化
	USART_Configuration();//串口1初始化
	printf("\x0c\0");   //超级终端清屏
	printf("\r\n*******************************************************************************");
	printf("\r\n************************  BootLoader for ECU-R-M3      ************************");
	printf("\r\n***************************  Design by YuNeng @ECU  ***************************");
	printf("\r\n*****************************   2017.03           *****************************");
	printf("\r\n*******************************************************************************");
	printf("\r\n");

	//读取0x0800 4000 前4个字节	
	data = *(unsigned short *)(0x08004000);  //读取更新变量地址
	printf("%d\n",data);
	//1表示需要更新，再比较APP1和APP2是否相同  如果相同则不需要更新(当前程序为最新程序)
	if (data == 0x01 )
	{
		FLASH_Unlock();
		//擦除APP1扇区
		while( 0 != FLASH_If_Erase_APP1())
		{
			printf("Erase APP2 error\n");
		}
		
		//更新APP2到APP1  将0x0808 0000 开始大小为480KB的代码复制到0x0800 8000开始的起始地址
		while( 0 != FLASH_IF_APP2_COPY_TO_APP1())
		{
			printf("IAP error\n");
		}
		
		//擦除APP2代码
		/*
		while( 0 != FLASH_If_Erase_APP2())
		{
			printf("Erase APP2 error\n");
		}
		*/
		
		//将更新标志设置为0 ,表示接下来无需更新 
		while(FLASH_COMPLETE != FLASH_ErasePage(0x08004000))
		{}
		while(FLASH_COMPLETE != FLASH_ProgramHalfWord(0x08004000, 0))
		{}
		reboot();
		//更新成功，跳转到程序入口
		if(*p_VectorValue==0xFFFFFFFF){
			printf("\n->The program is inefficacy!!\n");
		}else{
			printf("\n->Jump to user program!!\n");
			__disable_irq();
			JumpToApplication(AUTO_BOOT_ADDR);
		}
		
	}
	else
	{
		p_VectorValue=(uint32_t *)AUTO_BOOT_ADDR;
		
		if(*p_VectorValue==0xFFFFFFFF){
			printf("\n->The program is inefficacy!!\n");
		}else{
			printf("\n->Jump to user program!!\n");
			__disable_irq();
			JumpToApplication(AUTO_BOOT_ADDR);
		}
	}
	
}

/*********************************END OF FILE**********************************/
