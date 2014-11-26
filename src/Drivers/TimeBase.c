/**
 * @file  Timebase.c
 * @brief use TIM1 to generate a time base 
 * @version 1.0
 * @author joe
 * @date 2009��09��10��
 * @note
*/
#include "stm32f10x_lib.h" 

#include "TimeBase.h"

static int TimingDelay;

/**
 * @brief     ����ʱ
 * @param[in] u16 delay ��ʱ���� ʵ����ʱʱ�����delay/2 us.
 * @param[out] none
 * @return none
 * @note ��ʼ������Timer������ʱ����������Ҫ��ʱ���Ƚ�С��ֻ��0.5us������timer�������趨ֵ�ͺ�С��
 *       ��ucosII�ڽ����жϵĿ���̫�󣬵��³���������޷��˳�timer���жϳ���
 *       ��������ͨ����ѯTimer��Flag��ʵ��ʱ�������Ƿ���Timer��UpdateFlag��Ȼһֱ��Ч��û��ʱ������ȥ��
 *       ԭ���ˣ��ɴ����ָ����ʵ����ʱ���ˣ�
*/
void Delay(unsigned int delay)
{
	do{
		;
	}while(delay--);
}


/**
 * @brief     ��ʼ��������ʱʱ���ļ�����TIM2,�趨����������1ms��ʱ��
 * @param[in] none
 * @param[out] none
 * @return none
 * @note   �˳�ʼ�������е�����BSP_IntVectSet(BSP_INT_ID_TIM2, TIM2_UpdateISRHandler)���������������趨TIM2���жϴ���������
 *				 ����ֲ��ʱ����Ҫ���ݲ�ͬ�����������жϴ��������ķ����������޸ġ�       
*/
void TimeBase_Init(void)
{
	TIM_TimeBaseInitTypeDef						TIM_TimeBaseStructure;
	TIM_OCInitTypeDef							TIM_OCInitStructure;
	NVIC_InitTypeDef							NVIC_InitStructure;

	//��ʼ���ṹ�����
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);

	/*������Ӧʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  


	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler			= 0;      //72M�ļ���Ƶ��
	TIM_TimeBaseStructure.TIM_CounterMode		= TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseStructure.TIM_Period			= (72000/2-1);      //500us�Ķ�ʱ
	TIM_TimeBaseStructure.TIM_ClockDivision		= 0x0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Channel 1, 2, 3 and 4 Configuration in Timing mode */
	TIM_OCInitStructure.TIM_OCMode				= TIM_OCMode_Timing;
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse				= 0x0;

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	/* Enable the TIM2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel			= TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority	= 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd		= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief ��ʼ����TIM2��ʱ����
 * @param[in] unsigned int nTime*5 ����Ҫ��ʱ�ĺ������� nTime = 10 ����50ms����ʱ��nTime  = 200ʱ������1S����ʱ����ʱ���Ǿ������Եģ�
 * @return  none
*/
void StartDelay(unsigned short nTime)
{
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    //BSP_IntEn(BSP_INT_ID_TIM2);
    /* TIM counter enable */
    TIM_Cmd(TIM2, ENABLE);
    TimingDelay = nTime*10;
}


/**
 * @brief �ж���ʱʱ���Ƿ�
 * @param[in] none
 * @return 0: ��ʱ��
 *        1: ��ʱδ��
*/
unsigned char DelayIsEnd(void)
{
	if(TimingDelay>0)
		return 1;
	else
		return 0;
//	if(TimingDelay == 0)
//	{
//		return 0;
//	}
//	else
//		return 1; 
}

/**
 * @brief TIM2������ж�ISR
 * @param[in] none
 * @return none
 * @note  TIM2���жϷ���������
*/
void TIM2_UpdateISRHandler(void)
{    
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		if(TimingDelay != 0)
		{
			TimingDelay --;
		}  
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}