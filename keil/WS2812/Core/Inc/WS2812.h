
#ifndef __WS2812_H__
#define __WS2812_H__
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"



/*���ܺ���*/
void WS2812_Init(void);//��ʼ��
int WS2812_Return_HZ(void);//����ˢ����

/*���ƺ���*/

void LED_Cleared_SET(int SET);//��ĳһ��LEDϨ��!!!
void WS2812_OLL_slake(void);//������ledȫ��Ϩ��
void ws2812_send_ONE(int SET);//��������

/*��ɫ���ú���*/
void WS2812_PUT_ONE_LED(int LED_PUT_SET,int LED_PUT_number,int LED_colour_R,int LED_colour_B,int LED_colour_G );//���õ�һLED��ɫ
void WS2812_PUT_ONE_LED_colour(int LED_PUT_SET,int LED_PUT_number,int LED_colour[3]);//ʹ�ó�����ɫ���õ�һLED��ɫ��
void WS2812_PUT_SET_LED(int LED_PUT_SET,int LED_colour_R,int LED_colour_B,int LED_colour_G);//��������LED��ɫ��
void WS2812_PUT_SET_LED_colour(int LED_PUT_SET,int LED_colour[3]);//ʹ�ó�����ɫ��������LED��ɫ��

/*�������ú���*/
void WS2812_AUTO_send_OFF(void);//�ر��Զ�����
void WS2812_AUTO_send_ON(void);//�����Զ�����
void ws2812_SET_HZ(int HZ);//����LED��ˢ����
void ws2812_SET_Bri(int Bri);//����LED������


void WS2812_parameter(int R,int B,int G,int led_send_number);




/*������ɫ��{R,B,G}*/
#define LED_colour_black        {0,0,0}         //��ɫ
#define LED_colour_white        {255,255,255}   //��ɫ
#define LED_colour_red          {255,0,0}       //��ɫ
#define LED_colour_green        {0,255,0}       //��ɫ
#define LED_colour_blue         {0,255,0}       //��ɫ
#define LED_colour_purple       {255,0,255}     //��ɫ
#define LED_colour_yellow       {255,255,0}     //��ɫ
#define LED_colour_Orange       {255,165,0}     //��ɫ
#define LED_colour_cyan         {0,255,255}     //��ɫ


#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
