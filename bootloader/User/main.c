/**
  ******************************************************************************
  * @file    main.c
  * $Author: DSF $
  * $Revision: 66 $
  * $Date:: 2017-3-29 09:48:49 +0800 #$
  * @brief   ������.
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
	__set_FAULTMASK(1);     // �ر������ж�
	NVIC_SystemReset();			// ��λ
}


/**
  * @brief  ���ڴ�ӡ���
  * @param  None
  * @retval None
  */
int main(void)
{
	unsigned short data = 0;
	uint32_t	*p_VectorValue;
	SystemInit();//ϵͳʱ�ӳ�ʼ��
	USART_Configuration();//����1��ʼ��
	printf("\x0c\0");   //�����ն�����
	printf("\r\n*******************************************************************************");
	printf("\r\n************************  BootLoader for ECU-R-M3      ************************");
	printf("\r\n***************************  Design by YuNeng @ECU  ***************************");
	printf("\r\n*****************************   2017.03           *****************************");
	printf("\r\n*******************************************************************************");
	printf("\r\n");

	//��ȡ0x0800 4000 ǰ4���ֽ�	
	data = *(unsigned short *)(0x08004000);  //��ȡ���±�����ַ
	printf("%d\n",data);
	//1��ʾ��Ҫ���£��ٱȽ�APP1��APP2�Ƿ���ͬ  �����ͬ����Ҫ����(��ǰ����Ϊ���³���)
	if (data == 0x01 )
	{
		FLASH_Unlock();
		//����APP1����
		while( 0 != FLASH_If_Erase_APP1())
		{
			printf("Erase APP2 error\n");
		}
		
		//����APP2��APP1  ��0x0808 0000 ��ʼ��СΪ480KB�Ĵ��븴�Ƶ�0x0800 8000��ʼ����ʼ��ַ
		while( 0 != FLASH_IF_APP2_COPY_TO_APP1())
		{
			printf("IAP error\n");
		}
		
		//����APP2����
		/*
		while( 0 != FLASH_If_Erase_APP2())
		{
			printf("Erase APP2 error\n");
		}
		*/
		
		//�����±�־����Ϊ0 ,��ʾ������������� 
		while(FLASH_COMPLETE != FLASH_ErasePage(0x08004000))
		{}
		while(FLASH_COMPLETE != FLASH_ProgramHalfWord(0x08004000, 0))
		{}
		reboot();
		//���³ɹ�����ת���������
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
