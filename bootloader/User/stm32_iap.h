/*******************************************************************************
** 文件名称 : stm32_iap.h
** 软件版本 : 1.0
** 编译环境 : RealView MDK-ARM 4.20
** 文件作者 : 	
** 功能说明 : Bootloader相关函数声明
**
**                       (c) Copyright 2007-2012, viewtool
**                            http://www.viewtool.com
**                               All Rights Reserved
**
*******************************************************************************/
#ifndef __STM32_IAP_H__
#define __STM32_IAP_H__

#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
 #define PAGE_SIZE                         (0x400)    /* 1 Kbyte */
 #define FLASH_SIZE                        (0x20000)  /* 128 KBytes */
#elif defined STM32F10X_CL
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x40000)  /* 256 KBytes */
#elif defined STM32F10X_HD
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x80000)  /* 512 KBytes */
#elif defined STM32F10X_XL
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x100000) /* 1 MByte */
#else 
 #error "Please select first the STM32 device to be used (in stm32f10x.h)"    
#endif  

typedef  void (*pFunction)(void);

extern uint8_t JumpToApplication(uint32_t Addr);
FLASH_Status ProgramDatatoFlash(uint32_t *Address,uint16_t *Data,uint32_t DataNum);
FLASH_Status ErasePage(uint32_t StartPage,uint32_t EndPage); 

#endif /*__VTBOOTLOADER_H__*/
