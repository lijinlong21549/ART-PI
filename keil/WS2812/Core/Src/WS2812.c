//ͷ����
#include "WS2812.h"
//#include "main.h"
#include "tim.h"

/*ʱ�Ӳ���
ռ�ձ� = ��Pulse/Period��*100%��Ƶ��: f = 48M/Prescaler/Period
1�ź�ռ�ձ� = ��156/312��=50%����Pulse=156-1
0�ź�ռ�ձ� = ��84/312��=26.9%����Pulse=84-1
Ƶ�ʣ�f = 240/1/312= 0.769230MHZ
*/
#define LED_SET          16
#define LED_number       8
#define N                LED_number*24
//#define EXT_SDRAM_ADDR  	((uint32_t)0xC0000000)
//#define EXT_SDRAM_SIZE		(32 * 1024 * 1024)
uint32_t LED_OUT_test[2]={0,0};
uint32_t LED_OUT[LED_SET][LED_number][24]; //__attribute__((at(EXT_SDRAM_ADDR))) ;//����LED����ά����[LED��][LED�ĸ���][LED��24������]
int num =(1*LED_number*24)+4;
uint32_t LED_DATA[(1*LED_number*24)+4];
uint32_t OUT1[196]={0,0,
								39,39,39,39,39,39,39,39,
								39,39,39,39,39,39,39,39,
								39,39,39,39,39,39,39,39,

								21,21,21,21,21,21,21,21,
								21,21,21,21,21,21,21,21,
								21,21,21,21,21,21,21,21,

								39,39,39,39,39,39,39,39,
								39,39,39,39,39,39,39,39,
								39,39,39,39,39,39,39,39,

								21,21,21,21,21,21,21,21,
								21,21,21,21,21,21,21,21,
								21,21,21,21,21,21,21,21,

								39,39,39,39,39,39,39,39,
								39,39,39,39,39,39,39,39,
								39,39,39,39,39,39,39,39,

								21,21,21,21,21,21,21,21,
								21,21,21,21,21,21,21,21,
								21,21,21,21,21,21,21,21,

								39,39,39,39,39,39,39,39,
								39,39,39,39,39,39,39,39,
								39,39,39,39,39,39,39,39,

								21,21,21,21,21,21,21,21,
								21,21,21,21,21,21,21,21,
								21,21,21,21,21,21,21,21,
								0,0};

uint8_t RED[8];
uint8_t BLUE[8];
uint8_t GREEN[8];

struct WS2812_1 
{
	
	int led_num;//LED���ӵ�������1-2048
	int led_Bri;//LED��ʾ�����ȣ�0-100	
	int led_Hz;//LED��ˢ���ʣ�1-60Hz
	int led_HIGH;//LED�ĸߵ�ƽռ�ձ�
	int led_LOW;//LED�ĵ͵�ƽռ�ձ�
	int AUTO_send;//�Զ�����
	int AUTO_send_SET;//ִ���Զ����͵���
	uint32_t led_GPIO;//LED��PWM����GPIO
	uint32_t led_pin;//LED��PWM��������
	uint32_t LED_DMA;//LED��PWM��ռ�õ�DMA

	int led_PWM_1_Duty_cycle;//LED��PWM��1�źŵ�ռ�ձ�
	int led_PWM_0_Duty_cycle;//LED��PWM��0�źŵ�ռ�ձ�

	
}WS2812_1;

/**
  * @brief  ���͵�������
  * @param  SET��LED����
  * @retval ��ֵ
  */
void ws2812_send(int SET)//��������
{	
	/*
	//HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_1,(uint32_t *)OUT1,50);
	HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_1,(uint32_t *)LED_OUT_test,2);//�ȷ��������źţ��ȶ�DMA
	for(int i=0;i<LED_number;i++)
	{  
		HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_1,(uint32_t *)LED_OUT[SET][LED_number],N);
	}
	HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_1,(uint32_t *)LED_OUT_test,2);
	*/
/*
	LED_DATA[0]=0;
	LED_DATA[1]=0;
	LED_DATA[2]=0;
	LED_DATA[3]=0;
	LED_DATA[4]=0;
	LED_DATA[5]=0;
	LED_DATA[6]=0;
	LED_DATA[num-1]=0;
	LED_DATA[num-2]=0;
	for (int a=7;a<num-2;a++)
	{
	LED_DATA[a]=39;
	}
*/

	LED_DATA[0]=0;
	LED_DATA[1]=0;
	LED_DATA[num-1]=0;
	LED_DATA[num-2]=0;
	for (int a=0;a<LED_number;a++)
	{
		for (int b=0;b<24;b++)
	{
	LED_DATA[2+(a)*24+b]= LED_OUT[SET][a][b];
	}
	}

	//HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_1,(uint32_t *)LED_DATA,((1*LED_number*24)+3));
	HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_1,(uint32_t *)LED_DATA,195);

}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)// DMA ������ɻص�����
{
    HAL_TIM_PWM_Stop_DMA(&htim2,TIM_CHANNEL_1);
}



