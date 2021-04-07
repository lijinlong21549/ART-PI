//头函数
#include "WS2812.h"
//#include "main.h"
#include "tim.h"

/*时钟参数
占空比 = （Pulse/Period）*100%；频率: f = 48M/Prescaler/Period
1信号占空比 = （156/312）=50%；即Pulse=156-1
0信号占空比 = （84/312）=26.9%；即Pulse=84-1
频率：f = 240/1/312= 0.769230MHZ
*/
#define LED_SET          16
#define LED_number       8
#define N                LED_number*24
//#define EXT_SDRAM_ADDR  	((uint32_t)0xC0000000)
//#define EXT_SDRAM_SIZE		(32 * 1024 * 1024)
uint32_t LED_OUT_test[2]={0,0};
uint32_t LED_OUT[LED_SET][LED_number][24]; //__attribute__((at(EXT_SDRAM_ADDR))) ;//定义LED的三维数组[LED组][LED的个数][LED的24个参数]
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
	
	int led_num;//LED连接的数量；1-2048
	int led_Bri;//LED显示的亮度；0-100	
	int led_Hz;//LED的刷新率；1-60Hz
	int led_HIGH;//LED的高电平占空比
	int led_LOW;//LED的低电平占空比
	int AUTO_send;//自动发送
	int AUTO_send_SET;//执行自动发送的组
	uint32_t led_GPIO;//LED的PWM发送GPIO
	uint32_t led_pin;//LED的PWM发送引脚
	uint32_t LED_DMA;//LED的PWM所占用的DMA

	int led_PWM_1_Duty_cycle;//LED的PWM的1信号的占空比
	int led_PWM_0_Duty_cycle;//LED的PWM的0信号的占空比

	
}WS2812_1;

/**
  * @brief  发送单组脉冲
  * @param  SET：LED的组
  * @retval 空值
  */
