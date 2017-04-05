/**
  ******************************************************************************
  * @file    stm32_iap.c
  * $Author: DSF $
  * $Revision: 66 $
  * $Date:: 2017-3-29 09:52:35 +0800 #$
  * @brief   Cortex-M3 ����������ط�����.
  ******************************************************************************
  */
/** @addtogroup Ginkgo_Bootloader
  * @{
  */
/** @defgroup Ginkgo_Bootloader_IAP
  * @brief Flash��д����ģ�� 
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32_iap.h"
/* Private define ------------------------------------------------------------*/
#define ApplicationAddress    0x8004000	///<Ӧ�ó�����ʼ��ַ



   
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  ��������д��ָ����ַ��Flash�� ��
  * @param  Address Flash��ʼ��ַ��
  * @param  Data ���ݴ洢����ʼ��ַ��
  * @param  DataNum �����ֽ�����
  * @retval ������д״̬��
  */
FLASH_Status ProgramDatatoFlash(uint32_t *Address,uint16_t *Data,uint32_t DataNum) 
{
	FLASH_Status FLASHStatus=FLASH_COMPLETE;
	uint32_t i;
	uint16_t *p_Data=Data;

	FLASH_Unlock();

	for(i=0;i<(DataNum>>1);i++){
		FLASHStatus=FLASH_ProgramHalfWord(*Address, *p_Data);
		if(FLASHStatus!=FLASH_COMPLETE){
			return	FLASHStatus;	
		}
		*Address+=2;
		p_Data++;
	}
	return	FLASHStatus;
	
}
/**
  * @brief  ����ָ�����������Flash���� ��
  * @param  StartPage ��ʼ����
  * @param  EndPage ��������
  * @retval ��������״̬
  */
FLASH_Status ErasePage(uint32_t StartPage,uint32_t EndPage)
{
	FLASH_Status FLASHStatus=FLASH_COMPLETE;
	uint32_t i;
	int32_t Page=EndPage-StartPage;
	if((Page<0)||(StartPage<8)){
		return FLASH_ERROR_PG;
	}
	FLASH_Unlock();
	for(i=0;i<=Page;i++){
		FLASHStatus = FLASH_ErasePage(0x8000000+(StartPage+i)*PAGE_SIZE);
		if(FLASHStatus!=FLASH_COMPLETE){
			return	FLASHStatus;	
		}
	}
	return	FLASHStatus; 
}

/**
  *	@}
  */
/**
  *	@}
  */

