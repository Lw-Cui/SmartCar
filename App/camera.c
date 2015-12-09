/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ò°»ð¿Æ¼¼
 *     All rights reserved.
 *     ¼¼ÊõÌÖÂÛ£ºÒ°»ð³õÑ§ÂÛÌ³ http://www.chuxue123.com
 *
 *     ³ý×¢Ã÷³ö´¦Íâ£¬ÒÔÏÂËùÓÐÄÚÈÝ°æÈ¨¾ùÊôÒ°»ð¿Æ¼¼ËùÓÐ£¬Î´¾­ÔÊÐí£¬²»µÃÓÃÓÚÉÌÒµÓÃÍ¾£¬
 *     ÐÞ¸ÄÄÚÈÝÊ±±ØÐë±£ÁôÒ°»ð¿Æ¼¼µÄ°æÈ¨ÉùÃ÷¡£
 *
 * @file       MK60_it.c
 * @brief      Ò°»ðK60 Æ½Ì¨ÖÐ¶Ï¸´Î»º¯Êý
 * @author     Ò°»ð¿Æ¼¼
 * @version    v5.0
 * @date       2013-06-26
 */


/*!
 *  @brief      ¶þÖµ»¯Í¼Ïñ½âÑ¹£¨¿Õ¼ä »» Ê±¼ä ½âÑ¹£©
 *  @param      dist            Í¼Ïñ½âÑ¹Ä¿µÄµØÖ·
 *  @param      src             Í¼Ïñ½âÑ¹Ô´µØÖ·
 *  @param      srclen          ¶þÖµ»¯Í¼ÏñµÄÕ¼ÓÃ¿Õ¼ä´óÐ¡
 *  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
 *  Sample usage:
 */
void img_extract(uint8 dist[][CAMERA_W], uint8 src[CAMERA_SIZE], uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 ºÍ 1 ·Ö±ð¶ÔÓ¦µÄÑÕÉ«
    //×¢£ºÉ½ÍâµÄÉãÏñÍ· 0 ±íÊ¾ °×É«£¬1±íÊ¾ ºÚÉ«
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


//·¢ËÍÍ¼Ïñµ½ÉÏÎ»»úÏÔÊ¾
//²»Í¬µÄÉÏÎ»»ú£¬²»Í¬µÄÃüÁî
//Èç¹ûÊ¹ÓÃÆäËûÉÏÎ»»ú£¬ÔòÐèÒªÐÞ¸Ä´úÂë
void vcan_sendimg(uint8 imgaddr[][CAMERA_W], uint32 height, uint32 width)
{
#define CMD_IMG     1
    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //É½ÍâÉÏÎ»»ú Ê¹ÓÃµÄÃüÁî
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //É½ÍâÉÏÎ»»ú Ê¹ÓÃµÄÃüÁî
	uint32 index = 0;

    uart_putbuff(FIRE_PORT, cmdf, sizeof(cmdf));    //ÏÈ·¢ËÍÃüÁî

	while (index != height) {
		uart_putbuff(FIRE_PORT, imgaddr[index], width); //ÔÙ·¢ËÍÍ¼Ïñ
		index++;
	}

    uart_putbuff(FIRE_PORT, cmdr, sizeof(cmdr));    //ÏÈ·¢ËÍÃüÁî
}

