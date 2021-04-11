/**
 *@file    ILI9488.h
 *@brief   ʹ��SPI����ILI9488IC����Ļ
 *@author  Z
 *@date    2021/4/11
 *@note    ������ʵ��ILI9488����Ļ����
*/

#ifndef _ILI9488__H_
#define _ILI9488__H_

#include "main.h"
#include "spi.h"
/*���Ų����궨��*/
//LCD_SPIƬѡ
#define LCD_CS_HIGH() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_LOW() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
//LCD ����/�Ĵ���ѡ��
#define LCD_DC_HIGH() HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)
#define LCD_DC_LOW() HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)
//LCD ��λ
#define LCD_RES_HIGH() HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET)
#define LCD_RES_LOW() HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET)
//LCD ����
#define LCD_BL_HIGH() HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)
#define LCD_BL_LOW() HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)

/*ILI9488����궨��*/
#define LCD_NOP 0x00                                                      //�޲���
#define LCD_Software_Reset 0x01                                           //�������
#define LCD_Read_Display_Identification_Information 0x04                  //��ȡ��ʾ��ʶ��Ϣ
#define LCD_Read Number_of_the_Errors_on_DSI 0x05                         //��ȡDSI�ϵĴ�����
#define LCD_Read_Display_Status 0x09                                      //��ȡ��ʾ״̬
#define LCD_Read_Display_Power_Mode 0x0A                                  //��ȡ��ʾ��Դģʽ
#define LCD_Read_Display_MADCTL 0x0B                                      //��ȡ��ʾMADCTL
#define LCD_Read_Display_Pixel_Format 0x0C                                //��ȡ��ʾ���ظ�ʽ
#define LCD_Read_Display_Image_Mode 0x0D                                  //��ȡ��ʾͼ��ģʽ
#define LCD_Read_Display_Signal_Mode 0x0E                                 //��ȡ��ʾ�ź�ģʽ
#define LCD_Read_Display_Self_Diagnostic_Result 0x0F                      //��ȡ��ʾ������Ͻ��
#define LCD_Sleep_IN 0x10                                                 //����˯��ģʽ
#define LCD_Sleep_OUT 0x11                                                //�˳�˯��ģʽ
#define LCD_Partial_Mode_ON 0x12                                          //��������ģʽ
#define LCD_Normal_Display_Mode_ON 0x13                                   //������ʾģʽ����
#define LCD_Display_Inversion_OFF 0x20                                    //��ʾ��ת�ر�
#define LCD_Display_Inversion_ON 0x21                                     //��ʾ��ת����
#define LCD_All_pixels_OFF 0x22                                           //�������عر�
#define LCD_All_pixels_ON 0x23                                            //�������ؿ���
#define LCD_All_pixels_OFF 0x22                                           //�������عر�
#define LCD_Display_OFF 0x28                                              //��ʾ�ر�
#define LCD_Display_ON 0x29                                               //��ʾ����
#define LCD_Column_Address_Set 0x2A                                       //�е�ַ��
#define LCD_Page Address Set 0x2B                                         //ҳ���ַ��
#define LCD_Memory_Write 0x2C                                             //�ڴ�д��
#define LCD_Memory_Read 0x2E                                              //�ڴ��ȡ
#define LCD_Partial_Area 0x30                                             //��������
#define LCD_Vertical_Scrolling_Definition 0x33                            //��ֱ��������
#define LCD_Tearing_Effect_Line_OFF 0x34                                  //˺��Ч���߹ر�
#define LCD_Tearing_Effect_Line_ON 0x35                                   //˺��Ч���ߴ�
#define LCD_Memory Access Control 0x36                                    //�ڴ���ʿ���
#define LCD_Vertical_Scrolling_Start_Address 0x37                         //��ֱ������ʼ��ַ
#define LCD_Idle_Mode_OFF 0x38                                            //����ģʽ�ر�
#define LCD_Idle_Mode_ON 0x39                                             //����ģʽ����
#define LCD_Interface_Pixel_Format 0x3A                                   //�ӿ����ظ�ʽ
#define LCD_Memory_Write_Continue 0x3C                                    //�ڴ�д�����
#define LCD_Memory_Read_Continue 0x3E                                     //�ڴ��ȡ����
#define LCD_Write_Tear_Scan_Line 0x44                                     //д˺��ɨ����
#define LCD_Read_Tear_Scan_Line 0x45                                      //��ȡɨ����
#define LCD_Write_Display_Brightness 0x51                                 //д����ʾ����ֵ
#define LCD_Read_Display_Brightness_Value 0x52                            //��ȡ��ʾ����ֵ
#define LCD_Write_CTRL_Display_Value 0x53                                 //д��CTRL��ʾֵ
#define LCD_Write_Content_Adaptive_Brightness_Control_Value 0x55          //д��������Ӧ���ȿ���ֵ
#define LCD_Read_Content_Adaptive_Brightness_Control_Value 0x55           //��ȡ��������Ӧ���ȿ���ֵ
#define LCD_Write_CABC_Minimum_Brightness 0x5E                            //дCABC��С����
#define LCD_Read_CABC_Minimum_Brightness 0x5F                             //��ȡCABC��С����
#define LCD_Read_Automatic_Brightness_Control_Self_diagnostic_Result 0x68 //��ȡ�Զ����ȿ�������Ͻ��
#define LCD_Read_ID1 0xDA                                                 //��ȡID1
#define LCD_Read_ID2 0xDB                                                 //��ȡID2
#define LCD_Read_ID3 0xDC                                                 //��ȡID3


