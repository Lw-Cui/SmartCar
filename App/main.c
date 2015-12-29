/*!
 *     COPYRIGHT NOTICE
 *     Copyright_prev (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      Ұ��KL26 ƽ̨������
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-12-14
 */

#include "include.h"
#include "MKL_it.h"
#include "camera.h"
#include "mid_line.h"
#include "point.h"
#include "control.h"

/*!
 *  @brief      �жϳ�ʼ��
 *  @since      v1.0
 *  @cheatsheet

    pit_init_ms(PIT0, 25);
    set_vector_handler(PIT_VECTORn, PIT_IRQHandler);
    enable_irq(PIT_IRQn);
 */
void init_interrupt() {
	DisableInterrupts; 
	
    tpm_pwm_init(TPM0,TPM_CH0,20000, 0);
    tpm_pwm_init(TPM0,TPM_CH1,20000, 0);

    tpm_pwm_init(TPM1,TPM_CH0,200, MID);//�����PTA12      
    
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //����PORTA���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //����DMA0���жϷ�����Ϊ PORTA_IRQHandler

	/*
    pit_init_ms(PIT0, 20);
    set_vector_handler(PIT_VECTORn, PIT_IRQHandler);
    enable_irq(PIT_IRQn);
	*/

	EnableInterrupts;
}

/*!
 *  @brief      �������������������
 *  @since      v1.0
 */

#define LEN 255
int offset, velocity;

void main() {       
	uint8 imgbuff[CAMERA_SIZE];
	camera_init(imgbuff);  
	init_interrupt();

	uint8 img[CAMERA_H][CAMERA_W];
	Point new_dir[LEN];

    while(1) {
		camera_get_img();
		img_extract(img, imgbuff,CAMERA_SIZE);
		offset = get_offset(img, new_dir, direction(img, new_dir));
		velocity = get_velocity();

#ifdef _SEND_
		vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //���ͽ�ѹ���ͼ������
#endif
    }   
}
