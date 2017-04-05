//*================================================================================================
//* �ļ���				: timer.c
//* ��������			: ��ʱ����غ���
//* ����    			: DSF
//* �汾				: 0.01
//* �������ڡ�ʱ��		: 2017-3-29 09:53:10
//* ����޸����ڡ�ʱ��	: 
//* �޸�ԭ��			: 
//*================================================================================================
/*------------------------------------------ ͷ�ļ� ---------------------------------------------*/
#include "stm32f10x_conf.h"
#include "timer.h"
#include "xmodem.h"
/*-----------------------------------------------------------------------------------------------*/

/*----------------------------------------- ȫ�ֱ��� --------------------------------------------*/
extern uint32_t	g_RequestTimes;	//�����ļ��������
extern uint32_t	g_CheckType;
extern uint8_t	g_recTimeOutFlag;
extern uint32_t	g_starTimeTemp;
uint8_t	flag=0;
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------- �Զ����������� -----------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/

//*================================================================================================
//*											��	��	��
//*================================================================================================
/**
  * @brief  ��ʱ��3�жϴ��������ڸú����н����ڽ���״̬�����ڳ�ʼ״̬
  * @note   None
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
   //����Ƿ�����������¼�
   if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		//���TIM2���жϴ�����λ
	    TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
		g_recTimeOutFlag = TIME_OUT;
		if(g_starTimeTemp>0)
		{
		  g_starTimeTemp--;//��ʱ������Ϊ100ms
		}
	}
}

/**
  * @brief  ��ʼ��USART�������ݶ�ʱ���ж�
  * @param  None
  * @retval None
  * @note  	None
  */
void USART1_Timer3_Interrupts_Config(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  ��ʼ��USART1���ն�ʱ��,��ֹUSART�������ݳ�ʱ 
  * @param  None
  * @retval None
  * @note  	�ö�ʱ�����Է�ֹ��֡���ݽ��մ����ʱ�������״̬�����ִ���״̬
  */
void USART1_Timer3_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

    TIM_DeInit(TIM3);                              					//��λTIM2��ʱ��
        
    /* TIM3 configuration */
    TIM_TimeBaseStructure.TIM_Period = 100;        					// 100ms    
    TIM_TimeBaseStructure.TIM_Prescaler = 36000;    				// ��Ƶ36000      
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  		// ʱ�ӷ�Ƶ 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//�����������ϼ���
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Clear TIM3 update pending flag[���TIM3����жϱ�־] */
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    /* Enable TIM3 Update interrupt [TIM3����ж�����]*/
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
	/* TIM3����������*/
    TIM3->CNT=0;
	/* TIM3 enable counter [����TIM3����]*/
    TIM_Cmd(TIM3, DISABLE);  

	//��ʼ�����ڽ��ճ�ʱ��ʱ���ж�
  	USART1_Timer3_Interrupts_Config();
}
/**
  * @brief  �����ʱ���������Ĵ�������ֵ��������ʱ��
  * @param  None
  * @retval None
  * @note  	None
  */
void USART1_Timer3_Start(uint32_t time)
{
	TIM3->CNT=0;//����������Ĵ�����ֵ�����Լ�С��֡�����
	TIM3->ARR=time;
    /* Enable the TIM Counter */
	//TIM_Cmd(TIM3, ENABLE); 
    TIM3->CR1 |= ((uint16_t)0x0001);
}

/**
  * @brief  ֹͣ��ʱ���������ʱ���ļ���ֵ 
  * @param  None
  * @retval None
  * @note  	None
  */
void USART1_Timer3_Stop(void)
{ 
    /* Disable the TIM Counter */
	//TIM_Cmd(TIM3, DISABLE);
	TIM3->CR1 &= ((uint16_t)0x03FE);
	TIM3->CNT=0;
}
