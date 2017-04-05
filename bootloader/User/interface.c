//*================================================================================================
//* �ļ���				: interface.c
//* ��������			: BootLoader��������
//* ����    			: DSF
//* �汾				: 0.01
//* �������ڡ�ʱ��		: 2017-3-29 09:52:09
//* ����޸����ڡ�ʱ��	: 
//* �޸�ԭ��			: 
//*================================================================================================
/*------------------------------------------ ͷ�ļ� ---------------------------------------------*/
#include	<stdio.h>
#include	"decodeHex.h"
#include	"xmodem.h"
#include	"interface.h"
#include	"stm32_iap.h"
#include	"usart.h"
/*-----------------------------------------------------------------------------------------------*/

/*----------------------------------------- ȫ�ֱ��� --------------------------------------------*/
extern uint32_t	g_UserProgramAddr;//�û�����ִ�е�ַ
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------- �Զ����������� -----------------------------------------*/
typedef  void (*pFunction)(void);
/*-----------------------------------------------------------------------------------------------*/

//*================================================================================================
//*											��	��	��
//*================================================================================================

/**
  * @brief  ���Ƴ�����ת��ָ��λ�ÿ�ʼִ�� ��
  * @param  Addr ����ִ�е�ַ��
  * @retval ������ת״̬��
  */
