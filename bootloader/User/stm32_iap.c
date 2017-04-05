/**
  ******************************************************************************
  * @file    stm32_iap.c
  * $Author: DSF $
  * $Revision: 66 $
  * $Date:: 2017-3-29 09:52:35 +0800 #$
  * @brief   Cortex-M3 处理器的相关服务函数.
  ******************************************************************************
  */
/** @addtogroup Ginkgo_Bootloader
  * @{
  */
/** @defgroup Ginkgo_Bootloader_IAP
  * @brief Flash读写操作模块 
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32_iap.h"
/* Private define ------------------------------------------------------------*/
#define ApplicationAddress    0x8004000	///<应用程序起始地址



   
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  将数据烧写到指定地址的Flash中 。
  * @param  Address Flash起始地址。
  * @param  Data 数据存储区起始地址。
  * @param  DataNum 数据字节数。
  * @retval 数据烧写状态。
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
  * @brief  擦出指定扇区区间的Flash数据 。
  * @param  StartPage 起始扇区
  * @param  EndPage 结束扇区
  * @retval 扇区擦出状态
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

