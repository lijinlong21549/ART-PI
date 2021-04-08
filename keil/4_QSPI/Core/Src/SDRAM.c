
#include "main.h"
#include "stdio.h"
#include "SDRAM.h"
/*
void SDRAM_Init(void)
{
    uint32_t temp;
    
    // 1. 时钟使能命令 
    SDRAM_SendCommand(FMC_SDRAM_CMD_CLK_ENABLE, 1, 1, 0);
    
    // 2. 延时，至少100us 
    HAL_Delay(1);
    
    // 3. SDRAM全部预充电命令 
    SDRAM_SendCommand(FMC_SDRAM_CMD_PALL, 1, 1, 0);
    
    // 4. 自动刷新命令 
    SDRAM_SendCommand(FMC_SDRAM_CMD_AUTOREFRESH_MODE, 1, 8, 0);
    
    // 5. 配置SDRAM模式寄存器    
    temp = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1            |          //设置突发长度：1
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL     |          //设置突发类型：连续
                     SDRAM_MODEREG_CAS_LATENCY_3             |          //设置CL值：3
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD   |          //设置操作模式：标准
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;              //设置突发写模式：单点访问  
    SDRAM_SendCommand(FMC_SDRAM_CMD_LOAD_MODE, 1, 1, temp);
    
    // 6. 设置自刷新频率 
    //
    //  SDRAM refresh period / Number of rows）*SDRAM时钟速度 – 20
    // = 64000(64 ms) / 4096 *108MHz - 20
    //= 1667.5 取值1668
    //
    //HAL_SDRAM_ProgramRefreshRate(&hsdram1, 1668);
    HAL_SDRAM_ProgramRefreshRate(&hsdram1, 677);
		HAL_Delay(10);
}
*/

static FMC_SDRAM_CommandTypeDef Command;

#define REFRESH_COUNT        677

#define SDRAM_TIMEOUT                            ((uint32_t)0xFFFF)
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200) 


void SDRAM_Init(void)
{
  __IO uint32_t tmpmrd = 0;
  
  /* Step 1: Configure a clock configuration enable command */
  Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget          =  FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */ 
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);
    
  /* Step 3: Configure a PALL (precharge all) command */ 
  Command.CommandMode            = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);  
  
  /* Step 4: Configure an Auto Refresh command */ 
  Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 4;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);
  
  /* Step 5: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_4          |\
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |\
                     SDRAM_MODEREG_CAS_LATENCY_2           |\
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |\
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);
  
  /* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(&hsdram1, REFRESH_COUNT); 
}
//64000/8192*120-20