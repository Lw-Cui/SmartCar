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
 *  @brief      ������
 *  @param      imgbuff		ͼ�񻺳���
 *  @since      v1.0
 */

void processing(uint8 imgbuff[]) {
	DisableInterrupts;
	uint8 img[CAMERA_H][CAMERA_W];
#define LEN 255
	Point new_dir[LEN];

	img_extract(img, imgbuff,CAMERA_SIZE);                //ӥ�����ɼ���ͼ��Ϊһ�ֽ�8�����ص㣬�����ѹ��Ϊһ���ֽ�1�����ص㣬������λ����

	int black_cnt = 0;
	for (int i = 0; i < CAMERA_H; i++)
		for (int j = 0; j < CAMERA_W; j++)
			if (img[i][j] == 0)
				black_cnt++;
	while (black_cnt > CAMERA_H * CAMERA_W * 0.9) {
		tpm_pwm_duty(TPM1,TPM_CH0, MID);

		tpm_pwm_duty(TPM0,TPM_CH0, 0);
		tpm_pwm_duty(TPM0,TPM_CH1, 0);
	}
        
	int len;
	if (len = traversal(img, new_dir))
		direction(new_dir, len);
#ifdef _SEND_
	vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //���ͽ�ѹ���ͼ������
#endif

	EnableInterrupts;
}


void init_interrupt() {
	DisableInterrupts; 
	
    tpm_pwm_init(TPM0,TPM_CH0,20000,0);//��ʼ�������SD5Ƶ��Ϊ50Hz��S3010Ƶ��Ϊ300Hz��MID�궨�壬Ϊ���ռ�ձ���ֵ����ͬ�Ķ����ֵ��һ�����Լ���
    tpm_pwm_init(TPM0,TPM_CH1,20000,0);
    tpm_pwm_init(TPM1,TPM_CH0,200,MID);//�����PTA12      

    
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //����PORTA���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //����DMA0���жϷ�����Ϊ PORTA_IRQHandler

	/*
    pit_init_ms(PIT0, 25);
    set_vector_handler(PIT_VECTORn, PIT_IRQHandler);
    enable_irq(PIT_IRQn);
	*/

	EnableInterrupts;
}

void  main(void)
{       
	uint8 imgbuff[CAMERA_SIZE];
	camera_init(imgbuff);  

	init_interrupt();
    while(1)
    {
		camera_get_img();                                     //��while(1)�в���ʹ��PORTA��ʹ������ͷ�ɼ�ͼ����źŵ����󣬾Ϳ��Դ���PORTA
		processing(imgbuff);
    }   
}
