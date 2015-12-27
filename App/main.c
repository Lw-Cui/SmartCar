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

#include "include.h"
#include "MKL_it.h"
#include "camera.h"
#include "mid_line.h"
#include "point.h"
#include "control.h"

/*!
 *  @brief      中断初始化
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

    tpm_pwm_init(TPM1,TPM_CH0,200, MID);//舵机，PTA12      
    
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置PORTA的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置DMA0的中断服务函数为 PORTA_IRQHandler

	EnableInterrupts;
}

/*!
 *  @brief      主函数，进行总体控制
 *  @since      v1.0
 */

#define LEN 255
void main() {       
	uint8 imgbuff[CAMERA_SIZE];
	camera_init(imgbuff);  
	init_interrupt();

	uint8 img[CAMERA_H][CAMERA_W];
	Point new_dir[LEN];

    while(1) {
		camera_get_img();
		img_extract(img, imgbuff,CAMERA_SIZE);
		adjustment(img, new_dir, direction(img, new_dir));
#ifdef _SEND_
		vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //发送解压后的图像数据
#endif
    }   
}
