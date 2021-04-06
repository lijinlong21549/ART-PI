
#ifndef __WS2812_H__
#define __WS2812_H__
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"



/*功能函数*/
void WS2812_Init(void);//初始化
int WS2812_Return_HZ(void);//返回刷新率

/*控制函数*/

void LED_Cleared_SET(int SET);//将某一组LED熄灭!!!
void WS2812_OLL_slake(void);//将所有led全部熄灭
void ws2812_send_ONE(int SET);//发送脉冲

/*颜色设置函数*/
void WS2812_PUT_ONE_LED(int LED_PUT_SET,int LED_PUT_number,int LED_colour_R,int LED_colour_B,int LED_colour_G );//设置单一LED颜色
void WS2812_PUT_ONE_LED_colour(int LED_PUT_SET,int LED_PUT_number,int LED_colour[3]);//使用常用颜色设置单一LED颜色。
void WS2812_PUT_SET_LED(int LED_PUT_SET,int LED_colour_R,int LED_colour_B,int LED_colour_G);//设置整组LED颜色。
void WS2812_PUT_SET_LED_colour(int LED_PUT_SET,int LED_colour[3]);//使用常用颜色设置整组LED颜色。

/*参数设置函数*/
void WS2812_AUTO_send_OFF(void);//关闭自动发送
void WS2812_AUTO_send_ON(void);//开启自动发送
void ws2812_SET_HZ(int HZ);//设置LED的刷新率
void ws2812_SET_Bri(int Bri);//设置LED的亮度


void WS2812_parameter(int R,int B,int G,int led_send_number);




/*常用颜色宏{R,B,G}*/
#define LED_colour_black        {0,0,0}         //黑色
#define LED_colour_white        {255,255,255}   //白色
#define LED_colour_red          {255,0,0}       //红色
#define LED_colour_green        {0,255,0}       //绿色
#define LED_colour_blue         {0,255,0}       //蓝色
#define LED_colour_purple       {255,0,255}     //紫色
#define LED_colour_yellow       {255,255,0}     //黄色
#define LED_colour_Orange       {255,165,0}     //橙色
#define LED_colour_cyan         {0,255,255}     //橙色


#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
