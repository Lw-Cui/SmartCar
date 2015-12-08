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
#include "mid_line.h"

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

//发送图像到上位机显示
//不同的上位机，不同的命令
//如果使用其他上位机，则需要修改代码
void vcan_sendimg(uint8 imgaddr[][CAMERA_W], uint32 height, uint32 width)
{
#define CMD_IMG     1
    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令
	uint32 index = 0;

    uart_putbuff(FIRE_PORT, cmdf, sizeof(cmdf));    //先发送命令

	while (index != height) {
		uart_putbuff(FIRE_PORT, imgaddr[index], width); //再发送图像
		index++;
	}

    uart_putbuff(FIRE_PORT, cmdr, sizeof(cmdr));    //先发送命令
}



/*!
 *  @brief      二值化图像解压（空间 换 时间 解压）
 *  @param      dist            图像解压目的地址
 *  @param      src             图像解压源地址
 *  @param      srclen          二值化图像的占用空间大小
 *  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
 *  Sample usage:
 */
void img_extract(uint8 dist[][CAMERA_W], uint8 src[CAMERA_SIZE], uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    //注：山外的摄像头 0 表示 白色，1表示 黑色
    uint8 tmpsrc;
	uint8 line = 0;
	uint8 *dst = dist[line];

    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

/*!
 *  @brief      遍历最近一行，找到边界
 *  @param      img				图像数组
 *  @param      left_prev, right_prev		记录边界的数组
 *  @since      v1.0
 */
void traversal(uint8 img[][CAMERA_W], Point prev[][CAMERA_W]) {
	Point left_end = {EMPTY, EMPTY}, right_end = {EMPTY, EMPTY};
	Point pos = {CAMERA_H - 1, 0};
	Point end;

	while (pos.y < CAMERA_W) {
		end = processing(img, pos, prev);
		if (!empty(end) && gray_boundary(img, prev, end))
			;
		pos.y++;
	}

}

uint8 imgbuff[CAMERA_SIZE];
uint8 is_captured = 0;

/*
void PIT_IRQHandler() {
	PIT_Flag_Clear(PIT0); 
	if (is_captured) {
		DisableInterrupts;
		is_captured = 0;

		uint8 img[CAMERA_H][CAMERA_W];
		Point prev[CAMERA_H][CAMERA_W];

		for (int i = 0; i < CAMERA_H; i++)
			for (int j = 0; j < CAMERA_H; j++)
				set(&prev[i][j], EMPTY, EMPTY);

		img_extract(img, imgbuff,CAMERA_SIZE);                //鹰眼所采集的图像为一字节8个像素点，将其解压变为一个字节1个像素点，便于上位机处

		vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //发送解压后的图像数据
		vertical_line(img);
		traversal(img, prev);

		vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //发送解压后的图像数据
		EnableInterrupts;
	}
}
*/
