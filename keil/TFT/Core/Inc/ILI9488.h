/**
 *@file    ILI9488.h
 *@brief   使用SPI操作ILI9488IC的屏幕
 *@author  Z
 *@date    2021/4/11
 *@note    此驱动实现ILI9488的屏幕运行
*/

#ifndef _ILI9488__H_
#define _ILI9488__H_

#include "main.h"
#include "spi.h"
/*引脚操作宏定义*/
//LCD_SPI片选
#define LCD_CS_HIGH() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_LOW() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
//LCD 数据/寄存器选择
#define LCD_DC_HIGH() HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)
#define LCD_DC_LOW() HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)
//LCD 复位
#define LCD_RES_HIGH() HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET)
#define LCD_RES_LOW() HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET)
//LCD 背光
#define LCD_BL_HIGH() HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)
#define LCD_BL_LOW() HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)

/*ILI9488命令宏定义*/
#define LCD_NOP 0x00                                                      //无操作
#define LCD_Software_Reset 0x01                                           //软件重置
#define LCD_Read_Display_Identification_Information 0x04                  //读取显示标识信息
#define LCD_Read Number_of_the_Errors_on_DSI 0x05                         //读取DSI上的错误数
#define LCD_Read_Display_Status 0x09                                      //读取显示状态
#define LCD_Read_Display_Power_Mode 0x0A                                  //读取显示电源模式
#define LCD_Read_Display_MADCTL 0x0B                                      //读取显示MADCTL
#define LCD_Read_Display_Pixel_Format 0x0C                                //读取显示像素格式
#define LCD_Read_Display_Image_Mode 0x0D                                  //读取显示图像模式
#define LCD_Read_Display_Signal_Mode 0x0E                                 //读取显示信号模式
#define LCD_Read_Display_Self_Diagnostic_Result 0x0F                      //读取显示自我诊断结果
#define LCD_Sleep_IN 0x10                                                 //进入睡眠模式
#define LCD_Sleep_OUT 0x11                                                //退出睡眠模式
#define LCD_Partial_Mode_ON 0x12                                          //开启部分模式
#define LCD_Normal_Display_Mode_ON 0x13                                   //正常显示模式开启
#define LCD_Display_Inversion_OFF 0x20                                    //显示反转关闭
#define LCD_Display_Inversion_ON 0x21                                     //显示反转开启
#define LCD_All_pixels_OFF 0x22                                           //所有像素关闭
#define LCD_All_pixels_ON 0x23                                            //所有像素开启
#define LCD_All_pixels_OFF 0x22                                           //所有像素关闭
#define LCD_Display_OFF 0x28                                              //显示关闭
#define LCD_Display_ON 0x29                                               //显示开启
#define LCD_Column_Address_Set 0x2A                                       //列地址集
#define LCD_Page Address Set 0x2B                                         //页面地址集
#define LCD_Memory_Write 0x2C                                             //内存写入
#define LCD_Memory_Read 0x2E                                              //内存读取
#define LCD_Partial_Area 0x30                                             //部分区域
#define LCD_Vertical_Scrolling_Definition 0x33                            //垂直滚动定义
#define LCD_Tearing_Effect_Line_OFF 0x34                                  //撕裂效果线关闭
#define LCD_Tearing_Effect_Line_ON 0x35                                   //撕裂效果线打开
#define LCD_Memory Access Control 0x36                                    //内存访问控制
#define LCD_Vertical_Scrolling_Start_Address 0x37                         //垂直滚动开始地址
#define LCD_Idle_Mode_OFF 0x38                                            //空闲模式关闭
#define LCD_Idle_Mode_ON 0x39                                             //空闲模式开启
#define LCD_Interface_Pixel_Format 0x3A                                   //接口像素格式
#define LCD_Memory_Write_Continue 0x3C                                    //内存写入继续
#define LCD_Memory_Read_Continue 0x3E                                     //内存读取继续
#define LCD_Write_Tear_Scan_Line 0x44                                     //写撕裂扫描线
#define LCD_Read_Tear_Scan_Line 0x45                                      //读取扫描线
#define LCD_Write_Display_Brightness 0x51                                 //写入显示亮度值
#define LCD_Read_Display_Brightness_Value 0x52                            //读取显示亮度值
#define LCD_Write_CTRL_Display_Value 0x53                                 //写入CTRL显示值
#define LCD_Write_Content_Adaptive_Brightness_Control_Value 0x55          //写内容自适应亮度控制值
#define LCD_Read_Content_Adaptive_Brightness_Control_Value 0x55           //读取内容自适应亮度控制值
#define LCD_Write_CABC_Minimum_Brightness 0x5E                            //写CABC最小亮度
#define LCD_Read_CABC_Minimum_Brightness 0x5F                             //读取CABC最小亮度
#define LCD_Read_Automatic_Brightness_Control_Self_diagnostic_Result 0x68 //读取自动亮度控制自诊断结果
#define LCD_Read_ID1 0xDA                                                 //读取ID1
#define LCD_Read_ID2 0xDB                                                 //读取ID2
#define LCD_Read_ID3 0xDC                                                 //读取ID3


