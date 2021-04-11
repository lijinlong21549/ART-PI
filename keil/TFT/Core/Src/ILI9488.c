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

    /*
    #define LCD_CS_HIGH() 			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
    #define LCD_CS_LOW() 	    	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

    #define LCD_DC_HIGH() 			HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)
    #define LCD_DC_LOW() 	    	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)

    #define LCD_RES_HIGH() 			HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET)
    #define LCD_RES_LOW() 	  	HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET)

    #define LCD_BL_HIGH() 			HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)
    #define LCD_BL_LOW()   	  	HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)
    */

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
    }

#ifdef __cplusplus
}
#endif

#endif /* __ILI9488_H__ */
