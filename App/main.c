/*!
 *     COPYRIGHT NOTICE
 *     Copyright_prev (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       main.c
 * @brief      野火KL26 平台主程序
 * @author     野火科技
 * @version    v5.0
 * @date       2013-12-14
 */

/*
 * include.h 中需要注释掉 MKL_BME.h
 * 32% - 41% - 50% int 舵机占空比
 *
 */
#include "include.h"
#include "MKL_it.h"
#include "camera.h"
#include "mid_line.h"
#include "point.h"
#include "control.h"

void processing(uint8 imgbuff[CAMERA_SIZE]) {
	DisableInterrupts;
	uint8 img[CAMERA_H][CAMERA_W];
#define LEN 255
	Point new_dir[LEN];

	img_extract(img, imgbuff,CAMERA_SIZE);                //鹰眼所采集的图像为一字节8个像素点，将其解压变为一个字节1个像素点，便于上位机处

	int len;
	if (len = traversal(img, new_dir))
		direction(new_dir, len);

#ifdef _DEBUG_
	vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //发送解压后的图像数据
#endif

	EnableInterrupts;
}


void init_interrupt() {
	DisableInterrupts;

    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置PORTA的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置DMA0的中断服务函数为 PORTA_IRQHandler

    pit_init_ms(PIT0, 5);
    set_vector_handler(PIT_VECTORn, PIT_IRQHandler);
    enable_irq(PIT_IRQn);

 /*注意：电机和舵机必须为不同的TPM模块！！！！*****/
    tpm_pwm_init(TPM0,TPM_CH0,20000,0);//初始化舵机，SD5频率为50Hz，S3010频率为300Hz，MID宏定义，为舵机占空比中值，不同的舵机中值不一样，自己调
    tpm_pwm_init(TPM0,TPM_CH1,20000,0);
    
    tpm_pwm_init(TPM1,TPM_CH1, 50, 0);//舵机
    tpm_pulse_init(TPM2,TPM_CLKIN0,TPM_PS_1);//B16右电机

	EnableInterrupts;
}

void  main(void)
{
	uint8 imgbuff[CAMERA_SIZE];
    camera_init(imgbuff);

	init_interrupt();

    while(1)
    {
		camera_get_img();                                     //在while(1)中不断使能PORTA，使得摄像头采集图像的信号到来后，就可以触发PORTA
		extern volatile IMG_STATUS_e ov7725_eagle_img_flag;
		if (ov7725_eagle_img_flag == IMG_FINISH)
			processing(imgbuff);
    }   
}
