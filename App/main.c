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


extern volatile IMG_STATUS_e ov7725_eagle_img_flag;

void highlight(uint8 imgbuff[CAMERA_SIZE]) {
	DisableInterrupts;

	uint8 img[CAMERA_H][CAMERA_W];


	img_extract(img, imgbuff,CAMERA_SIZE);                //鹰眼所采集的图像为一字节8个像素点，将其解压变为一个字节1个像素点，便于上位机处

	//vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //发送解压后的图像数据
	traversal(img);
	//vertical_line(img);

	vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //发送解压后的图像数据
	EnableInterrupts;
}

void  main(void)
{
	DisableInterrupts;

	uint8 imgbuff[CAMERA_SIZE];
    camera_init(imgbuff);
    
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置PORTA的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置DMA0的中断服务函数为 PORTA_IRQHandler

	/*
    pit_init_ms(PIT0,20);
    set_vector_handler(PIT_VECTORn, PIT_IRQHandler);
    enable_irq(PIT_IRQn);
	*/
	EnableInterrupts;

    while(1)
    {
      camera_get_img();                                     //在while(1)中不断使能PORTA，使得摄像头采集图像的信号到来后，就可以触发PORTA
		if (ov7725_eagle_img_flag == IMG_FINISH)
			highlight(imgbuff);
    }   
}

