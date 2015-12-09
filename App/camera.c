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
#include "camera.h"


void img_extract(uint8 dist[][CAMERA_W], uint8 src[CAMERA_SIZE], uint32 srclen)
{
    uint8 colour[2] = {255, 0}; 
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


void vcan_sendimg(uint8 imgaddr[][CAMERA_W], uint32 height, uint32 width)
{
#define CMD_IMG     1
    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};
	uint32 index = 0;

    uart_putbuff(FIRE_PORT, cmdf, sizeof(cmdf)); 

	while (index != height) {
		uart_putbuff(FIRE_PORT, imgaddr[index], width);
		index++;
	}

    uart_putbuff(FIRE_PORT, cmdr, sizeof(cmdr));    
}