/*��չ����*/
#define LCD_Interface_Mode_Control 0xB0                                   //�ӿ�ģʽ����
#define LCD_Frame_Rate_Control_In_Normal_Mode_Full_Colors 0xB1            //֡���ʿ��ƣ�������ģʽ/ȫ��ɫ�£�
#define LCD_Frame_Rate_Control_In_Idle_Mode_8_Colors 0xB2                 //֡���ʿ��ƣ��ڿ���ģʽ��Ϊ8����ɫ��
#define LCD_Frame_Rate_Control_In_Partial_Mode_Full_Colors 0xB3           //֡���ʿ��ƣ��ڲ���ģʽ/ȫ��ɫ�£�
#define LCD_Display_Inversion_Control 0xB4                                //��ʾ��ת����
#define LCD_Blanking_Porch_Control 0xB5                                   //�����ſ��ƣ���
#define LCD_Display_Function_Control 0xB6                                 //��ʾ���ܿ���
#define LCD_Entry_Mode_Set 0xB7                                           //����ģʽ�趨
#define LCD_Color_Enhancement_Control_1 0xB9                              //ɫ����ǿ����1
#define LCD_Color_Enhancement_Control_2 0xBA                              //ɫ����ǿ����2
#define LCD_HS_Lanes_Control 0xBE                                         //HSͨ������
#define LCD_Power_Control_1 0xC0                                          //��Դ����1
#define LCD_Power_Control_2 0xC1                                          //��Դ����2
#define LCD_Power_Control_3_For_Normal_Mode 0xC2                          //��Դ����3(������ͨģʽ)
#define LCD_Power_Control_4_For_Idle_Mode 0xC3                            //��Դ����4(���ڿ���ģʽ)
#define LCD_Power_Control_5_For_Partial_Mode 0xC4                         //��Դ����5(���ڲ���ģʽ)
#define LCD_VCOM_Control 0xC5                                             //VCOM����
#define LCD_CABC_Control_1 0xC6                                           //CABC�ؼ�1
#define LCD_CABC_Control_2 0xC8                                           //CABC�ؼ�2
#define LCD_CABC_Control_3 0xC9                                           //CABC�ؼ�3
#define LCD_CABC_Control_4 0xCA                                           //CABC�ؼ�4
#define LCD_CABC_Control_5 0xCB                                           //CABC�ؼ�5
#define LCD_CABC_Control_6 0xCC                                           //CABC�ؼ�6
#define LCD_CABC_Control_7 0xCD                                           //CABC�ؼ�7
#define LCD_CABC_Control_8 0xCE                                           //CABC�ؼ�8
#define LCD_CABC_Control_9 0xCF                                           //CABC�ؼ�9
#define LCD_NV_Memory_Write 0xD0                                          //NV�洢��д
#define LCD_NV_Memory_Protection_Key 0xD1                                 //NV�ڴ汣����
#define LCD_NV_Memory_Status_Read 0xD2                                    //NV�ڴ�״̬��ȡ
#define LCD_Read_ID4 0xD3                                                 //��ȡID4
#define LCD_Adjust_Control_1 0xD7                                         //�����ؼ�1
#define LCD_PGAMCTRL_Positive_Gamma_Control 0xE0                          //PGAMCTRL(��٤�����)
#define LCD_NGAMCTRL_Negative_Gamma Control 0xE1                          //PGAMCTRL(��٤�����)
#define LCD_Ditigal_Gamma_Control_1 0xE2                                  //����٤�����1
#define LCD_Ditigal_Gamma_Control_2 0xE3                                  //����٤�����2
#define LCD_Set_Image_Function 0xE9                                       //�趨Ӱ����
#define LCD_Adjust_Control_2 0xF2                                         //�����ؼ�2
#define LCD_Adjust_Control_3 0xF7                                         //�����ؼ�3
#define LCD_Adjust_Control_4 0xF8                                         //�����ؼ�4
#define LCD_Adjust_Control_5 0xF9                                         //�����ؼ�5
#define LCD_SPI_Read_Command_Setting 0xFB                                 //SPI��ȡ��������
#define LCD_Adjust_Control_6 0xFC                                         //�����ؼ�6

void LCD_Init(void);

#endif /* _ILI9488__H_ */
