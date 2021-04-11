#ifndef __W25Q64_QSPI_H__
#define __W25Q64_QSPI_H__

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "stdio.h"
#include "W25Q64_QSPI.h"
#include "quadspi.h"

  /*�ײ㺯��*/
  /************************************************************************************************************************************************************************************************************/
  extern QSPI_HandleTypeDef hqspi;

  static void W25Qx_QSPI_Delay(uint32_t ms);
  static uint8_t W25Qx_QSPI_Addr_Mode_Init(void);
  static uint8_t W25Qx_QSPI_ResetMemory (void);
  static uint8_t W25Qx_QSPI_WriteEnable (void);
  static uint8_t W25Qx_QSPI_AutoPollingMemReady  (uint32_t Timeout);

  /**
   * @brief  ��ʼ��QSPI�洢��
   * @retval QSPI�洢��״̬
   */
  uint8_t W25Qx_QSPI_Init(void)
  {
    QSPI_CommandTypeDef s_command;
    uint8_t value = W25QxJV_FSR_QE;

    /* QSPI�洢����λ */
    if (W25Qx_QSPI_ResetMemory() != QSPI_OK)
      {
        return QSPI_NOT_SUPPORTED;
      }

    /* ʹ��д���� */
    if (W25Qx_QSPI_WriteEnable() != QSPI_OK)
      {
        return QSPI_ERROR;
      }
    /* ������·ʹ�ܵ�״̬�Ĵ�����ʹ����ͨ��IO2��IO3���� */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = WRITE_STATUS_REG2_CMD;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.DummyCycles = 0;
    s_command.NbData = 1;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    /* �������� */
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }
    /* �������� */
    if (HAL_QSPI_Transmit(&hqspi, &value, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }
    /* �Զ���ѯģʽ�ȴ��洢������ */
    if (W25Qx_QSPI_AutoPollingMemReady(W25QxJV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
      {
        return QSPI_ERROR;
      }





    //printf("QSPI��ʼ���ɹ�\r\n");
    uint32_t test_ID=W25Qx_QSPI_FLASH_ReadDeviceID();
    printf("ʶ��QSPI-Flash:");

    if(test_ID == 0x0000EF13)
      {
        printf("W25Q80\r\n");
      }
    else if(test_ID == 0x0000EF14)
      {
        printf("W25Q16\r\n");
      }
    else if(test_ID == 0x0000EF15)
      {
        printf("W25Q32\r\n");
      }
    else if(test_ID == 0x0000EF16)
      {
        printf("W25Q64\r\n");
      }
    else if(test_ID == 0x0000EF17)
      {
        printf("W25Q128\r\n");
      }

    /* ���õ�ַģʽΪ 4 �ֽ� , ��W25Q256ֱ������*/
    if (sFLASH_ID != 0XEF4019)
      return QSPI_OK;

    if (W25Qx_QSPI_Addr_Mode_Init() != QSPI_OK)
      {
        return QSPI_ERROR;
      }


    return QSPI_OK;
  }

  /**
   * @brief  ����ַģʽ����4�ֽڵ�ַ������Ϊ4�ֽ�
   * @retval QSPI�洢��״̬
   */
  static uint8_t W25Qx_QSPI_Addr_Mode_Init(void)
  {
    QSPI_CommandTypeDef s_command;
    uint8_t reg;
    /* ��ʼ����ȡ״̬�Ĵ������� */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = READ_STATUS_REG3_CMD;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.DummyCycles = 0;
    s_command.NbData = 1;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* �������� */
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }
    /* �������� */
    if (HAL_QSPI_Receive(&hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }

    /* ���Ĵ�����ֵ */
    if ((reg & W25Q256FV_FSR_4ByteAddrMode) == 1)    // 4�ֽ�ģʽ
      {
        return QSPI_OK;
      }
    else    // 3�ֽ�ģʽ
      {
        /* ���ý��� 4 �ֽڵ�ַģʽ���� */
        s_command.Instruction = Enter_4Byte_Addr_Mode_CMD;
        s_command.DataMode = QSPI_DATA_NONE;

        /* ���ò��������� */
        if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
            != HAL_OK)
          {
            return QSPI_ERROR;
          }

        /* �Զ���ѯģʽ�ȴ��洢������ */
        if (W25Qx_QSPI_AutoPollingMemReady(
              W25QxJV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
          {
            return QSPI_ERROR;
          }

        return QSPI_OK;
      }
  }

  /**
   * @brief  ��QSPI�洢���ж�ȡ��������.
   * @param  pData: ָ��Ҫ��ȡ�����ݵ�ָ��
   * @param  ReadAddr: ��ȡ��ʼ��ַ
   * @param  Size: Ҫ��ȡ�����ݴ�С
   * @retval QSPI�洢��״̬
   */
  uint8_t W25Qx_QSPI_FastRead(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
  {
    QSPI_CommandTypeDef s_command;

    if(Size == 0)	return QSPI_OK;

    /* ��ʼ�������� */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = QUAD_INOUT_FAST_READ_CMD;
    s_command.AddressMode = QSPI_ADDRESS_4_LINES;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.Address = ReadAddr;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_4_LINES;
    s_command.DummyCycles = 0;
    s_command.NbData = Size;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* �������� */
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }

    /* �������� */
    if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }
    return QSPI_OK;
  }

  /**
   * @brief  ��QSPI�洢���ж�ȡ��������.
   * @note   ��ָ��ֻ��ʹ����50MHzһ�£��������²�����
   * @param  pData: ָ��Ҫ��ȡ�����ݵ�ָ��
   * @param  ReadAddr: ��ȡ��ʼ��ַ
   * @param  Size: Ҫ��ȡ�����ݴ�С
   * @retval QSPI�洢��״̬
   */
  uint8_t W25Qx_QSPI_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
  {
    QSPI_CommandTypeDef s_command;
    /* ��ʼ�������� */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = READ_CMD;    //READ_CMD;
    s_command.AddressMode = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.Address = ReadAddr;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.DummyCycles = 0;
    s_command.NbData = Size;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* �������� */
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }

    /* �������� */
    if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }
    return QSPI_OK;
  }

  /**
   * @brief  ����������д��QSPI�洢��
   * @param  pData: ָ��Ҫд�����ݵ�ָ��
   * @param  WriteAddr: д��ʼ��ַ
   * @param  Size: Ҫд������ݴ�С
   * @retval QSPI�洢��״̬
   */
  uint8_t W25Qx_QSPI_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
  {
    QSPI_CommandTypeDef s_command;
    uint32_t end_addr, current_size, current_addr;
    /* ����д���ַ��ҳ��ĩβ֮��Ĵ�С */
    current_addr = 0;

    while (current_addr <= WriteAddr)
      {
        current_addr += W25QxJV_PAGE_SIZE;
      }
    current_size = current_addr - WriteAddr;

    /* ������ݵĴ�С�Ƿ�С��ҳ���е�ʣ��λ�� */
    if (current_size > Size)
      {
        current_size = Size;
      }

    /* ��ʼ����ַ���� */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;

    /* ��ʼ���������� */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = QUAD_INPUT_PAGE_PROG_CMD;
    s_command.AddressMode = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_4_LINES;
    s_command.DummyCycles = 0;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* ��ҳִ��д�� */
    do
      {
        s_command.Address = current_addr;
        s_command.NbData = current_size;

        /* ����д���� */
        if (W25Qx_QSPI_WriteEnable() != QSPI_OK)
          {
            return QSPI_ERROR;
          }

        /* �������� */
        if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
            != HAL_OK)
          {
            return QSPI_ERROR;
          }

        /* �������� */
        if (HAL_QSPI_Transmit(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
            != HAL_OK)
          {
            return QSPI_ERROR;
          }

        /* �����Զ���ѯģʽ�ȴ�������� */
        if (W25Qx_QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
          {
            return QSPI_ERROR;
          }

        /* ������һҳ��̵ĵ�ַ�ʹ�С���� */
        current_addr += current_size;
        pData += current_size;
        current_size =
          ((current_addr + W25QxJV_PAGE_SIZE) > end_addr) ?
          (end_addr - current_addr) : W25QxJV_PAGE_SIZE;
      }
    while (current_addr < end_addr);
    return QSPI_OK;
  }

  /**
   * @brief  ����QSPI�洢����ָ����
   * @param  BlockAddress: ��Ҫ�����Ŀ��ַ
   * @retval QSPI�洢��״̬
   */
  uint8_t W25Qx_QSPI_Erase_Block(uint32_t BlockAddress)
  {
    QSPI_CommandTypeDef s_command;
    /* ��ʼ���������� */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = SECTOR_ERASE_CMD;
    s_command.AddressMode = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.Address = BlockAddress;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_NONE;
    s_command.DummyCycles = 0;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* ����д���� */
    if (W25Qx_QSPI_WriteEnable() != QSPI_OK)
      {
        return QSPI_ERROR;
      }

    /* �������� */
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }

    /* �����Զ���ѯģʽ�ȴ��������� */
    if (W25Qx_QSPI_AutoPollingMemReady(W25QxJV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
      {
        return QSPI_ERROR;
      }
    return QSPI_OK;
  }

  /**
   * @brief  ��������QSPI�洢��
   * @retval QSPI�洢��״̬
   */
  uint8_t W25Qx_QSPI_Erase_Chip(void)
  {
    QSPI_CommandTypeDef s_command;
    /* ��ʼ���������� */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = CHIP_ERASE_CMD;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_NONE;
    s_command.DummyCycles = 0;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* ����д���� */
    if (W25Qx_QSPI_WriteEnable() != QSPI_OK)
      {
        return QSPI_ERROR;
      }
    /* �������� */
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }
    /* �����Զ���ѯģʽ�ȴ��������� */
    if (W25Qx_QSPI_AutoPollingMemReady(W25QxJV_BULK_ERASE_MAX_TIME) != QSPI_OK)
      {
        return QSPI_ERROR;
      }
    return QSPI_OK;
  }

  /**
   * @brief  ��ȡQSPI�洢���ĵ�ǰ״̬
   * @retval QSPI�洢��״̬
   */
  uint8_t W25Qx_QSPI_GetStatus(void)
  {
    QSPI_CommandTypeDef s_command;
    uint8_t reg;
    /* ��ʼ����ȡ״̬�Ĵ������� */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = READ_STATUS_REG1_CMD;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.DummyCycles = 0;
    s_command.NbData = 1;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* �������� */
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }
    /* �������� */
    if (HAL_QSPI_Receive(&hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }
    /* ���Ĵ�����ֵ */
    if ((reg & W25QxJV_FSR_BUSY) != 0)
      {
        return QSPI_BUSY;
      }
    else
      {
        return QSPI_OK;
      }
  }

  /**
   * @brief  ����QSPI�洢��������
   * @param  pInfo: �����ýṹ�ϵ�ָ��
   * @retval QSPI�洢��״̬
   */
  uint8_t W25Qx_QSPI_GetInfo(QSPI_Info *pInfo)
  {
    /* ���ô洢�����ýṹ */
    pInfo->FlashSize = W25QxJV_FLASH_SIZE;
    pInfo->EraseSectorSize = W25QxJV_SUBSECTOR_SIZE;
    pInfo->EraseSectorsNumber = (W25QxJV_FLASH_SIZE / W25QxJV_SUBSECTOR_SIZE);
    pInfo->ProgPageSize = W25QxJV_PAGE_SIZE;
    pInfo->ProgPagesNumber = (W25QxJV_FLASH_SIZE / W25QxJV_PAGE_SIZE);
    return QSPI_OK;
  }

  /**
   * @brief  ��λQSPI�洢����
   * @param  hqspi: QSPI���
   * @retval ��
   */
  static uint8_t W25Qx_QSPI_ResetMemory()
  {
    QSPI_CommandTypeDef s_command;
    /* ��ʼ����λʹ������ */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = RESET_ENABLE_CMD;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_NONE;
    s_command.DummyCycles = 0;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* �������� */
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
        return QSPI_ERROR;
      }

    /* ���͸�λ�洢������ */
    s_command.Instruction = RESET_MEMORY_CMD;
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
        return QSPI_ERROR;
      }

    s_command.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    s_command.Instruction = RESET_ENABLE_CMD;

    /* �������� */
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
        return QSPI_ERROR;
      }

    /* ���͸�λ�洢������ */
    s_command.Instruction = RESET_MEMORY_CMD;
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
        return QSPI_ERROR;
      }

    W25Qx_QSPI_Delay(1);

    /* �����Զ���ѯģʽ�ȴ��洢������ */
    if (W25Qx_QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
      {
        return QSPI_ERROR;
      }
    return QSPI_OK;
  }

  /**
   * @brief  ����д��ʹ�ܣ��ȴ�����Ч.
   * @param  hqspi: QSPI���
   * @retval ��
   */
  static uint8_t W25Qx_QSPI_WriteEnable()
  {
    QSPI_CommandTypeDef s_command;
    QSPI_AutoPollingTypeDef s_config;
    /* ����д���� */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = WRITE_ENABLE_CMD;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_NONE;
    s_command.DummyCycles = 0;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        return QSPI_ERROR;
      }

    /* �����Զ���ѯģʽ�ȴ�д���� */
    s_config.Match = W25QxJV_FSR_WREN;
    s_config.Mask = W25QxJV_FSR_WREN;
    s_config.MatchMode = QSPI_MATCH_MODE_AND;
    s_config.StatusBytesSize = 1;
    s_config.Interval = 0x10;
    s_config.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    s_command.Instruction = READ_STATUS_REG1_CMD;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.NbData = 1;

    if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config,
                             HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
        return QSPI_ERROR;
      }
    return QSPI_OK;
  }

  /**
   * @brief  ��ȡ�洢����SR���ȴ�EOP
   * @param  hqspi: QSPI���
   * @param  Timeout ��ʱ
   * @retval ��
   */
  static uint8_t W25Qx_QSPI_AutoPollingMemReady(uint32_t Timeout)
  {
    QSPI_CommandTypeDef s_command;
    QSPI_AutoPollingTypeDef s_config;
    /* �����Զ���ѯģʽ�ȴ��洢��׼������ */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = READ_STATUS_REG1_CMD;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.DummyCycles = 0;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    s_config.Match = 0x00;
    s_config.Mask = W25QxJV_FSR_BUSY;
    s_config.MatchMode = QSPI_MATCH_MODE_AND;
    s_config.StatusBytesSize = 1;
    s_config.Interval = 0x10;
    s_config.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, Timeout) != HAL_OK)
      {
        return QSPI_ERROR;
      }
    return QSPI_OK;
  }

  /**
   * @brief  ��ȡFLASH ID
   * @param 	��
   * @retval FLASH ID
   */
  uint32_t W25Qx_QSPI_FLASH_ReadID(void)
  {
    QSPI_CommandTypeDef s_command;
    uint32_t Temp = 0;
    uint8_t pData[3];
    /* ��ȡJEDEC ID */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = READ_JEDEC_ID_CMD;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DummyCycles = 0;
    s_command.NbData = 3;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        printf("QSPI_FLASH_ReadID ERROR!!!....\r\n");
        /* �û��������������һЩ����������������� */
        while (1)
          {

          }
      }
    if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
        != HAL_OK)
      {
        printf("QSPI_FLASH_ReadID ERROR!!!....\r\n");
        /* �û��������������һЩ����������������� */
        while (1)
          {

          }
      }

    Temp = (pData[2] | pData[1] << 8) | (pData[0] << 16);

    return Temp;
  }

  /**
   * @brief  ��ȡFLASH Device ID
   * @param 	��
   * @retval FLASH Device ID
   */
  uint32_t W25Qx_QSPI_FLASH_ReadDeviceID(void)
  {
    QSPI_CommandTypeDef s_command;
    uint32_t Temp = 0;
    uint8_t pData[3];
    /*##-2-��ȡ�豸ID����    ###########################################*/
    /* ��ȡ����/�豸 ID */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction = READ_ID_CMD;
    s_command.AddressMode = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.Address = 0x000000;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.DummyCycles = 0;
    s_command.NbData = 2;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
        printf("QSPI_FLASH_ReadDeviceID ERROR!!!....\r\n");
        /* �û��������������һЩ����������������� */
        while (1)
          {

          }
      }
    if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)	!= HAL_OK)
      {
        printf("QSPI_FLASH_ReadDeviceID ERROR!!! ....\r\n");
        /* �û��������������һЩ����������������� */
        while (1)
          {

          }
      }

    Temp = pData[1] | (pData[0] << 8);

    return Temp;
  }

  static void W25Qx_QSPI_Delay(uint32_t ms)
  {
    HAL_Delay(ms);
  }



#ifdef __cplusplus
}
#endif

#endif /* __W25Q64_H__ */