void ws2812_send(int SET)//发送脉冲
{	
	/*
	//HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_1,(uint32_t *)OUT1,50);
	HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_1,(uint32_t *)LED_OUT_test,2);//先发两个低信号，稳定DMA
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

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)// DMA 传输完成回调函数
{
    HAL_TIM_PWM_Stop_DMA(&htim2,TIM_CHANNEL_1);
}



/*
函数：Decimal_TO_Binary(int DATA,int DATA_BAT)
功能：将十进制数据转为以30、66对于的二进制
参数：
DATA：需要转换的函数
RGB_colour:选择转换哪种RGB
*/
void Decimal_TO_Binary(int DATA,int RGB_colour)
	{
	//用于返回的数组	
	uint32_t OUT_DATA[8];

	//先将OUT_DATA填充0字符
	for(int i=0;i<8;i++)
	{
		OUT_DATA[i]=33;
	}

	//创建一个用于while循环的DATA
	int W_DATA=(DATA*WS2812_1.led_Bri)/100;//适配亮度设置
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
	//倒序输出部分可以删去
	//将倒序的数据对照转换为30、66的01码，反序转换到对于数组

}


//可调用函数

/*
函数：ws2812_send
功能：发送N
参数：
*/



/**
  * @brief  将某一组LED熄灭
  * @param  SET：LED的组
  * @retval 空值
  */
void LED_Cleared_SET(int SET)
{
	//LED位循环
	for(int number=0;number<LED_number;number++)
	{
	//LED RBG循环
	for(int RBG=0;RBG<24;RBG++)
	{
		LED_OUT[SET][number][RBG]=WS2812_1.led_PWM_0_Duty_cycle;//所有LED值等于零信号参数
	}
	}
	
}

/**
 * @brief	将所有led全部熄灭
*/
void WS2812_OLL_slake(void)
{
	for(int i=0;i<LED_SET;i++)
	{
		LED_Cleared_SET(i);
	}
	ws2812_send(1);//发送
}


/**
 * @brief	WS2812初始化
*/
void WS2812_Init(void)//初始化WS2812
{

	WS2812_1.led_num=LED_number;//LED连接的数量；1-2000
	WS2812_1.led_Bri=100;//LED显示的亮度；0-100
	WS2812_1.led_Hz=60;//LED的刷新率；1-60Hz
	WS2812_1.led_HIGH=50;//LED的高电平占空比
	WS2812_1.led_LOW=27;//LED的低电平占空比
	WS2812_1.led_GPIO='A';//LED的PWM发送引脚GPIO
	WS2812_1.led_pin='1';//LED的PWM发送引脚
	WS2812_1.LED_DMA='1';//LED的PWM所占用的DMA
	WS2812_1.AUTO_send=0;//关闭自动发送
	WS2812_1.AUTO_send_SET=0;//执行自动发送的组为0
	WS2812_1.led_PWM_1_Duty_cycle=39;//LED的PWM的1信号的占空比
	WS2812_1.led_PWM_0_Duty_cycle=21;//LED的PWM的0信号的占空比

	WS2812_OLL_slake();//熄灭所有LED灯
}

/**
  * @brief  开启自动发送。
  * @retval 空值
  */
void WS2812_AUTO_send_ON(void)
{
	WS2812_1.AUTO_send=1;//开启自动发送
}
/**
  * @brief  关闭自动发送。
  * @retval 空值
  */
void WS2812_AUTO_send_OFF(void)
{
	WS2812_1.AUTO_send=0;//关闭自动发送
}


/**
  * @brief  返回WS2812参数中的刷新率参数。
  * @retval WS2812刷新率
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
  * @brief  设置单一LED颜色。
  * @param  LED_PUT_SET：LED所在的组
  * @param  LED_PUT_number：LED所在的位
  * @param  LED_colour_R：LED颜色的红
  * @param  LED_colour_B：LED颜色的蓝
  * @param  LED_colour_G：LED颜色的绿
  * @retval 空值
  */
void WS2812_PUT_ONE_LED(int LED_PUT_SET,int LED_PUT_number,int LED_colour_R,int LED_colour_B,int LED_colour_G )
{
	//将十进制转化为表示LED信号0、1的参数
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
  * @brief  使用常用颜色设置单一LED颜色。
  * @param  LED_PUT_SET：LED所在的组
  * @param  LED_PUT_number：LED所在的位
  * @param  LED_colour：LED的颜色
  * @retval 空值
  */
void WS2812_PUT_ONE_LED_colour(int LED_PUT_SET,int LED_PUT_number,int LED_colour[3])
{
	//将十进制转化为表示LED信号0、1的参数
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
  * @brief  设置整组LED颜色。
  * @param  LED_PUT_SET：LED所在的组
  * @param  LED_colour_R：LED颜色的红
  * @param  LED_colour_B：LED颜色的蓝
  * @param  LED_colour_G：LED颜色的绿
  * @retval 空值
  */
void WS2812_PUT_SET_LED(int LED_PUT_SET,int LED_colour_R,int LED_colour_B,int LED_colour_G)
{
	//将十进制转化为表示LED信号0、1的参数
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
  * @brief  使用常用颜色设置整组LED颜色。
  * @param  LED_PUT_SET：LED所在的组
  * @param  LED_PUT_number：LED所在的位
  * @param  LED_colour：LED的颜色
  * @retval 空值
  */
void WS2812_PUT_SET_LED_colour(int LED_PUT_SET,int LED_colour[3])
{
	//将十进制转化为表示LED信号0、1的参数
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
  * @brief  发送单组脉冲
  * @param  SET：LED的组
  * @retval 空值
  */
void ws2812_send_ONE(int SET)//发送脉冲
{	
	ws2812_send(SET);
}

/**
  * @brief  设置LED的刷新率
  * @param  HZ：刷新率
  * @retval 空值
  */
void ws2812_SET_HZ(int HZ)
{	
	WS2812_1.led_Hz=HZ;
}
/**
  * @brief  设置LED的亮度
  * @param  Bri：亮度
  * @retval 空值
  */
void ws2812_SET_Bri(int Bri)
{	
	WS2812_1.led_Bri=Bri;
}