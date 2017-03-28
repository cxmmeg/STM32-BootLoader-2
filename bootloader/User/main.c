/**
  ******************************************************************************
  * @file    main.c
  * $Author: wdluo $
  * $Revision: 66 $
  * $Date:: 2012-08-14 19:09:52 +0800 #$
  * @brief   主函数.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "decodeHex.h"
#include "ProgramFlash.h"
#include "xmodem.h"
#include "interface.h"
#include "timer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
uint8_t		g_ProgramFlashDataTemp[BUFFER_SIZE];
uint32_t	g_starTimeTemp=9;//进入BootLoader延时数，单位为秒
uint32_t	g_UserProgramAddr=0;//用户程序执行地址
uint32_t	g_HaveGetAddr=0;//获取用户程序执行地址标志，若已经获取则为1
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  串口打印输出
  * @param  None
  * @retval None
  */
int main(void)
{
	uint32_t	*p_VectorValue;
	uint32_t	t=0;
	SystemInit();//系统时钟初始化
	USART_Configuration();//串口1初始化
	USART1_Timer3_Config();
	printf("\x0c\0");   //超级终端清屏
//	printf("\033[1;40;32m");             //设置超级终端背景为黑色，字符为绿色
	printf("\r\n*******************************************************************************");
	printf("\r\n************************  BootLoader for STM32F107RC   ************************");
	printf("\r\n***************************  Design by YuNeng @ECU  ***************************");
	printf("\r\n*****************************   2017.03           *****************************");
	printf("\r\n*******************************************************************************");
	printf("\r\n");
	//BootPut();
	printf("Press 'h' key for help! \r\n");
	printf("Press any key to interrupt autoboot:%d",g_starTimeTemp); //
	for(t=g_starTimeTemp;t!=0;t--)
	{
		if(port_inbyte(500)>0)
		{
		printf("\n\r->");
		break;
		}
		else
		{
		printf("\b%d",g_starTimeTemp);		//显示时间
		/*此处添加应用程序跳转函数*/
		}
	}
	if(g_starTimeTemp==0){
		p_VectorValue=(uint32_t *)AUTO_BOOT_ADDR;
		/*for(i=8;i!=0;i--){
			CheckSum+=(*(p_VectorValue++));
		} */
		if(*p_VectorValue==0xFFFFFFFF){
			printf("\n\r->The program is inefficacy!!");
			printf("\n\r->");
		}else{
			printf("\n\r->Jump to user program!!\n\r");
			__disable_irq();
			JumpToApplication(AUTO_BOOT_ADDR);
		}
		//printf("\n\r->");
	}
	//xmodemReceive(CHECK_CRC);
	while(1)
	{
		CmdLoop(USART_GetChar());
		//if(getkey()=='j'){JumpToFlash(0x3000);}
//		t = 0;
//		t = port_inbyte(10);
//		if(t!=0){
//			port_outbyte(t);
//		}
	}
}


/*********************************END OF FILE**********************************/
