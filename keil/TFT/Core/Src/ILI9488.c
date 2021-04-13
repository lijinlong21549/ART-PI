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
	* @brief  ��������
	* @retval ��
	*/
void Back_light(void)
{
		LCD_BL_HIGH();
		LCD_BL_LOW();
}

/**
	* @brief  LCD��ʼ��
	* @retval ��
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
		Back_light(); //��������
		LCD_RES_HIGH();
		HAL_Delay(10);
		LCD_RES_LOW();
		HAL_Delay(50);
		LCD_RES_HIGH();
		HAL_Delay(200);
		printf("LCD��ʼ��׼�����");

		lcd_write_cmd(LCD_PGAMCTRL_Positive_Gamma_Control);//��٤�����
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

		lcd_write_cmd(LCD_NGAMCTRL_Negative_Gamma_Control);//��٤�����
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

		lcd_write_cmd(LCD_Power_Control_1); //��Դ����1
    lcd_write_data(0x10);								//VREG1OUT  =  4.5625V
    lcd_write_data(0x10);								//VREG2OUT  = -4.5625V

    lcd_write_cmd(LCD_Power_Control_2);//��Դ����2
    lcd_write_data(0x41);

    lcd_write_cmd(LCD_VCOM_Control);//VCOM����
    lcd_write_data(0x00);						//δ���NV�洢��
    lcd_write_data(0x22);						//VCOM=-1.46875V
    lcd_write_data(0x80);						//VCOM����VCM_REG [7��0]��ֵ

    lcd_write_cmd(LCD_Memory_Access_Control);//�ڴ���ʿ���
    lcd_write_data(0x48);
		/*	0x48
		010 :��3λ�����Ŵ�MPU���洢��д/���ķ���
		0		:LCD��ֱˢ�·�����ơ�
		1		:��ɫѡ�������ؿ��ƣ�0 = RGB��ɫ�˹�Ƭ��壬1 = BGR��ɫ�˹�Ƭ��壩
		0		:LCDˮƽˢ�·�����ơ�
		0		:������
		0		:������
			
		*/
    lcd_write_cmd(LCD_Interface_Pixel_Format);   //�ӿ����ظ�ʽ
    lcd_write_data(0x66);
		/*0x66
		0		:������
		110 :RGB�ӿڸ�ʽ:18 bits/pixel 
		0		:������
		110	:MCU�ӿڸ�ʽ:18 bits/pixel
		*/

    lcd_write_cmd(LCD_Interface_Mode_Control);   //�ӿ�ģʽ����
    lcd_write_data(0x00);
		/*0x00
		0		:3/4�ߴ��нӿ�ѡ��: SDA_EN = 0��DIN��SDO��������3/4�ߴ��нӿڡ�
														SDA_EN = 1��DIN / SDA��������3/4�ߴ��нӿڣ�����ʹ��SDO���š�
		000 :������
		0		:VSYNC���ԣ�0 =�͵�ƽͬ��ʱ�ӣ�1 =�ߵ�ƽͬ��ʱ�ӣ�
		0		:HSYNC���ԣ�0 =�͵�ƽͬ��ʱ�ӣ�1 =�ߵ�ƽͬ��ʱ�ӣ�
		0		:DOTCLK�������ã�0 =������ʱ���ȡ�����ݣ�1 =���½�ʱ���ȡ�����ݣ�
		0		:���ü���  ��0 =������RGB�ӿڣ�1 =������RGB�ӿڣ�
		*/

    lcd_write_cmd(LCD_Frame_Rate_Control_In_Normal_Mode_Full_Colors);   //֡���ʿ��ƣ�������ģʽ/ȫ��ɫ�£�
    lcd_write_data(0xB0);
		/*0xB0
		1011:����ȫ����ͨģʽ��֡Ƶ������ͨ��ʾģʽ���ڲ�ʱ�ӵķ�Ƶ������Ϊ����CNT=15��
																																					˺��Ч����OFF��R34h��Frame rate=68.36 ��
																																					˺��Ч����ON ��R35h��Frame rate=65.65 ��
		00	:������
		00	:����ģʽ���ڲ�ʱ�ӵķ�Ƶ�ȡ�����Ƶ
		*/
    lcd_write_data(0x11);
		/*0x11
		000		:������
		10001	:������MCU�ӿ�����������ģʽ��1H��ͨ��������:17 clocks 
		*/

    lcd_write_cmd(LCD_Display_Inversion_Control);//��ʾ��ת����
    lcd_write_data(0x02);
		/*0x02
		00000:������
		010  :2�㷴ת
		*/

    lcd_write_cmd(LCD_Display_Function_Control);   //��ʾ���ܿ���
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

    lcd_write_cmd(LCD_Entry_Mode_Set);//����ģʽ�趨
    lcd_write_data(0xC6);

    lcd_write_cmd(LCD_Set_Image_Function);//�趨Ӱ����
    lcd_write_data(0x00);

    lcd_write_cmd(LCD_Adjust_Control_3);//�����ؼ�3
    lcd_write_data(0xA9);
    lcd_write_data(0x51);
    lcd_write_data(0x2C);
    lcd_write_data(0x82);

    lcd_write_cmd(LCD_Sleep_OUT);//�˳�˯��ģʽ
    HAL_Delay(120);
    lcd_write_cmd(LCD_Display_ON);//��ʾ����
		
}

#ifdef __cplusplus
}
#endif

#endif /* __ILI9488_H__ */
