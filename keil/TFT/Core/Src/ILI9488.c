#ifndef __ILI9488_H__
#define __ILI9488_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "stdio.h"
#include "ILI9488.h"


/**
	* @brief  开启背光
	* @retval 无
	*/
void Back_light(void)
{
		LCD_BL_HIGH();
		LCD_BL_LOW();
}

/**
	* @brief  LCD初始化
	* @retval 无
	*/

void lcd_write_cmd(uint8_t cmd)
	{
	LCD_DC_LOW();
	HAL_SPI_Transmit(&hspi2, &cmd, 1, ILI9488_TIMEOUT_VALUE);	
	}
void lcd_write_data(uint8_t data)
	{
	LCD_DC_HIGH();
	HAL_SPI_Transmit(&hspi2, &data, 1, ILI9488_TIMEOUT_VALUE);	
	}
/*
static rt_err_t lcd_write_data(const rt_uint8_t data)
{
    rt_size_t len;

    rt_pin_write(LCD_DC_PIN, PIN_HIGH);

    len = rt_spi_send(spi_dev_lcd, &data, 1);

    if (len != 1)
    {
        LOG_I("lcd_write_data error. %d", len);
        return -RT_ERROR;
    }
    else
    {
        return RT_EOK;
    }
}

static rt_err_t lcd_write_cmd(const rt_uint8_t cmd)
{
    rt_size_t len;

    rt_pin_write(LCD_DC_PIN, PIN_LOW);

    len = rt_spi_send(spi_dev_lcd, &cmd, 1);

    if (len != 1)
    {
        LOG_I("lcd_write_cmd error. %d", len);
        return -RT_ERROR;
    }
    else
    {
        return RT_EOK;
    }
}*/

void LCD_Init(void)
{
		Back_light(); //点亮背光
		LCD_RES_HIGH();
		HAL_Delay(10);
		LCD_RES_LOW();
		HAL_Delay(50);
		LCD_RES_HIGH();
		HAL_Delay(200);
		printf("LCD初始化准备完毕");

		lcd_write_cmd(LCD_PGAMCTRL_Positive_Gamma_Control);//正伽马控制
		lcd_write_data(0x00);
    lcd_write_data(0x07);
    lcd_write_data(0x0f);
    lcd_write_data(0x0D);
    lcd_write_data(0x1B);
    lcd_write_data(0x0A);
    lcd_write_data(0x3c);
    lcd_write_data(0x78);
    lcd_write_data(0x4A);
    lcd_write_data(0x07);
    lcd_write_data(0x0E);
    lcd_write_data(0x09);
    lcd_write_data(0x1B);
    lcd_write_data(0x1e);
    lcd_write_data(0x0f);

		lcd_write_cmd(LCD_NGAMCTRL_Negative_Gamma_Control);//负伽马控制
    lcd_write_data(0x00);
    lcd_write_data(0x22);
    lcd_write_data(0x24);
    lcd_write_data(0x06);
    lcd_write_data(0x12);
    lcd_write_data(0x07);
    lcd_write_data(0x36);
    lcd_write_data(0x47);
    lcd_write_data(0x47);
    lcd_write_data(0x06);
    lcd_write_data(0x0a);
    lcd_write_data(0x07);
    lcd_write_data(0x30);
    lcd_write_data(0x37);
    lcd_write_data(0x0f);

		lcd_write_cmd(LCD_Power_Control_1); //电源控制1
    lcd_write_data(0x10);								//VREG1OUT  =  4.5625V
    lcd_write_data(0x10);								//VREG2OUT  = -4.5625V

    lcd_write_cmd(LCD_Power_Control_2);//电源控制2
    lcd_write_data(0x41);

    lcd_write_cmd(LCD_VCOM_Control);//VCOM控制
    lcd_write_data(0x00);						//未编程NV存储器
    lcd_write_data(0x22);						//VCOM=-1.46875V
    lcd_write_data(0x80);						//VCOM来自VCM_REG [7：0]的值

    lcd_write_cmd(LCD_Memory_Access_Control);//内存访问控制
    lcd_write_data(0x48);
		/*	0x48
		010 :这3位控制着从MPU到存储器写/读的方向。
		0		:LCD垂直刷新方向控制。
		1		:颜色选择器开关控制（0 = RGB彩色滤光片面板，1 = BGR彩色滤光片面板）
		0		:LCD水平刷新方向控制。
		0		:无意义
		0		:无意义
			
		*/
    lcd_write_cmd(LCD_Interface_Pixel_Format);   //接口像素格式
    lcd_write_data(0x66);
		/*0x66
		0		:无意义
		110 :RGB接口格式:18 bits/pixel 
		0		:无意义
		110	:MCU接口格式:18 bits/pixel
		*/

    lcd_write_cmd(LCD_Interface_Mode_Control);   //接口模式控制
    lcd_write_data(0x00);
		/*0x00
		0		:3/4线串行接口选择: SDA_EN = 0，DIN和SDO引脚用于3/4线串行接口。
														SDA_EN = 1，DIN / SDA引脚用于3/4线串行接口，而不使用SDO引脚。
		000 :无意义
		0		:VSYNC极性（0 =低电平同步时钟，1 =高电平同步时钟）
		0		:HSYNC极性（0 =低电平同步时钟，1 =高电平同步时钟）
		0		:DOTCLK极性设置（0 =在上升时间获取的数据，1 =在下降时间获取的数据）
		0		:启用极性  （0 =高启用RGB接口，1 =低启用RGB接口）
		*/

    lcd_write_cmd(LCD_Frame_Rate_Control_In_Normal_Mode_Full_Colors);   //帧速率控制（在正常模式/全彩色下）
    lcd_write_data(0xB0);
		/*0xB0
		1011:设置全彩普通模式的帧频。将普通显示模式的内部时钟的分频比设置为开：CNT=15；
																																					撕裂效果线OFF（R34h）Frame rate=68.36 ；
																																					撕裂效果线ON （R35h）Frame rate=65.65 ；
		00	:无意义
		00	:正常模式下内部时钟的分频比。不分频
		*/
    lcd_write_data(0x11);
		/*0x11
		000		:无意义
		10001	:用于在MCU接口上设置正常模式的1H（通道）周期:17 clocks 
		*/

    lcd_write_cmd(LCD_Display_Inversion_Control);//显示反转控制
    lcd_write_data(0x02);
		/*0x02
		00000:无意义
		010  :2点反转
		*/

    lcd_write_cmd(LCD_Display_Function_Control);   //显示功能控制
    lcd_write_data(0x02);
		/*0x02
		0	:
		0	:
		0	:
		0	:
		00:
		10:
		*/
    lcd_write_data(0x02);

    lcd_write_cmd(LCD_Entry_Mode_Set);//进入模式设定
    lcd_write_data(0xC6);

    lcd_write_cmd(LCD_Set_Image_Function);//设定影像功能
    lcd_write_data(0x00);

    lcd_write_cmd(LCD_Adjust_Control_3);//调整控件3
    lcd_write_data(0xA9);
    lcd_write_data(0x51);
    lcd_write_data(0x2C);
    lcd_write_data(0x82);

    lcd_write_cmd(LCD_Sleep_OUT);//退出睡眠模式
    HAL_Delay(120);
    lcd_write_cmd(LCD_Display_ON);//显示开启
		
}

#ifdef __cplusplus
}
#endif

#endif /* __ILI9488_H__ */
