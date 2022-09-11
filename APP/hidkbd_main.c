/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : ��������Ӧ��������������ϵͳ��ʼ��
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "CONFIG.h"
#include "HAL.h"
#include "printf.h"
#include "hiddev.h"
#include "hidkbd.h"

volatile uint8_t GPIOTigFlag = 0;

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if (defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02};
#endif

/*********************************************************************
 * @fn      Main_Circulation
 *
 * @brief   ��ѭ��
 *
 * @return  none
 */
__HIGH_CODE
__attribute__((noinline)) void Main_Circulation()
{
    // FLASH_ROM_SW_RESET();
    // R8_FLASH_CTRL = 0x04; // flash�ر�
    while (1)
    {
        TMOS_SystemProcess();
    }
}

__INTERRUPT __HIGH_CODE void GPIOA_IRQHandler()
{
    R16_PA_INT_IF = R16_PA_INT_IF;
    GPIOTigFlag = 1;
}

__INTERRUPT __HIGH_CODE void GPIOB_IRQHandler()
{
    R16_PB_INT_IF = R16_PB_INT_IF;
    GPIOTigFlag = 1;
}

/*********************************************************************
 * @fn      main
 *
 * @brief   ������
 *
 * @return  none
 */
int main(void)
{
#if (defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
#if (defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif
#ifdef DEBUG
    GPIOA_SetBits(bTXD1);
    GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
#endif
    PRINT("%s\n", VER_LIB);
    CH58X_BLEInit();
    HAL_Init();

    GPIOB_SetBits(GPIO_Pin_9);
    GPIOB_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    GPIOA_SetBits(GPIO_Pin_7);
    GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA);
    GPIOB_SetBits(GPIO_Pin_8);
    GPIOB_ModeCfg(GPIO_Pin_8, GPIO_ModeOut_PP_5mA);
    GPIOB_SetBits(GPIO_Pin_16);
    GPIOB_ModeCfg(GPIO_Pin_16, GPIO_ModeOut_PP_5mA);
    GPIOB_SetBits(GPIO_Pin_18);
    GPIOB_ModeCfg(GPIO_Pin_18, GPIO_ModeOut_PP_5mA);

    GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PD);
    GPIOA_ITModeCfg(GPIO_Pin_4, GPIO_ITMode_RiseEdge);
    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeIN_PD);
    GPIOA_ITModeCfg(GPIO_Pin_5, GPIO_ITMode_RiseEdge);
    GPIOB_ResetBits(GPIO_Pin_0);
    GPIOB_ModeCfg(GPIO_Pin_0, GPIO_ModeOut_PP_5mA);

    sys_safe_access_enable();
    R8_SLP_WAKE_CTRL |= RB_WAKE_EV_MODE | RB_SLP_GPIO_WAKE;
    sys_safe_access_enable();
    R8_SLP_POWER_CTRL &= ~(RB_WAKE_DLY_MOD);
    sys_safe_access_enable();
    R8_SLP_POWER_CTRL |= 0x01;

    PFIC_EnableIRQ(GPIO_A_IRQn);
    PFIC_EnableIRQ(GPIO_B_IRQn);

    GAPRole_PeripheralInit();
    HidDev_Init();
    HidEmu_Init();
    Main_Circulation();
}

/******************************** endfile @ main ******************************/