/*
������Decimal_TO_Binary(int DATA,int DATA_BAT)
���ܣ���ʮ��������תΪ��30��66���ڵĶ�����
������
DATA����Ҫת���ĺ���
RGB_colour:ѡ��ת������RGB
*/
void Decimal_TO_Binary(int DATA,int RGB_colour)
	{
	//���ڷ��ص�����	
	uint32_t OUT_DATA[8];

	//�Ƚ�OUT_DATA���0�ַ�
	for(int i=0;i<8;i++)
	{
		OUT_DATA[i]=33;
	}

	//����һ������whileѭ����DATA
	int W_DATA=(DATA*WS2812_1.led_Bri)/100;//������������
	//int W_DATA=DATA*0.8;
	int A=8;
	int B=0;
	while(!(A==0))
	{
		B=W_DATA%2;
		
		if(B==1)
		{
			OUT_DATA[A-1]=WS2812_1.led_PWM_1_Duty_cycle;
		}
		else
		{
			OUT_DATA[A-1]=WS2812_1.led_PWM_0_Duty_cycle;
		}
		W_DATA=W_DATA/2;
		
		A=A-1;
	}

	//
	if(RGB_colour==0)
	{
		for(int i=0;i<8;i++)
		{
			RED[i]=OUT_DATA[i];
		}
	}

	if(RGB_colour==1)
	{
		for(int i=0;i<8;i++)
		{
			BLUE[i]=OUT_DATA[i];
		}
	}

	if(RGB_colour==2)
	{
		for(int i=0;i<8;i++)
		{
			GREEN[i]=OUT_DATA[i];
		}
	}
	return;
	//����������ֿ���ɾȥ
	//����������ݶ���ת��Ϊ30��66��01�룬����ת������������

}


//�ɵ��ú���

/*
������ws2812_send
���ܣ�����N
������
*/



/**
  * @brief  ��ĳһ��LEDϨ��
  * @param  SET��LED����
  * @retval ��ֵ
  */
void LED_Cleared_SET(int SET)
{
	//LEDλѭ��
	for(int number=0;number<LED_number;number++)
	{
	//LED RBGѭ��
	for(int RBG=0;RBG<24;RBG++)
	{
		LED_OUT[SET][number][RBG]=WS2812_1.led_PWM_0_Duty_cycle;//����LEDֵ�������źŲ���
	}
	}
	
}

/**
 * @brief	������ledȫ��Ϩ��
*/
void WS2812_OLL_slake(void)
{
	for(int i=0;i<LED_SET;i++)
	{
		LED_Cleared_SET(i);
	}
	ws2812_send(1);//����
}


/**
 * @brief	WS2812��ʼ��
*/
void WS2812_Init(void)//��ʼ��WS2812
{

	WS2812_1.led_num=LED_number;//LED���ӵ�������1-2000
	WS2812_1.led_Bri=100;//LED��ʾ�����ȣ�0-100
	WS2812_1.led_Hz=60;//LED��ˢ���ʣ�1-60Hz
	WS2812_1.led_HIGH=50;//LED�ĸߵ�ƽռ�ձ�
	WS2812_1.led_LOW=27;//LED�ĵ͵�ƽռ�ձ�
	WS2812_1.led_GPIO='A';//LED��PWM��������GPIO
	WS2812_1.led_pin='1';//LED��PWM��������
	WS2812_1.LED_DMA='1';//LED��PWM��ռ�õ�DMA
	WS2812_1.AUTO_send=0;//�ر��Զ�����
	WS2812_1.AUTO_send_SET=0;//ִ���Զ����͵���Ϊ0
	WS2812_1.led_PWM_1_Duty_cycle=39;//LED��PWM��1�źŵ�ռ�ձ�
	WS2812_1.led_PWM_0_Duty_cycle=21;//LED��PWM��0�źŵ�ռ�ձ�

	WS2812_OLL_slake();//Ϩ������LED��
}

/**
  * @brief  �����Զ����͡�
  * @retval ��ֵ
  */
void WS2812_AUTO_send_ON(void)
{
	WS2812_1.AUTO_send=1;//�����Զ�����
}
/**
  * @brief  �ر��Զ����͡�
  * @retval ��ֵ
  */
void WS2812_AUTO_send_OFF(void)
{
	WS2812_1.AUTO_send=0;//�ر��Զ�����
}


/**
  * @brief  ����WS2812�����е�ˢ���ʲ�����
  * @retval WS2812ˢ����
  */
int WS2812_Return_HZ(void)
{
	int HZ=WS2812_1.led_Hz;
	return(HZ);
}

int WS2812_Return_AUTO_send(void)
{
	int AUTO_send=WS2812_1.AUTO_send;
	return(AUTO_send);
}
/**
  * @brief  ���õ�һLED��ɫ��
  * @param  LED_PUT_SET��LED���ڵ���
  * @param  LED_PUT_number��LED���ڵ�λ
  * @param  LED_colour_R��LED��ɫ�ĺ�
  * @param  LED_colour_B��LED��ɫ����
  * @param  LED_colour_G��LED��ɫ����
  * @retval ��ֵ
  */
