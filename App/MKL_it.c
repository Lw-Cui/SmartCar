/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.c
 * @brief      Ұ��K60 ƽ̨�жϸ�λ����
 * @author     Ұ��Ƽ�
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
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */

void PORTA_IRQHandler() {
    uint8  n = 0;    //���ź�
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 6;                                              //���ж�
    if(flag & (1 << n))                                 //PTA6�����ж�
    {
        camera_vsync();
    }
}

/*!
 *  @brief      DMA0�жϷ�����
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
 *  @brief      ��ʱ�жϺ����� ������
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

	//�������ݼĴ�����
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   {
        //�û���Ҫ�����������
    }

	//�������ݼĴ�����
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK)  {
        //�û���Ҫ����������
    }
}
