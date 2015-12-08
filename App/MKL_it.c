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
#include "mid_line.h"

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

//����ͼ����λ����ʾ
//��ͬ����λ������ͬ������
//���ʹ��������λ��������Ҫ�޸Ĵ���
void vcan_sendimg(uint8 imgaddr[][CAMERA_W], uint32 height, uint32 width)
{
#define CMD_IMG     1
    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����
	uint32 index = 0;

    uart_putbuff(FIRE_PORT, cmdf, sizeof(cmdf));    //�ȷ�������

	while (index != height) {
		uart_putbuff(FIRE_PORT, imgaddr[index], width); //�ٷ���ͼ��
		index++;
	}

    uart_putbuff(FIRE_PORT, cmdr, sizeof(cmdr));    //�ȷ�������
}



/*!
 *  @brief      ��ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
 *  @param      dist            ͼ���ѹĿ�ĵ�ַ
 *  @param      src             ͼ���ѹԴ��ַ
 *  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
 *  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
 *  Sample usage:
 */
void img_extract(uint8 dist[][CAMERA_W], uint8 src[CAMERA_SIZE], uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 �� 1 �ֱ��Ӧ����ɫ
    //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
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
 *  @brief      �������һ�У��ҵ��߽�
 *  @param      img				ͼ������
 *  @param      left_prev, right_prev		��¼�߽������
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

		img_extract(img, imgbuff,CAMERA_SIZE);                //ӥ�����ɼ���ͼ��Ϊһ�ֽ�8�����ص㣬�����ѹ��Ϊһ���ֽ�1�����ص㣬������λ����

		vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //���ͽ�ѹ���ͼ������
		vertical_line(img);
		traversal(img, prev);

		vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //���ͽ�ѹ���ͼ������
		EnableInterrupts;
	}
}
*/