void WS2812_PUT_ONE_LED(int LED_PUT_SET,int LED_PUT_number,int LED_colour_R,int LED_colour_B,int LED_colour_G )
{
	//��ʮ����ת��Ϊ��ʾLED�ź�0��1�Ĳ���
	Decimal_TO_Binary(LED_colour_R,0);
	Decimal_TO_Binary(LED_colour_B,1);
	Decimal_TO_Binary(LED_colour_G,2);

	for(int i=0;i<8;i++)
	{
		LED_OUT[LED_PUT_SET][LED_PUT_number][i]=GREEN[i];
	}

	for(int i=8;i<16;i++)
	{
		LED_OUT[LED_PUT_SET][LED_PUT_number][i]=RED[i-8];
	}

	for(int i=16;i<24;i++)
	{
		LED_OUT[LED_PUT_SET][LED_PUT_number][i]=BLUE[i-16];
	}
}

/**
  * @brief  ʹ�ó�����ɫ���õ�һLED��ɫ��
  * @param  LED_PUT_SET��LED���ڵ���
  * @param  LED_PUT_number��LED���ڵ�λ
  * @param  LED_colour��LED����ɫ
  * @retval ��ֵ
  */
void WS2812_PUT_ONE_LED_colour(int LED_PUT_SET,int LED_PUT_number,int LED_colour[3])
{
	//��ʮ����ת��Ϊ��ʾLED�ź�0��1�Ĳ���
	Decimal_TO_Binary(LED_colour[0],0);
	Decimal_TO_Binary(LED_colour[1],1);
	Decimal_TO_Binary(LED_colour[2],2);

	for(int i=0;i<8;i++)
	{
		LED_OUT[LED_PUT_SET][LED_PUT_number][i]=GREEN[i];
	}

	for(int i=8;i<16;i++)
	{
		LED_OUT[LED_PUT_SET][LED_PUT_number][i]=RED[i-8];
	}

	for(int i=16;i<24;i++)
	{
		LED_OUT[LED_PUT_SET][LED_PUT_number][i]=BLUE[i-16];
	}
}

/**
  * @brief  ��������LED��ɫ��
  * @param  LED_PUT_SET��LED���ڵ���
  * @param  LED_colour_R��LED��ɫ�ĺ�
  * @param  LED_colour_B��LED��ɫ����
  * @param  LED_colour_G��LED��ɫ����
  * @retval ��ֵ
  */
void WS2812_PUT_SET_LED(int LED_PUT_SET,int LED_colour_R,int LED_colour_B,int LED_colour_G)
{
	//��ʮ����ת��Ϊ��ʾLED�ź�0��1�Ĳ���
	Decimal_TO_Binary(LED_colour_R,0);
	Decimal_TO_Binary(LED_colour_B,1);
	Decimal_TO_Binary(LED_colour_G,2);
	for(int a=0;a<LED_number;a++)
	{
	for(int i=0;i<8;i++)
	{
		LED_OUT[LED_PUT_SET][a][i]=GREEN[i];
	}

	for(int i=8;i<16;i++)
	{
		LED_OUT[LED_PUT_SET][a][i]=RED[i-8];
	}

	for(int i=16;i<24;i++)
	{
		LED_OUT[LED_PUT_SET][a][i]=BLUE[i-16];
	}
	}
}
/**
  * @brief  ʹ�ó�����ɫ��������LED��ɫ��
  * @param  LED_PUT_SET��LED���ڵ���
  * @param  LED_PUT_number��LED���ڵ�λ
  * @param  LED_colour��LED����ɫ
  * @retval ��ֵ
  */
void WS2812_PUT_SET_LED_colour(int LED_PUT_SET,int LED_colour[3])
{
	//��ʮ����ת��Ϊ��ʾLED�ź�0��1�Ĳ���
	Decimal_TO_Binary(LED_colour[0],0);
	Decimal_TO_Binary(LED_colour[1],1);
	Decimal_TO_Binary(LED_colour[2],2);

	for(int a=0;a<LED_number;a++)
	{
	for(int i=0;i<8;i++)
	{
		LED_OUT[LED_PUT_SET][a][i]=GREEN[i];
	}

	for(int i=8;i<16;i++)
	{
		LED_OUT[LED_PUT_SET][a][i]=RED[i-8];
	}

	for(int i=16;i<24;i++)
	{
		LED_OUT[LED_PUT_SET][a][i]=BLUE[i-16];
	}
	}
}

/**
  * @brief  ���͵�������
  * @param  SET��LED����
  * @retval ��ֵ
  */
void ws2812_send_ONE(int SET)//��������
{	
	ws2812_send(SET);
}

/**
  * @brief  ����LED��ˢ����
  * @param  HZ��ˢ����
  * @retval ��ֵ
  */
void ws2812_SET_HZ(int HZ)
{	
	WS2812_1.led_Hz=HZ;
}
/**
  * @brief  ����LED������
  * @param  Bri������
  * @retval ��ֵ
  */
void ws2812_SET_Bri(int Bri)
{	
	WS2812_1.led_Bri=Bri;
}