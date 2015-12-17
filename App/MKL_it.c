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

/*!
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */

void PORTA_IRQHandler()
{
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
void DMA0_IRQHandler()
{
    camera_dma();
}


/*!
 *  @brief      ��ʱ�жϺ����� ������
 *  @since      v1.0
 */
uint16 encode_cnt;

void PIT_IRQHandler() {
	PIT_Flag_Clear(PIT0); 
	
	//������������ȡ
	encode_cnt = tpm_pulse_get(TPM2);
	
	//����
	tpm_pulse_clean(TPM2);
}


/*!
 *  @brief      ��ȡ����������
 *  @since      v1.0
 */
uint16 get_encode_cnt() {
	return encode_cnt;
}
