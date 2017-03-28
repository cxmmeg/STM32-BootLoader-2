//*================================================================================================
//* 文件名				: ProgramFlash.c
//* 功能描述			: 将解析后的Hex数据烧写在Flash中的应用层函数
//* 作者    			: 罗伟东
//* 版本				: 0.01
//* 建立日期、时间		: 2011/04/15 20:33
//* 最近修改日期、时间	: 
//* 修改原因			: 
//*================================================================================================
/*------------------------------------------ 头文件 ---------------------------------------------*/
#include	"decodeHex.h"
#include	"ProgramFlash.h"
#include	"interface.h"
#include	"stm32_iap.h"
/*-----------------------------------------------------------------------------------------------*/

/*----------------------------------------- 全局变量 --------------------------------------------*/
extern uint8_t	g_ProgramFlashDataTemp[BUFFER_SIZE];
uint32_t	g_AddrBase=0x800;//hex数据存储起始地址
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------- 自定义数据类型 -----------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/

//*================================================================================================
//*											函	数	区
//*================================================================================================

/**************************************************************************************************
** 函数名称 : ProgramHexToFlash
** 功能描述 : 将一行HEX文件数据烧写在Flash中
** 入口参数 : <p_HexData>[in] 准备烧写的一行HEX文件数据首地址
**			: <UseDefaultAddr>[in] 是否使用HEX文件数据中的默认地址
**								   TRUE:使用默认地址	FALSE:使用UserAddr指定的地址
**			: <UserAddr>[in] 指定的Flash地址
**			: <HexDataTemp>[in] 暂存烧入Flash中的数据缓冲区首地址
** 出口参数 : 无
** 返 回 值 : 若烧写成功则返回0
** 其他说明 : UserAddr地址必须是0X100的整数倍。HexDataTemp最好是全局变量数组首地址
***************************************************************************************************/
uint8_t ProgramHexToFlash(uint8_t *p_HexData,uint32_t AddrSelect,volatile uint32_t UserAddr)
{
	volatile uint8_t 	ErrorCode=0;//需要返回的错误代码
	t_HexLineInfo 	HexDataInfo;//暂时存储HEX数据的相关信息
	uint8_t	CanGetData=TRUE;//判断数据是否全部解析并已经存入缓存标志
	uint32_t	HexData_Index_Temp=0;
	uint32_t	HexDataTemp_Index_Temp=0;
	uint32_t	i=0;
	uint32_t	DST=0;
	volatile uint32_t	FirstAddr=0;
	volatile uint8_t	HaveGetAddrFlag=FALSE;
	volatile uint32_t	Sector=0;
	for(i=0;i<BUFFER_SIZE;i++){		//初始化需要写入Flash的数据缓冲区
		g_ProgramFlashDataTemp[i]=0xFF;	
	}
	/*将p_HexData所指向的存储块中的HEX数据解析到g_ProgramFlashDataTemp所指向的缓冲区中*/
	while(CanGetData==TRUE){
		ErrorCode=GetHexLineInfo(p_HexData+HexData_Index_Temp,&HexDataInfo);//获取HEX数据信息
		if((HaveGetAddrFlag==FALSE)&&(HexDataInfo.Rectype==RCD_DATA)){
			FirstAddr=HexDataInfo.OffsetAddr;
			HaveGetAddrFlag=TRUE;
		}
		HexData_Index_Temp+=((HexDataInfo.DataLength)*2+13);
		if(ErrorCode==RIGHT){
			if(HexDataInfo.Rectype==RCD_DATA){
				ErrorCode=GetHexData(&HexDataInfo,g_ProgramFlashDataTemp+HexDataTemp_Index_Temp);//将HEX数据提出出来存入HexDataTemp数据缓冲区
				HexDataTemp_Index_Temp+=HexDataInfo.DataLength;
			}else if(HexDataInfo.Rectype==RCD_END_FILE){//接收到文件结尾跳出循环
			   	CanGetData=FALSE;
			}else if(HexDataInfo.Rectype==RCD_EXT_LINEAR_ADDR){
				//CanGetData=FALSE;
			}else if(HexDataInfo.Rectype==RCD_START_LINEAR_ADDR){

			}else if(HexDataInfo.Rectype==RCD_EXT_SEGMENT_ADDR){
				//CanGetData=FALSE;
			}else if(HexDataInfo.Rectype==RCD_START_SEGMENT_ADDR){

			}else{
				CanGetData=FALSE;
			}
		}
		else{
			CanGetData=FALSE;
		}
		if(p_HexData[HexData_Index_Temp]!=':'){//判断该次接收的数据是否处理完毕
			CanGetData=FALSE;
		}
	}
	/*将其缓冲区数据写入Flash中*/
	if((ErrorCode==RIGHT)&&(AddrSelect==USE_USER_DEFINED_ADDR)){//数据正确且利用指定地址烧写数据
		DST=(g_AddrBase<<16)+(FirstAddr+UserAddr);	
		ErrorCode = ProgramDatatoFlash(&DST,(uint16_t*)g_ProgramFlashDataTemp,BUFFER_SIZE);
	}else if((ErrorCode==RIGHT)&&(AddrSelect==USE_DFT_ADDR)){//数据正确且利用HEX文件数据里的地址烧写数据
		DST=(g_AddrBase<<16)+FirstAddr;
		ErrorCode = ProgramDatatoFlash(&DST,(uint16_t*)g_ProgramFlashDataTemp,BUFFER_SIZE);	
	}else{
		ErrorCode=1;
	}
	return ErrorCode;		
}
/**************************************************************************************************
** 函数名称 : EraseFlash
** 功能描述 : 擦除选中的Flash扇区数据
** 入口参数 : <StartSector>[in] 需要擦除的起始扇区
**			: <EndSector>[in] 需要擦除的终止扇区
** 出口参数 : 无
** 返 回 值 : 若擦除成功则返回0
** 其他说明 : EndSector必须大于等于StartSector
***************************************************************************************************/
uint8_t EraseFlash(uint32_t StartSector,uint32_t EndSector)
{
	uint8_t ErrorCode=0;
	if((EndSector<StartSector)||(StartSector<8)){
		ErrorCode=1;
		return ErrorCode;
	}else{
		//ErrorCode=PrepareSectors(StartSector,EndSector);
		ErrorCode=ErasePage(StartSector,EndSector);
		return ErrorCode; 
	}
}
/**************************************************************************************************
** 函数名称 : DetectSector
** 功能描述 : 根据输入的地址信息判断该地址处于那个扇区
** 入口参数 : <Address>[in] 需要判断的地址
** 出口参数 : 无
** 返 回 值 : 地址对应的扇区值
** 其他说明 : 无
***************************************************************************************************/
uint8_t DetectSector(uint32_t Address)
{
	return (Address - 0x8000000)/PAGE_SIZE;
}

