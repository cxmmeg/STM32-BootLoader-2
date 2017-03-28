//*================================================================================================
//* 文件名				: interface.h
//* 功能描述			: BootLoader使用界面相关函数定义
//* 作者    			: 罗伟东
//* 版本				: 0.01
//* 建立日期、时间		: 2011/04/27 19:37
//* 最近修改日期、时间	: 
//* 修改原因			: 
//*================================================================================================
#ifndef	__INTERFACE_H
#define	__INTERFACE_H


#define		PROM_ADDR	0X8004000
#define		AUTO_BOOT_ADDR	0X8004000  //16kb

extern void BootPut(void);
uint8_t JumpToApplication(uint32_t Addr);
extern void BootLoaderHelp(void);
extern void CmdLoop(uint32_t cmd);
extern void Reset_Handler(void);
extern void EraseFlashInterface(void);
extern void StartUserProgram(void);

__swi(0x00) void SwiHandle1(int Handle);

#define IRQDisable() SwiHandle1(0)
#define IRQEnable() SwiHandle1(1)
#define FIQDisable() SwiHandle1(2)
#define FIQEnable() SwiHandle1(3)
#define JumToFlash() SwiHandle1(4)

#endif	/*__INTERFACE_H*/
