//*================================================================================================
//* �ļ���				: ProgramFlash.c
//* ��������			: ���������Hex������д��Flash�е�Ӧ�ò㺯��
//* ����    			: ��ΰ��
//* �汾				: 0.01
//* �������ڡ�ʱ��		: 2011/04/15 20:33
//* ����޸����ڡ�ʱ��	: 
//* �޸�ԭ��			: 
//*================================================================================================
/*------------------------------------------ ͷ�ļ� ---------------------------------------------*/
#include	"decodeHex.h"
#include	"ProgramFlash.h"
#include	"interface.h"
#include	"stm32_iap.h"
/*-----------------------------------------------------------------------------------------------*/

/*----------------------------------------- ȫ�ֱ��� --------------------------------------------*/
extern uint8_t	g_ProgramFlashDataTemp[BUFFER_SIZE];
uint32_t	g_AddrBase=0x800;//hex���ݴ洢��ʼ��ַ
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------- �Զ����������� -----------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/

//*================================================================================================
//*											��	��	��
//*================================================================================================

/**************************************************************************************************
** �������� : ProgramHexToFlash
** �������� : ��һ��HEX�ļ�������д��Flash��
** ��ڲ��� : <p_HexData>[in] ׼����д��һ��HEX�ļ������׵�ַ
**			: <UseDefaultAddr>[in] �Ƿ�ʹ��HEX�ļ������е�Ĭ�ϵ�ַ
**								   TRUE:ʹ��Ĭ�ϵ�ַ	FALSE:ʹ��UserAddrָ���ĵ�ַ
**			: <UserAddr>[in] ָ����Flash��ַ
**			: <HexDataTemp>[in] �ݴ�����Flash�е����ݻ������׵�ַ
** ���ڲ��� : ��
** �� �� ֵ : ����д�ɹ��򷵻�0
** ����˵�� : UserAddr��ַ������0X100����������HexDataTemp�����ȫ�ֱ��������׵�ַ
***************************************************************************************************/
uint8_t ProgramHexToFlash(uint8_t *p_HexData,uint32_t AddrSelect,volatile uint32_t UserAddr)
{
	volatile uint8_t 	ErrorCode=0;//��Ҫ���صĴ������
	t_HexLineInfo 	HexDataInfo;//��ʱ�洢HEX���ݵ������Ϣ
	uint8_t	CanGetData=TRUE;//�ж������Ƿ�ȫ���������Ѿ����뻺���־
	uint32_t	HexData_Index_Temp=0;
	uint32_t	HexDataTemp_Index_Temp=0;
	uint32_t	i=0;
	uint32_t	DST=0;
	volatile uint32_t	FirstAddr=0;
	volatile uint8_t	HaveGetAddrFlag=FALSE;
	volatile uint32_t	Sector=0;
	for(i=0;i<BUFFER_SIZE;i++){		//��ʼ����Ҫд��Flash�����ݻ�����
		g_ProgramFlashDataTemp[i]=0xFF;	
	}
	/*��p_HexData��ָ��Ĵ洢���е�HEX���ݽ�����g_ProgramFlashDataTemp��ָ��Ļ�������*/
	while(CanGetData==TRUE){
		ErrorCode=GetHexLineInfo(p_HexData+HexData_Index_Temp,&HexDataInfo);//��ȡHEX������Ϣ
		if((HaveGetAddrFlag==FALSE)&&(HexDataInfo.Rectype==RCD_DATA)){
			FirstAddr=HexDataInfo.OffsetAddr;
			HaveGetAddrFlag=TRUE;
		}
		HexData_Index_Temp+=((HexDataInfo.DataLength)*2+13);
		if(ErrorCode==RIGHT){
			if(HexDataInfo.Rectype==RCD_DATA){
				ErrorCode=GetHexData(&HexDataInfo,g_ProgramFlashDataTemp+HexDataTemp_Index_Temp);//��HEX���������������HexDataTemp���ݻ�����
				HexDataTemp_Index_Temp+=HexDataInfo.DataLength;
			}else if(HexDataInfo.Rectype==RCD_END_FILE){//���յ��ļ���β����ѭ��
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
		if(p_HexData[HexData_Index_Temp]!=':'){//�жϸôν��յ������Ƿ������
			CanGetData=FALSE;
		}
	}
	/*���仺��������д��Flash��*/
	if((ErrorCode==RIGHT)&&(AddrSelect==USE_USER_DEFINED_ADDR)){//������ȷ������ָ����ַ��д����
		DST=(g_AddrBase<<16)+(FirstAddr+UserAddr);	
		ErrorCode = ProgramDatatoFlash(&DST,(uint16_t*)g_ProgramFlashDataTemp,BUFFER_SIZE);
	}else if((ErrorCode==RIGHT)&&(AddrSelect==USE_DFT_ADDR)){//������ȷ������HEX�ļ�������ĵ�ַ��д����
		DST=(g_AddrBase<<16)+FirstAddr;
		ErrorCode = ProgramDatatoFlash(&DST,(uint16_t*)g_ProgramFlashDataTemp,BUFFER_SIZE);	
	}else{
		ErrorCode=1;
	}
	return ErrorCode;		
}
/**************************************************************************************************
** �������� : EraseFlash
** �������� : ����ѡ�е�Flash��������
** ��ڲ��� : <StartSector>[in] ��Ҫ��������ʼ����
**			: <EndSector>[in] ��Ҫ��������ֹ����
** ���ڲ��� : ��
** �� �� ֵ : �������ɹ��򷵻�0
** ����˵�� : EndSector������ڵ���StartSector
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
** �������� : DetectSector
** �������� : ��������ĵ�ַ��Ϣ�жϸõ�ַ�����Ǹ�����
** ��ڲ��� : <Address>[in] ��Ҫ�жϵĵ�ַ
** ���ڲ��� : ��
** �� �� ֵ : ��ַ��Ӧ������ֵ
** ����˵�� : ��
***************************************************************************************************/
uint8_t DetectSector(uint32_t Address)
{
	return (Address - 0x8000000)/PAGE_SIZE;
}

