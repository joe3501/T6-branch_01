/**
 * @file  Timebase.c
 * @brief use TIM1 to generate a time base 
 * @version 1.0
 * @author joe
 * @date 2009年09月10日
 * @note
*/
#include "stm32f10x_lib.h" 

#include "TimeBase.h"

static int TimingDelay;

/**
 * @brief     软延时
 * @param[in] u16 delay 延时参数 实际延时时间等于delay/2 us.
 * @param[out] none
 * @return none
 * @note 开始想利用Timer来做延时，但由于需要的时基比较小，只有0.5us，所以timer的周期设定值就很小，
 *       而ucosII在进出中断的开销太大，导致程序根本就无法退出timer的中断程序；
 *       后来又想通过查询Timer的Flag来实现时基，但是发现Timer的UpdateFlag竟然一直有效，没得时间慢慢去找
 *       原因了，干脆就用指令来实现延时得了！
*/
void Delay(unsigned int delay)
{
	do{
		;
	}while(delay--);
}


/**
 * @brief     初始化产生延时时基的计数器TIM2,设定计数器产生1ms的时基
 * @param[in] none
 * @param[out] none
 * @return none
 * @note   此初始化函数中调用了BSP_IntVectSet(BSP_INT_ID_TIM2, TIM2_UpdateISRHandler)这个函数，这个是设定TIM2的中断处理函数的
 *				 在移植的时候需要根据不同工程中设置中断处理函数的方法来稍作修改。       
*/
void TimeBase_Init(void)
{
	TIM_TimeBaseInitTypeDef						TIM_TimeBaseStructure;
	TIM_OCInitTypeDef							TIM_OCInitStructure;
	NVIC_InitTypeDef							NVIC_InitStructure;

	//初始化结构体变量
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);

	/*开启相应时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  


	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler			= 0;      //72M的计数频率
	TIM_TimeBaseStructure.TIM_CounterMode		= TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseStructure.TIM_Period			= (72000/2-1);      //500us的定时
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
 * @brief 开始利用TIM2定时计数
 * @param[in] unsigned int nTime*5 等于要延时的毫秒数， nTime = 10 产生50ms的延时，nTime  = 200时，产生1S的延时（此时基是经过测试的）
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
 * @brief 判断延时时间是否到
 * @param[in] none
 * @return 0: 定时到
 *        1: 定时未到
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
 * @brief TIM2的溢出中断ISR
 * @param[in] none
 * @return none
 * @note  TIM2的中断服务函数调用
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
