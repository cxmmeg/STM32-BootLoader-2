/**
  ******************************************************************************
  * @file    usart.h
  * $Author: wdluo $
  * $Revision: 66 $
  * $Date:: 2012-08-14 19:09:52 +0800 #$
  * @brief   串口操作相关函数声明.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USART_H
#define _USART_H
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stm32f10x.h>

/* Exported Functions --------------------------------------------------------*/
void USART_RCC_Configuration(void);
void USART_GPIO_Configuration(void);
void USART_Configuration(void);
uint8_t USART_GetChar(void);
void USART_SendChar(uint8_t data);

#endif /*_usart_H*/

/*********************************END OF FILE**********************************/
