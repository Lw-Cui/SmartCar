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

/*
 * include.h ����Ҫע�͵� MKL_BME.h
 *
 */
#include "include.h"
#include "MKL_it.h"
#include "camera.h"
#include "mid_line.h"


extern volatile IMG_STATUS_e ov7725_eagle_img_flag;

void highlight() {
	DisableInterrupts;

	uint8 img[CAMERA_H][CAMERA_W];
	Point prev[CAMERA_H][CAMERA_W];

	for (int i = 0; i < CAMERA_H; i++)
		for (int j = 0; j < CAMERA_H; j++)
			set(&prev[i][j], EMPTY, EMPTY);

	img_extract(img, imgbuff,CAMERA_SIZE);                //ӥ�����ɼ���ͼ��Ϊһ�ֽ�8�����ص㣬�����ѹ��Ϊһ���ֽ�1�����ص㣬������λ����

	vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //���ͽ�ѹ���ͼ������
	traversal(img, prev);
	vertical_line(img);

	vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //���ͽ�ѹ���ͼ������
	EnableInterrupts;
}

void  main(void)
{
	DisableInterrupts;

	uint8 imgbuff[CAMERA_SIZE];
    camera_init(imgbuff);
    
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //����PORTA���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //����DMA0���жϷ�����Ϊ PORTA_IRQHandler

	/*
    pit_init_ms(PIT0,20);
    set_vector_handler(PIT_VECTORn, PIT_IRQHandler);
    enable_irq(PIT_IRQn);
	*/
	EnableInterrupts;

    while(1)
    {
      camera_get_img();                                     //��while(1)�в���ʹ��PORTA��ʹ������ͷ�ɼ�ͼ����źŵ����󣬾Ϳ��Դ���PORTA
		if (ov7725_eagle_img_flag == IMG_FINISH)
			highlight();
    }   
}

