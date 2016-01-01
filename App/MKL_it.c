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
#include "include.h"
#include "MKL_it.h"
#include "camera.h"
#include "mid_line.h"
#include "point.h"
#include "control.h"

/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */

void PORTA_IRQHandler() {
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
void DMA0_IRQHandler() {
    camera_dma();
}


int max(int c1, int c2) {
	return c1 > c2? c1: c2;
}

int min(int c1, int c2) {
	return c1 < c2? c1: c2;
}

int protect(int offset) {
	if (offset > 0)
		return min(MAX_OFFSET, offset);
	else
		return max(-MAX_OFFSET, offset);
}


/*!
 *  @brief      定时中断函数， 编码器
 *  @since      v1.0
	encode_cnt = tpm_pulse_get(TPM2);
	
	tpm_pulse_clean(TPM2);
 */
#define KD 0.5
extern int offset, velocity;
void PIT_IRQHandler() {
	PIT_Flag_Clear(PIT0); 

	static int last_offset = 0;
	if (last_offset == 0)
		last_offset = offset;

	offset += (offset - last_offset) * KD;
	last_offset = offset;

	tpm_pwm_duty(TPM1,TPM_CH0, MID + protect(offset));

	tpm_pwm_duty(TPM0,TPM_CH1,0);
#ifdef _MOTO_
	tpm_pwm_duty(TPM0,TPM_CH0, velocity);
#else
	tpm_pwm_duty(TPM0,TPM_CH0, 0);
#endif
}

void uart_handler() {
    UARTn_e uratn = FIRE_PORT;

	//接收数据寄存器满
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   {
        //用户需要处理接收数据
    }

	//发送数据寄存器空
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK)  {
        //用户需要处理发送数据
    }
}
