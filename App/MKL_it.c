/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       MK60_it.c
 * @brief      野火K60 平台中断复位函数
 * @author     野火科技
 * @version    v5.0
 * @date       2013-06-26
 */

#include "MKL_it.h"
#include "include.h"

/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */

void PORTA_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 6;                                              //场中断
    if(flag & (1 << n))                                 //PTA6触发中断
    {
        camera_vsync();
    }
}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}


/*!
 *  @brief      定时中断函数， 编码器
 *  @since      v1.0
 */
uint16 encode_cnt;

void PIT_IRQHandler() {
	PIT_Flag_Clear(PIT0); 
	
	//编码器计数获取
	encode_cnt = tpm_pulse_get(TPM2);
	
	//清零
	tpm_pulse_clean(TPM2);
}


/*!
 *  @brief      获取编码器计数
 *  @since      v1.0
 */
uint16 get_encode_cnt() {
	return encode_cnt;
}