/*扩展命令*/
#define LCD_Interface_Mode_Control 0xB0                                   //接口模式控制
#define LCD_Frame_Rate_Control_In_Normal_Mode_Full_Colors 0xB1            //帧速率控制（在正常模式/全彩色下）
#define LCD_Frame_Rate_Control_In_Idle_Mode_8_Colors 0xB2                 //帧速率控制（在空闲模式下为8种颜色）
#define LCD_Frame_Rate_Control_In_Partial_Mode_Full_Colors 0xB3           //帧速率控制（在部分模式/全彩色下）
#define LCD_Display_Inversion_Control 0xB4                                //显示反转控制
#define LCD_Blanking_Porch_Control 0xB5                                   //落料门控制？？
#define LCD_Display_Function_Control 0xB6                                 //显示功能控制
#define LCD_Entry_Mode_Set 0xB7                                           //进入模式设定
#define LCD_Color_Enhancement_Control_1 0xB9                              //色彩增强控制1
#define LCD_Color_Enhancement_Control_2 0xBA                              //色彩增强控制2
#define LCD_HS_Lanes_Control 0xBE                                         //HS通道控制
#define LCD_Power_Control_1 0xC0                                          //电源控制1
#define LCD_Power_Control_2 0xC1                                          //电源控制2
#define LCD_Power_Control_3_For_Normal_Mode 0xC2                          //电源控制3(用于普通模式)
#define LCD_Power_Control_4_For_Idle_Mode 0xC3                            //电源控制4(用于空闲模式)
#define LCD_Power_Control_5_For_Partial_Mode 0xC4                         //电源控制5(用于部分模式)
#define LCD_VCOM_Control 0xC5                                             //VCOM控制
#define LCD_CABC_Control_1 0xC6                                           //CABC控件1
#define LCD_CABC_Control_2 0xC8                                           //CABC控件2
#define LCD_CABC_Control_3 0xC9                                           //CABC控件3
#define LCD_CABC_Control_4 0xCA                                           //CABC控件4
#define LCD_CABC_Control_5 0xCB                                           //CABC控件5
#define LCD_CABC_Control_6 0xCC                                           //CABC控件6
#define LCD_CABC_Control_7 0xCD                                           //CABC控件7
#define LCD_CABC_Control_8 0xCE                                           //CABC控件8
#define LCD_CABC_Control_9 0xCF                                           //CABC控件9
#define LCD_NV_Memory_Write 0xD0                                          //NV存储器写
#define LCD_NV_Memory_Protection_Key 0xD1                                 //NV内存保护锁
#define LCD_NV_Memory_Status_Read 0xD2                                    //NV内存状态读取
#define LCD_Read_ID4 0xD3                                                 //读取ID4
#define LCD_Adjust_Control_1 0xD7                                         //调整控件1
#define LCD_PGAMCTRL_Positive_Gamma_Control 0xE0                          //PGAMCTRL(正伽马控制)
#define LCD_NGAMCTRL_Negative_Gamma Control 0xE1                          //PGAMCTRL(负伽马控制)
#define LCD_Ditigal_Gamma_Control_1 0xE2                                  //数字伽马控制1
#define LCD_Ditigal_Gamma_Control_2 0xE3                                  //数字伽马控制2
#define LCD_Set_Image_Function 0xE9                                       //设定影像功能
#define LCD_Adjust_Control_2 0xF2                                         //调整控件2
#define LCD_Adjust_Control_3 0xF7                                         //调整控件3
#define LCD_Adjust_Control_4 0xF8                                         //调整控件4
#define LCD_Adjust_Control_5 0xF9                                         //调整控件5
#define LCD_SPI_Read_Command_Setting 0xFB                                 //SPI读取命令设置
#define LCD_Adjust_Control_6 0xFC                                         //调整控件6

void LCD_Init(void);

#endif /* _ILI9488__H_ */
