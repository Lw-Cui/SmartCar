#ifndef __INCLUDE_H__
#define __INCLUDE_H__

/*
 * 定义宏代表打开相应功能
 */
#define _MOTO_
#define _SEND_

/*
 * Include 用户自定义的头文件
 */
#include  "common.h"
//#include  "MKL_BME.h"           //位操作
#include  "MKL_wdog.h"          //看门狗
#include  "MKL_gpio.h"          //IO口操作
#include  "MKL_uart.h"          //串口
#include  "MKL_SysTick.h"       //滴答定时器
#include  "MKL_lptmr.h"         //低功耗定时器(延时、脉冲计数、定时、计时)
#include  "MKL_i2c.h"           //I2C
#include  "MKL_spi.h"           //SPI
#include  "MKL_tpm.h"           //TPM（类似K60的 FTM ，pwm、脉冲计数）
#include  "MKL_pit.h"           //PIT
#include  "MKL_adc.h"           //ADC
#include  "MKL_dac.h"           //DAC
#include  "MKL_dma.h"           //DMA
#include  "MKL_FLASH.h"         //FLASH
#include "MKL_port.h"           //PORT


#include  "FIRE_LED.H"          //LED
#include  "FIRE_KEY.H"          //KEY
#include  "FIRE_MMA7455.h"      //三轴加速度MMA7455
#include  "FIRE_NRF24L0.h"      //无线模块NRF24L01+

#include  "FIRE_camera.h"       //摄像头总头文件
#include  "FIRE_LCD.h"          //液晶总头文件

#include  "FIRE_TSL1401.h"      //线性CCD
#include  "FIRE_key_event.h"    //按键消息处理
#include  "FIRE_NRF24L0_MSG.h"  //无线模块消息处理




#endif  //__INCLUDE_H__

