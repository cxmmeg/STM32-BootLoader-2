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
#define ApplicationAddress    0x8008000	///<应用程序起始地址



   
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

/**
  * @brief  This function does an erase of APP1 user flash area
  * @retval 0: user flash area successfully erased
  *         1: error occurred
  */
uint32_t FLASH_If_Erase_APP1()
{
	
  uint32_t UserStartPage = 0x08008000,PageCount = 240, i = 0;

  for(i = 0x08008000; i < (UserStartPage+PageCount*0x800); i += 0x800)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_ErasePage(i) != FLASH_COMPLETE)
    {
      /* Error occurred while page erase */
      return (1);
    }
  }
  
  return (0);
}

/**
  * @brief  This function does an erase of APP2 user flash area
  * @retval 0: user flash area successfully erased
  *         1: error occurred
  */
uint32_t FLASH_If_Erase_APP2()
{
 uint32_t UserStartPage = 0x08080000,PageCount = 256, i = 0;

  for(i = 0x08080000; i < (UserStartPage+PageCount*0x800); i += 0x800)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_ErasePage(i) != FLASH_COMPLETE)
    {
      /* Error occurred while page erase */
      return (1);
    }
  }
  
  return (0);
}

uint32_t FLASH_IF_APP2_COPY_TO_APP1(void)
{
	unsigned int app1addr  = 0x08008000, app2addr = 0x08080000;
	unsigned int app2data;
	while (app1addr < 0x0807FFFF)
	{
		app2data = *(unsigned int *)(app2addr);
		
		if (FLASH_ProgramWord(app1addr, app2data) == FLASH_COMPLETE)
    {
     /* Check the written value */
      if (*(uint32_t*)app1addr != (uint32_t)app2data)
      {
        /* Flash content doesn't match SRAM content */
        return(2);
      }
      /* Increment FLASH destination address */
      app1addr += 4;
			app2addr += 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (1);
    }
	}
	return (0);
}