uint8_t JumpToApplication(uint32_t Addr)
{
	pFunction Jump_To_Application;
	__IO uint32_t JumpAddress; 
	/* Test if user code is programmed starting from address "ApplicationAddress" */
	if (((*(__IO uint32_t*)Addr) & 0x2FFE0000 ) == 0x20000000)
	{ 
	  /* Jump to user application */
	  JumpAddress = *(__IO uint32_t*) (Addr + 4);
	  Jump_To_Application = (pFunction) JumpAddress; 
	  /* Initialize user application's Stack Pointer */
	  __set_MSP(*(__IO uint32_t*) Addr);
	  #ifdef USE_USB_PORT
	  USB_Cable_Config(DISABLE);
	  #endif
	  Jump_To_Application();
	}
//	TimeOutFlag=0xFF;
	return 1;
}
/**************************************************************************************************
** �������� : BootPut
** �������� : ��ʾBootLoader�������
** ��ڲ��� : ��
** ���ڲ��� : ��
** �� �� ֵ : ��
** ����˵�� : ��
***************************************************************************************************/
void BootPut(void)
{	 	
	//USART_SendChar(0x0C);USART_SendChar(0x0C);
	printf("===============================================================================\n\r");
	printf("**************************Bootloader for ECU-R-M3 V0.01*************************\n\r");
	printf("************************************View Tool**********************************\n\r");
	printf("Type H For Help\n\r");
	printf("===============================================================================\n\r");
	printf("->");	 
}
/**************************************************************************************************
** �������� : BootLoaderHelp
** �������� : ��ʾBootLoader������ʾ
** ��ڲ��� : ��
** ���ڲ��� : ��
** �� �� ֵ : ��
** ����˵�� : ��
***************************************************************************************************/
void BootLoaderHelp(void)
{
   	(void)printf("\n\r");
	(void)printf("    1 : Load Hex File To Flash Of CRC\n\r");
	(void)printf("    2 : Load Hex File To Flash Of SUM\n\r");
	(void)printf("    3 : Start User Program \n\r");
	(void)printf("    4 : Erase Flash \n\r");
	(void)printf("    R : Reboot \n\r");
	//(void)printf("    S : Set Boot-delay Time \n\r");	 
	//(void)printf("    B : Cancel Loading \n\r");
	(void)printf("->");
}
/**************************************************************************************************
** �������� : CmdLoop
** �������� : ���ݴ��������������ִ������������
** ��ڲ��� : <cmd>[in] �������뷨��������
** ���ڲ��� : ��
** �� �� ֵ : ��
** ����˵�� : ��
***************************************************************************************************/
void CmdLoop(uint32_t cmd)
{
  //register uint32_t AT;
  //uint32_t	*P_RAM;
  //uint32_t	*p_Flash;
  //uint32_t	i=0,t=0;
  volatile  uint32_t ret=0;
  switch(cmd)
  {
    case 'H':
    case 'h':
      printf("H");
      BootLoaderHelp();
      break;
    case 0x0d://Enter��
      printf("\n\r->");
      break;
    case '1':
      printf("1");
      ret = xmodemReceive('C');
      break;
    case '2':
      printf("2");
      xmodemReceive(NAK);
      break;
    case '3':
      printf("3");
	  StartUserProgram(); 
   	  break;	 		    	
    case '4':
      printf("4");
	  EraseFlashInterface();
      break;
    case 'S':
    case 's':
      printf("S");
      //SetBootDelayTime();
      break;
    case 'R':
    case 'r':
		JumpToApplication(0);//�Զ������ת��������ת��ַΪ0X0000
		//Reset_Handler();//��������������ĸ�λ����
      break; 
    default: break;
  }
}
/**************************************************************************************************
** �������� : EraseFlash
** �������� : ����ѡ���Flas��
** ��ڲ��� : ��
** ���ڲ��� : ��
** �� �� ֵ : ��
** ����˵�� : ��
***************************************************************************************************/
void EraseFlashInterface(void)
{
	uint32_t	StartSector=0;
	uint32_t	EndSector=0;
	uint32_t	T=0;
	uint32_t	EndSectorFlasg=0;
	uint32_t	EraseEndFlag=0;
	uint32_t	HaveDataFlag=0;
	printf("\n\r->Please input start sector:");
	while(1){
		T=USART_GetChar();
		switch(T){
			case '0':case '1':case '2':case '3':case '4':
			case '5':case '6':case '7':case '8':case '9':
				if(EndSectorFlasg==0){
					USART_SendChar(T);
					StartSector=StartSector*10+T-'0';
					HaveDataFlag=1;
					if(StartSector>127){
						printf("\n\r->Error Sector!!");
						StartSector=8;
						HaveDataFlag=0;
						printf("\n\r->Please input start sector again:");
					}
				}
				if(EndSectorFlasg==1){
					USART_SendChar(T);
					EndSector=EndSector*10+T-'0';
					HaveDataFlag=2;
					if(EndSectorFlasg>127){
						printf("\n\r->Error Sector!!");
						EndSector=0;
						HaveDataFlag=0;
						printf("\n\r->Please input start sector again:");
					}
				}
				break;
			case 0x0D:
				if((EndSectorFlasg==0)&&(HaveDataFlag==1)){
					printf("\n\r->Please input end sector:");
					EndSectorFlasg=1;
				}
				if((EndSectorFlasg==1)&&(HaveDataFlag==2)){
					if(EndSector<StartSector){
						printf("\n\r->Start sector is small end sctor,please input again!!");
						printf("\n\r->Please input start sector again:");
						StartSector=8;
						EndSector=0;
						EndSectorFlasg=0;
					}else if(StartSector<8){
						printf("\n\r->Sctor 0 to 7 is bootloader zone!!");
						printf("\n\r->Please input start sector again:");	
						StartSector=8;
						EndSector=127;
						EndSectorFlasg=0;
					}else{
						printf("\n\r->Erasing Flash,Please Wait...");
						printf("\n\r->Erasing StartSector : %d\n\r->Erasing StartSector : %d",StartSector,EndSector);
						//PrepareSectors(StartSector,EndSector);
						ErasePage(StartSector,EndSector);
					    printf("\n\r->Erase Flash OK!");
					    printf("\n\r->");
						EraseEndFlag=1;
					}
				}
				break;
			case 'q':
			case 'Q':
				printf("\n\r->");
				USART_SendChar('Q');
				printf("\n\r->Quit of erase!!");
				printf("\n\r->");
				EraseEndFlag=1;
			default:
				break;
		}
		if(EraseEndFlag){
			break;
		}
	}
}
/**************************************************************************************************
** �������� : StartUserProgram
** �������� : �����û�����
** ��ڲ��� : ��
** ���ڲ��� : ��
** �� �� ֵ : ��
** ����˵�� : ���������û�����ǰ�����Ѿ�������д����ú������������������򣬷�����Ҫ�������������ַ
***************************************************************************************************/
void StartUserProgram(void)
{
	uint32_t	T=0;
	uint32_t	AddrTemp=0;
	uint32_t  HaveGetAddrFlag=0;
	uint32_t	CheckSum=0;
	uint32_t	i=0;
	uint32_t	*p_VectorValue;
	g_UserProgramAddr = 0;
	if(g_UserProgramAddr==0){
		printf("\n\r->Please input start address of hex:");
		while(1){
			T=USART_GetChar();
			switch(T){
				case '0':case '1':case '2':case '3':case '4':
				case '5':case '6':case '7':case '8':case '9':
					USART_SendChar(T);
					AddrTemp=(AddrTemp<<4)+(T-'0');
					break;			
				case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':
					USART_SendChar(T);
					AddrTemp=(AddrTemp<<4)+(T-'A');
					break;

				case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':
					USART_SendChar(T);
					AddrTemp=(AddrTemp<<4)+(T-'a');
					break;
				case 0x0D:
					HaveGetAddrFlag=1;
					p_VectorValue=(uint32_t *)AddrTemp;
					if(*p_VectorValue==0xFFFFFFFF){
						HaveGetAddrFlag=0;
						CheckSum=0;
						AddrTemp=0;
						printf("\n\r->The program is inefficacy!!");
						printf("\n\r->Please input start address of hex again:");
					}else{
						printf("\n\r->Jump to user program!!");
						JumpToApplication(AddrTemp);
					}
					break;
				case 'q':case 'Q':
					printf("\n\r->");
					USART_SendChar('Q');
					printf("\n\r->");
					HaveGetAddrFlag=1;
				default:
					break;
			}
			if(HaveGetAddrFlag){
				break;
			}
		}
	}else{
		p_VectorValue=(uint32_t *)g_UserProgramAddr;
		for(i=8;i!=0;i--){
			CheckSum+=(*(p_VectorValue++));
		}
		if(CheckSum!=0){
			printf("\n\r->The program is inefficacy!!");
			printf("\n\r->");
		}else{
			printf("\n\r->Jump to user program!!");
			JumpToApplication(g_UserProgramAddr);
		}
		//JumpToApplication(g_UserProgramAddr);//��ת���û�����
	}
}
