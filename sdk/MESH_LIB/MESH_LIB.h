/********************************** (C) COPYRIGHT *******************************
 * File Name          : mesh_lib.h
 * Author             : WCH
 * Version            : V1.70
 * Date               : 2022/10/26
 * Description        :
 * Copyright (c) 2023 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef mesh_lib_H
#define mesh_lib_H

#ifdef __cplusplus
extern "C"
{
#endif

#pragma anon_unions

#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "string.h"
#include "app_mesh_config.h"

/******************************************************************************/

#ifndef int8
typedef signed char int8;
#endif
#ifndef int16
typedef short int16;
#endif
#ifndef BOOL
typedef unsigned char BOOL;
#endif
#ifndef s8
typedef signed char s8;
#endif
#ifndef s16
typedef signed short s16;
#endif
#ifndef s32
typedef signed long s32;
#endif
#ifndef u8
typedef unsigned char u8;
#endif
#ifndef u16
typedef unsigned short u16;
#endif
#ifndef u32
typedef unsigned long u32;
#endif
#ifndef u64
typedef unsigned long long u64;
#endif
#ifndef u8C
typedef const unsigned char u8C;
#endif
#ifndef u32V
typedef unsigned long volatile u32V;
#endif
#ifndef uint8
typedef unsigned char uint8;
#endif
#ifndef uint16
typedef unsigned short uint16;
#endif
#ifndef uint32
typedef unsigned long uint32;
#endif
#ifndef s8_t
typedef signed char s8_t;
#endif
#ifndef s16_t
typedef signed short s16_t;
#endif
#ifndef s32_t
typedef signed long s32_t;
#endif
#ifndef s64_t
typedef signed long long s64_t;
#endif
#ifndef u8_t
typedef unsigned char u8_t;
#endif
#ifndef u16_t
typedef unsigned short u16_t;
#endif
#ifndef u32_t
typedef unsigned long u32_t;
#endif
#ifndef u64_t
typedef unsigned long long u64_t;
#endif
#ifndef bt_mesh_atomic_t
typedef unsigned int bt_mesh_atomic_t;
#endif
#ifndef ssize_t
typedef signed int ssize_t;
#endif

#ifndef TRUE
#define TRUE                1
#endif
#ifndef FALSE
#define FALSE               0
#endif
#ifndef NULL
#define NULL                0
#endif
#ifndef VOID
#define VOID                (void)
#endif
#ifndef  CONST
#define CONST               const
#endif

/******************************************************************************/
#define  VER_MESH_FILE            "MESH_LIB_V1.7"
extern const uint8_t VER_MESH_LIB[];

// Global_Error_Code
#define MESH_ERROR_BASE_NUM      	(0x0)       ///< Global error base

#define NOERR                       (MESH_ERROR_BASE_NUM + 0)
#define EINVAL                      (MESH_ERROR_BASE_NUM + 1)
#define EALREADY                    (MESH_ERROR_BASE_NUM + 2)
#define ESRCH                       (MESH_ERROR_BASE_NUM + 3)
#define EBUSY                       (MESH_ERROR_BASE_NUM + 4)
#define ENOTCONN                    (MESH_ERROR_BASE_NUM + 5)
#define EAGAIN                      (MESH_ERROR_BASE_NUM + 6)
#define ENOBUFS                     (MESH_ERROR_BASE_NUM + 7)
#define ENOENT                      (MESH_ERROR_BASE_NUM + 8)
#define ENOMEM                      (MESH_ERROR_BASE_NUM + 9)
#define EEXIST                      (MESH_ERROR_BASE_NUM + 10)
#define EIO                         (MESH_ERROR_BASE_NUM + 11)
#define EDEADLK                     (MESH_ERROR_BASE_NUM + 12)
#define ESPIPE                      (MESH_ERROR_BASE_NUM + 13)
#define EACCES                      (MESH_ERROR_BASE_NUM + 14)
#define ENXIO                       (MESH_ERROR_BASE_NUM + 15)
#define ENOSPC                      (MESH_ERROR_BASE_NUM + 16)
#define EBADMSG                     (MESH_ERROR_BASE_NUM + 17)
#define E2BIG                       (MESH_ERROR_BASE_NUM + 18)
#define ENOTSUP                     (MESH_ERROR_BASE_NUM + 19)
#define EADDRINUSE                  (MESH_ERROR_BASE_NUM + 20)
#define EMSGSIZE                    (MESH_ERROR_BASE_NUM + 21)
#define ECANCELED                   (MESH_ERROR_BASE_NUM + 22)
#define ETIMEDOUT                   (MESH_ERROR_BASE_NUM + 23)
#define EADDRNOTAVAIL               (MESH_ERROR_BASE_NUM + 24)

/* Primary Network Key index */
#define BLE_MESH_NET_PRIMARY 0x000

/* Function_State_Define */
#define BLE_MESH_RELAY_DISABLED 0x00
#define BLE_MESH_RELAY_ENABLED 0x01
#define BLE_MESH_RELAY_NOT_SUPPORTED 0x02

#define BLE_MESH_BEACON_DISABLED 0x00
#define BLE_MESH_BEACON_ENABLED 0x01

#define BLE_MESH_GATT_PROXY_DISABLED 0x00
#define BLE_MESH_GATT_PROXY_ENABLED 0x01
#define BLE_MESH_GATT_PROXY_NOT_SUPPORTED 0x02

#define BLE_MESH_FRIEND_DISABLED 0x00
#define BLE_MESH_FRIEND_ENABLED 0x01
#define BLE_MESH_FRIEND_NOT_SUPPORTED 0x02

#define BLE_MESH_NODE_IDENTITY_STOPPED 0x00
#define BLE_MESH_NODE_IDENTITY_RUNNING 0x01
#define BLE_MESH_NODE_IDENTITY_NOT_SUPPORTED 0x02

/* Features */
#define BLE_MESH_FEAT_RELAY BIT(0)
#define BLE_MESH_FEAT_PROXY BIT(1)
#define BLE_MESH_FEAT_FRIEND BIT(2)
#define BLE_MESH_FEAT_LOW_POWER BIT(3)
#define BLE_MESH_FEAT_SUPPORTED (BLE_MESH_FEAT_RELAY |  \
                                 BLE_MESH_FEAT_PROXY |  \
                                 BLE_MESH_FEAT_FRIEND | \
                                 BLE_MESH_FEAT_LOW_POWER)

#define BLE_MESH_SDU_UNSEG_MAX          11    // The user's actual usable length is 7 bytes
#define BLE_MESH_CTL_SEG_SDU_MAX        8
#define BLE_MESH_APP_SEG_SDU_MAX        12
#define BLE_MESH_TX_SDU_MAX            (32 * BLE_MESH_APP_SEG_SDU_MAX)    // The user's actual usable length needs to be reduced by 8 bytes
#define BLE_MESH_RX_SDU_MAX            (32 * BLE_MESH_APP_SEG_SDU_MAX)
#define BLE_MESH_RX_CTL_MAX            (32 * BLE_MESH_CTL_SEG_SDU_MAX)

#define BLE_MESH_KEY_PRIMARY 0x0000
#define BLE_MESH_KEY_ANY     0xffff

#define BLE_MESH_ADDR_IS_UNICAST(addr) ((addr) && (addr) < 0x8000)
#define BLE_MESH_ADDR_IS_GROUP(addr) ((addr) >= 0xc000 && (addr) < 0xff00)
#define BLE_MESH_ADDR_IS_VIRTUAL(addr) ((addr) >= 0x8000 && (addr) < 0xc000)
#define BLE_MESH_ADDR_IS_RFU(addr) ((addr) >= 0xff00 && (addr) <= 0xfffb)

/**************************************ACCESS****************************************/

#define BLE_MESH_ADDR_UNASSIGNED   0x0000
#define BLE_MESH_ADDR_ALL_NODES    0xffff
#define BLE_MESH_ADDR_PROXIES      0xfffc
#define BLE_MESH_ADDR_FRIENDS      0xfffd
#define BLE_MESH_ADDR_RELAYS       0xfffe

#define BLE_MESH_KEY_UNUSED        0xffff
#define BLE_MESH_KEY_DEV           0xfffe
#define BLE_MESH_KEY_DEV_LOCAL     BLE_MESH_KEY_DEV
#define BLE_MESH_KEY_DEV_REMOTE    0xfffd
#define BLE_MESH_KEY_DEV_ANY       0xfffc

#define BLE_MESH_IS_DEV_KEY(key) (key == BLE_MESH_KEY_DEV_LOCAL || \
                                    key == BLE_MESH_KEY_DEV_REMOTE)

/**************************************k_timer****************************************/
#define MSEC_PER_SEC 		1000U
#define K_NO_WAIT       0
#define K_MSEC(ms)      (ms)
#define K_SECONDS(s)    K_MSEC((s) * MSEC_PER_SEC)
#define K_MINUTES(m)    K_SECONDS((m) * 60)
#define K_HOURS(h)      K_MINUTES((h) * 60)
#define K_FOREVER       (-1)

/**************************************MODEL****************************************/

// BLE_MESH_MODEL_OP
#define OP_APP_KEY_ADD                     BLE_MESH_MODEL_OP_1(0x00)
#define OP_APP_KEY_UPDATE                  BLE_MESH_MODEL_OP_1(0x01)
#define OP_DEV_COMP_DATA_STATUS            BLE_MESH_MODEL_OP_1(0x02)
#define OP_MOD_PUB_SET                     BLE_MESH_MODEL_OP_1(0x03)
#define OP_HEALTH_CURRENT_STATUS           BLE_MESH_MODEL_OP_1(0x04)
#define OP_HEALTH_FAULT_STATUS             BLE_MESH_MODEL_OP_1(0x05)
#define OP_HEARTBEAT_PUB_STATUS            BLE_MESH_MODEL_OP_1(0x06)
#define OP_APP_KEY_DEL                     BLE_MESH_MODEL_OP_2(0x80, 0x00)
#define OP_APP_KEY_GET                     BLE_MESH_MODEL_OP_2(0x80, 0x01)
#define OP_APP_KEY_LIST                    BLE_MESH_MODEL_OP_2(0x80, 0x02)
#define OP_APP_KEY_STATUS                  BLE_MESH_MODEL_OP_2(0x80, 0x03)
#define OP_ATTENTION_GET                   BLE_MESH_MODEL_OP_2(0x80, 0x04)
#define OP_ATTENTION_SET                   BLE_MESH_MODEL_OP_2(0x80, 0x05)
#define OP_ATTENTION_SET_UNREL             BLE_MESH_MODEL_OP_2(0x80, 0x06)
#define OP_ATTENTION_STATUS                BLE_MESH_MODEL_OP_2(0x80, 0x07)
#define OP_DEV_COMP_DATA_GET               BLE_MESH_MODEL_OP_2(0x80, 0x08)
#define OP_BEACON_GET                      BLE_MESH_MODEL_OP_2(0x80, 0x09)
#define OP_BEACON_SET                      BLE_MESH_MODEL_OP_2(0x80, 0x0a)
#define OP_BEACON_STATUS                   BLE_MESH_MODEL_OP_2(0x80, 0x0b)
#define OP_DEFAULT_TTL_GET                 BLE_MESH_MODEL_OP_2(0x80, 0x0c)
#define OP_DEFAULT_TTL_SET                 BLE_MESH_MODEL_OP_2(0x80, 0x0d)
#define OP_DEFAULT_TTL_STATUS              BLE_MESH_MODEL_OP_2(0x80, 0x0e)
#define OP_FRIEND_GET                      BLE_MESH_MODEL_OP_2(0x80, 0x0f)
#define OP_FRIEND_SET                      BLE_MESH_MODEL_OP_2(0x80, 0x10)
#define OP_FRIEND_STATUS                   BLE_MESH_MODEL_OP_2(0x80, 0x11)
#define OP_GATT_PROXY_GET                  BLE_MESH_MODEL_OP_2(0x80, 0x12)
#define OP_GATT_PROXY_SET                  BLE_MESH_MODEL_OP_2(0x80, 0x13)
#define OP_GATT_PROXY_STATUS               BLE_MESH_MODEL_OP_2(0x80, 0x14)
#define OP_KRP_GET                         BLE_MESH_MODEL_OP_2(0x80, 0x15)
#define OP_KRP_SET                         BLE_MESH_MODEL_OP_2(0x80, 0x16)
#define OP_KRP_STATUS                      BLE_MESH_MODEL_OP_2(0x80, 0x17)
#define OP_MOD_PUB_GET                     BLE_MESH_MODEL_OP_2(0x80, 0x18)
#define OP_MOD_PUB_STATUS                  BLE_MESH_MODEL_OP_2(0x80, 0x19)
#define OP_MOD_PUB_VA_SET                  BLE_MESH_MODEL_OP_2(0x80, 0x1a)
#define OP_MOD_SUB_ADD                     BLE_MESH_MODEL_OP_2(0x80, 0x1b)
#define OP_MOD_SUB_DEL                     BLE_MESH_MODEL_OP_2(0x80, 0x1c)
#define OP_MOD_SUB_DEL_ALL                 BLE_MESH_MODEL_OP_2(0x80, 0x1d)
#define OP_MOD_SUB_OVERWRITE               BLE_MESH_MODEL_OP_2(0x80, 0x1e)
#define OP_MOD_SUB_STATUS                  BLE_MESH_MODEL_OP_2(0x80, 0x1f)
#define OP_MOD_SUB_VA_ADD                  BLE_MESH_MODEL_OP_2(0x80, 0x20)
#define OP_MOD_SUB_VA_DEL                  BLE_MESH_MODEL_OP_2(0x80, 0x21)
#define OP_MOD_SUB_VA_OVERWRITE            BLE_MESH_MODEL_OP_2(0x80, 0x22)
#define OP_NET_TRANSMIT_GET                BLE_MESH_MODEL_OP_2(0x80, 0x23)
#define OP_NET_TRANSMIT_SET                BLE_MESH_MODEL_OP_2(0x80, 0x24)
#define OP_NET_TRANSMIT_STATUS             BLE_MESH_MODEL_OP_2(0x80, 0x25)
#define OP_RELAY_GET                       BLE_MESH_MODEL_OP_2(0x80, 0x26)
#define OP_RELAY_SET                       BLE_MESH_MODEL_OP_2(0x80, 0x27)
#define OP_RELAY_STATUS                    BLE_MESH_MODEL_OP_2(0x80, 0x28)
#define OP_MOD_SUB_GET                     BLE_MESH_MODEL_OP_2(0x80, 0x29)
#define OP_MOD_SUB_LIST                    BLE_MESH_MODEL_OP_2(0x80, 0x2a)
#define OP_MOD_SUB_GET_VND                 BLE_MESH_MODEL_OP_2(0x80, 0x2b)
#define OP_MOD_SUB_LIST_VND                BLE_MESH_MODEL_OP_2(0x80, 0x2c)
#define OP_LPN_TIMEOUT_GET                 BLE_MESH_MODEL_OP_2(0x80, 0x2d)
#define OP_LPN_TIMEOUT_STATUS              BLE_MESH_MODEL_OP_2(0x80, 0x2e)
#define OP_HEALTH_FAULT_CLEAR              BLE_MESH_MODEL_OP_2(0x80, 0x2f)
#define OP_HEALTH_FAULT_CLEAR_UNREL        BLE_MESH_MODEL_OP_2(0x80, 0x30)
#define OP_HEALTH_FAULT_GET                BLE_MESH_MODEL_OP_2(0x80, 0x31)
#define OP_HEALTH_FAULT_TEST               BLE_MESH_MODEL_OP_2(0x80, 0x32)
#define OP_HEALTH_FAULT_TEST_UNREL         BLE_MESH_MODEL_OP_2(0x80, 0x33)
#define OP_HEALTH_PERIOD_GET               BLE_MESH_MODEL_OP_2(0x80, 0x34)
#define OP_HEALTH_PERIOD_SET               BLE_MESH_MODEL_OP_2(0x80, 0x35)
#define OP_HEALTH_PERIOD_SET_UNREL         BLE_MESH_MODEL_OP_2(0x80, 0x36)
#define OP_HEALTH_PERIOD_STATUS            BLE_MESH_MODEL_OP_2(0x80, 0x37)
#define OP_HEARTBEAT_PUB_GET               BLE_MESH_MODEL_OP_2(0x80, 0x38)
#define OP_HEARTBEAT_PUB_SET               BLE_MESH_MODEL_OP_2(0x80, 0x39)
#define OP_HEARTBEAT_SUB_GET               BLE_MESH_MODEL_OP_2(0x80, 0x3a)
#define OP_HEARTBEAT_SUB_SET               BLE_MESH_MODEL_OP_2(0x80, 0x3b)
#define OP_HEARTBEAT_SUB_STATUS            BLE_MESH_MODEL_OP_2(0x80, 0x3c)
#define OP_MOD_APP_BIND                    BLE_MESH_MODEL_OP_2(0x80, 0x3d)
#define OP_MOD_APP_STATUS                  BLE_MESH_MODEL_OP_2(0x80, 0x3e)
#define OP_MOD_APP_UNBIND                  BLE_MESH_MODEL_OP_2(0x80, 0x3f)
#define OP_NET_KEY_ADD                     BLE_MESH_MODEL_OP_2(0x80, 0x40)
#define OP_NET_KEY_DEL                     BLE_MESH_MODEL_OP_2(0x80, 0x41)
#define OP_NET_KEY_GET                     BLE_MESH_MODEL_OP_2(0x80, 0x42)
#define OP_NET_KEY_LIST                    BLE_MESH_MODEL_OP_2(0x80, 0x43)
#define OP_NET_KEY_STATUS                  BLE_MESH_MODEL_OP_2(0x80, 0x44)
#define OP_NET_KEY_UPDATE                  BLE_MESH_MODEL_OP_2(0x80, 0x45)
#define OP_NODE_IDENTITY_GET               BLE_MESH_MODEL_OP_2(0x80, 0x46)
#define OP_NODE_IDENTITY_SET               BLE_MESH_MODEL_OP_2(0x80, 0x47)
#define OP_NODE_IDENTITY_STATUS            BLE_MESH_MODEL_OP_2(0x80, 0x48)
#define OP_NODE_RESET                      BLE_MESH_MODEL_OP_2(0x80, 0x49)
#define OP_NODE_RESET_STATUS               BLE_MESH_MODEL_OP_2(0x80, 0x4a)
#define OP_SIG_MOD_APP_GET                 BLE_MESH_MODEL_OP_2(0x80, 0x4b)
#define OP_SIG_MOD_APP_LIST                BLE_MESH_MODEL_OP_2(0x80, 0x4c)
#define OP_VND_MOD_APP_GET                 BLE_MESH_MODEL_OP_2(0x80, 0x4d)
#define OP_VND_MOD_APP_LIST                BLE_MESH_MODEL_OP_2(0x80, 0x4e)

/* Generic OnOff Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_ONOFF_GET                          BLE_MESH_MODEL_OP_2(0x82, 0x01)
#define BLE_MESH_MODEL_OP_GEN_ONOFF_SET                          BLE_MESH_MODEL_OP_2(0x82, 0x02)
#define BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK                    BLE_MESH_MODEL_OP_2(0x82, 0x03)
#define BLE_MESH_MODEL_OP_GEN_ONOFF_STATUS                       BLE_MESH_MODEL_OP_2(0x82, 0x04)

/* Generic Level Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_LEVEL_GET                          BLE_MESH_MODEL_OP_2(0x82, 0x05)
#define BLE_MESH_MODEL_OP_GEN_LEVEL_SET                          BLE_MESH_MODEL_OP_2(0x82, 0x06)
#define BLE_MESH_MODEL_OP_GEN_LEVEL_SET_UNACK                    BLE_MESH_MODEL_OP_2(0x82, 0x07)
#define BLE_MESH_MODEL_OP_GEN_LEVEL_STATUS                       BLE_MESH_MODEL_OP_2(0x82, 0x08)
#define BLE_MESH_MODEL_OP_GEN_DELTA_SET                          BLE_MESH_MODEL_OP_2(0x82, 0x09)
#define BLE_MESH_MODEL_OP_GEN_DELTA_SET_UNACK                    BLE_MESH_MODEL_OP_2(0x82, 0x0A)
#define BLE_MESH_MODEL_OP_GEN_MOVE_SET                           BLE_MESH_MODEL_OP_2(0x82, 0x0B)
#define BLE_MESH_MODEL_OP_GEN_MOVE_SET_UNACK                     BLE_MESH_MODEL_OP_2(0x82, 0x0C)

/* Generic Default Transition Time Message Opcode*/
#define BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_GET                 BLE_MESH_MODEL_OP_2(0x82, 0x0D)
#define BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_SET                 BLE_MESH_MODEL_OP_2(0x82, 0x0E)
#define BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_SET_UNACK           BLE_MESH_MODEL_OP_2(0x82, 0x0F)
#define BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_STATUS              BLE_MESH_MODEL_OP_2(0x82, 0x10)

/* Generic Power OnOff Message Opcode*/
#define BLE_MESH_MODEL_OP_GEN_ONPOWERUP_GET                      BLE_MESH_MODEL_OP_2(0x82, 0x11)
#define BLE_MESH_MODEL_OP_GEN_ONPOWERUP_STATUS                   BLE_MESH_MODEL_OP_2(0x82, 0x12)

/* Generic Power OnOff Setup Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_ONPOWERUP_SET                      BLE_MESH_MODEL_OP_2(0x82, 0x13)
#define BLE_MESH_MODEL_OP_GEN_ONPOWERUP_SET_UNACK                BLE_MESH_MODEL_OP_2(0x82, 0x14)

/* Generic Power Level Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_GET                    BLE_MESH_MODEL_OP_2(0x82, 0x15)
#define BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_SET                    BLE_MESH_MODEL_OP_2(0x82, 0x16)
#define BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_SET_UNACK              BLE_MESH_MODEL_OP_2(0x82, 0x17)
#define BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_STATUS                 BLE_MESH_MODEL_OP_2(0x82, 0x18)
#define BLE_MESH_MODEL_OP_GEN_POWER_LAST_GET                     BLE_MESH_MODEL_OP_2(0x82, 0x19)
#define BLE_MESH_MODEL_OP_GEN_POWER_LAST_STATUS                  BLE_MESH_MODEL_OP_2(0x82, 0x1A)
#define BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_GET                  BLE_MESH_MODEL_OP_2(0x82, 0x1B)
#define BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_STATUS               BLE_MESH_MODEL_OP_2(0x82, 0x1C)
#define BLE_MESH_MODEL_OP_GEN_POWER_RANGE_GET                    BLE_MESH_MODEL_OP_2(0x82, 0x1D)
#define BLE_MESH_MODEL_OP_GEN_POWER_RANGE_STATUS                 BLE_MESH_MODEL_OP_2(0x82, 0x1E)

/* Generic Power Level Setup Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_SET                  BLE_MESH_MODEL_OP_2(0x82, 0x1F)
#define BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_SET_UNACK            BLE_MESH_MODEL_OP_2(0x82, 0x20)
#define BLE_MESH_MODEL_OP_GEN_POWER_RANGE_SET                    BLE_MESH_MODEL_OP_2(0x82, 0x21)
#define BLE_MESH_MODEL_OP_GEN_POWER_RANGE_SET_UNACK              BLE_MESH_MODEL_OP_2(0x82, 0x22)

/* Generic Battery Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_BATTERY_GET                        BLE_MESH_MODEL_OP_2(0x82, 0x23)
#define BLE_MESH_MODEL_OP_GEN_BATTERY_STATUS                     BLE_MESH_MODEL_OP_2(0x82, 0x24)

/* Generic Location Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_GET                     BLE_MESH_MODEL_OP_2(0x82, 0x25)
#define BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_STATUS                  BLE_MESH_MODEL_OP_1(0x40)
#define BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_GET                      BLE_MESH_MODEL_OP_2(0x82, 0x26)
#define BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_STATUS                   BLE_MESH_MODEL_OP_2(0x82, 0x27)

/* Generic Location Setup Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET                     BLE_MESH_MODEL_OP_1(0x41)
#define BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET_UNACK               BLE_MESH_MODEL_OP_1(0x42)
#define BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET                      BLE_MESH_MODEL_OP_2(0x82, 0x28)
#define BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET_UNACK                BLE_MESH_MODEL_OP_2(0x82, 0x29)

/* Generic Manufacturer Property Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_MANU_PROPERTIES_GET                BLE_MESH_MODEL_OP_2(0x82, 0x2A)
#define BLE_MESH_MODEL_OP_GEN_MANU_PROPERTIES_STATUS             BLE_MESH_MODEL_OP_1(0x43)
#define BLE_MESH_MODEL_OP_GEN_MANU_PROPERTY_GET                  BLE_MESH_MODEL_OP_2(0x82, 0x2B)
#define BLE_MESH_MODEL_OP_GEN_MANU_PROPERTY_SET                  BLE_MESH_MODEL_OP_1(0x44)
#define BLE_MESH_MODEL_OP_GEN_MANU_PROPERTY_SET_UNACK            BLE_MESH_MODEL_OP_1(0x45)
#define BLE_MESH_MODEL_OP_GEN_MANU_PROPERTY_STATUS               BLE_MESH_MODEL_OP_1(0x46)

/* Generic Admin Property Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTIES_GET               BLE_MESH_MODEL_OP_2(0x82, 0x2C)
#define BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTIES_STATUS            BLE_MESH_MODEL_OP_1(0x47)
#define BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_GET                 BLE_MESH_MODEL_OP_2(0x82, 0x2D)
#define BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_SET                 BLE_MESH_MODEL_OP_1(0x48)
#define BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_SET_UNACK           BLE_MESH_MODEL_OP_1(0x49)
#define BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_STATUS              BLE_MESH_MODEL_OP_1(0x4A)

/* Generic User Property Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_USER_PROPERTIES_GET                BLE_MESH_MODEL_OP_2(0x82, 0x2E)
#define BLE_MESH_MODEL_OP_GEN_USER_PROPERTIES_STATUS             BLE_MESH_MODEL_OP_1(0x4B)
#define BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_GET                  BLE_MESH_MODEL_OP_2(0x82, 0x2F)
#define BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_SET                  BLE_MESH_MODEL_OP_1(0x4C)
#define BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_SET_UNACK            BLE_MESH_MODEL_OP_1(0x4D)
#define BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_STATUS               BLE_MESH_MODEL_OP_1(0x4E)

/* Generic Client Property Message Opcode */
#define BLE_MESH_MODEL_OP_GEN_CLIENT_PROPERTIES_GET              BLE_MESH_MODEL_OP_1(0x4F)
#define BLE_MESH_MODEL_OP_GEN_CLIENT_PROPERTIES_STATUS           BLE_MESH_MODEL_OP_1(0x50)

/* Sensor Message Opcode */
#define BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET                  BLE_MESH_MODEL_OP_2(0x82, 0x30)
#define BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS               BLE_MESH_MODEL_OP_1(0x51)
#define BLE_MESH_MODEL_OP_SENSOR_GET                             BLE_MESH_MODEL_OP_2(0x82, 0x31)
#define BLE_MESH_MODEL_OP_SENSOR_STATUS                          BLE_MESH_MODEL_OP_1(0x52)
#define BLE_MESH_MODEL_OP_SENSOR_COLUMN_GET                      BLE_MESH_MODEL_OP_2(0x82, 0x32)
#define BLE_MESH_MODEL_OP_SENSOR_COLUMN_STATUS                   BLE_MESH_MODEL_OP_1(0x53)
#define BLE_MESH_MODEL_OP_SENSOR_SERIES_GET                      BLE_MESH_MODEL_OP_2(0x82, 0x33)
#define BLE_MESH_MODEL_OP_SENSOR_SERIES_STATUS                   BLE_MESH_MODEL_OP_1(0x54)

/* Sensor Setup Message Opcode */
#define BLE_MESH_MODEL_OP_SENSOR_CADENCE_GET                     BLE_MESH_MODEL_OP_2(0x82, 0x34)
#define BLE_MESH_MODEL_OP_SENSOR_CADENCE_SET                     BLE_MESH_MODEL_OP_1(0x55)
#define BLE_MESH_MODEL_OP_SENSOR_CADENCE_SET_UNACK               BLE_MESH_MODEL_OP_1(0x56)
#define BLE_MESH_MODEL_OP_SENSOR_CADENCE_STATUS                  BLE_MESH_MODEL_OP_1(0x57)
#define BLE_MESH_MODEL_OP_SENSOR_SETTINGS_GET                    BLE_MESH_MODEL_OP_2(0x82, 0x35)
#define BLE_MESH_MODEL_OP_SENSOR_SETTINGS_STATUS                 BLE_MESH_MODEL_OP_1(0x58)
#define BLE_MESH_MODEL_OP_SENSOR_SETTING_GET                     BLE_MESH_MODEL_OP_2(0x82, 0x36)
#define BLE_MESH_MODEL_OP_SENSOR_SETTING_SET                     BLE_MESH_MODEL_OP_1(0x59)
#define BLE_MESH_MODEL_OP_SENSOR_SETTING_SET_UNACK               BLE_MESH_MODEL_OP_1(0x5A)
#define BLE_MESH_MODEL_OP_SENSOR_SETTING_STATUS                  BLE_MESH_MODEL_OP_1(0x5B)

/* Time Message Opcode */
#define BLE_MESH_MODEL_OP_TIME_GET                               BLE_MESH_MODEL_OP_2(0x82, 0x37)
#define BLE_MESH_MODEL_OP_TIME_SET                               BLE_MESH_MODEL_OP_1(0x5C)
#define BLE_MESH_MODEL_OP_TIME_STATUS                            BLE_MESH_MODEL_OP_1(0x5D)
#define BLE_MESH_MODEL_OP_TIME_ROLE_GET                          BLE_MESH_MODEL_OP_2(0x82, 0x38)
#define BLE_MESH_MODEL_OP_TIME_ROLE_SET                          BLE_MESH_MODEL_OP_2(0x82, 0x39)
#define BLE_MESH_MODEL_OP_TIME_ROLE_STATUS                       BLE_MESH_MODEL_OP_2(0x82, 0x3A)
#define BLE_MESH_MODEL_OP_TIME_ZONE_GET                          BLE_MESH_MODEL_OP_2(0x82, 0x3B)
#define BLE_MESH_MODEL_OP_TIME_ZONE_SET                          BLE_MESH_MODEL_OP_2(0x82, 0x3C)
#define BLE_MESH_MODEL_OP_TIME_ZONE_STATUS                       BLE_MESH_MODEL_OP_2(0x82, 0x3D)
#define BLE_MESH_MODEL_OP_TAI_UTC_DELTA_GET                      BLE_MESH_MODEL_OP_2(0x82, 0x3E)
#define BLE_MESH_MODEL_OP_TAI_UTC_DELTA_SET                      BLE_MESH_MODEL_OP_2(0x82, 0x3F)
#define BLE_MESH_MODEL_OP_TAI_UTC_DELTA_STATUS                   BLE_MESH_MODEL_OP_2(0x82, 0x40)

/* Scene Message Opcode */
#define BLE_MESH_MODEL_OP_SCENE_GET                              BLE_MESH_MODEL_OP_2(0x82, 0x41)
#define BLE_MESH_MODEL_OP_SCENE_RECALL                           BLE_MESH_MODEL_OP_2(0x82, 0x42)
#define BLE_MESH_MODEL_OP_SCENE_RECALL_UNACK                     BLE_MESH_MODEL_OP_2(0x82, 0x43)
#define BLE_MESH_MODEL_OP_SCENE_STATUS                           BLE_MESH_MODEL_OP_1(0x5E)
#define BLE_MESH_MODEL_OP_SCENE_REGISTER_GET                     BLE_MESH_MODEL_OP_2(0x82, 0x44)
#define BLE_MESH_MODEL_OP_SCENE_REGISTER_STATUS                  BLE_MESH_MODEL_OP_2(0x82, 0x45)

/* Scene Setup Message Opcode */
#define BLE_MESH_MODEL_OP_SCENE_STORE                            BLE_MESH_MODEL_OP_2(0x82, 0x46)
#define BLE_MESH_MODEL_OP_SCENE_STORE_UNACK                      BLE_MESH_MODEL_OP_2(0x82, 0x47)
#define BLE_MESH_MODEL_OP_SCENE_DELETE                           BLE_MESH_MODEL_OP_2(0x82, 0x9E)
#define BLE_MESH_MODEL_OP_SCENE_DELETE_UNACK                     BLE_MESH_MODEL_OP_2(0x82, 0x9F)

/* Scheduler Message Opcode */
#define BLE_MESH_MODEL_OP_SCHEDULER_ACT_GET                      BLE_MESH_MODEL_OP_2(0x82, 0x48)
#define BLE_MESH_MODEL_OP_SCHEDULER_ACT_STATUS                   BLE_MESH_MODEL_OP_1(0x5F)
#define BLE_MESH_MODEL_OP_SCHEDULER_GET                          BLE_MESH_MODEL_OP_2(0x82, 0x49)
#define BLE_MESH_MODEL_OP_SCHEDULER_STATUS                       BLE_MESH_MODEL_OP_2(0x82, 0x4A)

/* Scheduler Setup Message Opcode */
#define BLE_MESH_MODEL_OP_SCHEDULER_ACT_SET                      BLE_MESH_MODEL_OP_1(0x60)
#define BLE_MESH_MODEL_OP_SCHEDULER_ACT_SET_UNACK                BLE_MESH_MODEL_OP_1(0x61)

/* Light Lightness Message Opcode */
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_GET                    BLE_MESH_MODEL_OP_2(0x82, 0x4B)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET                    BLE_MESH_MODEL_OP_2(0x82, 0x4C)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET_UNACK              BLE_MESH_MODEL_OP_2(0x82, 0x4D)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_STATUS                 BLE_MESH_MODEL_OP_2(0x82, 0x4E)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_GET             BLE_MESH_MODEL_OP_2(0x82, 0x4F)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_SET             BLE_MESH_MODEL_OP_2(0x82, 0x50)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_SET_UNACK       BLE_MESH_MODEL_OP_2(0x82, 0x51)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_STATUS          BLE_MESH_MODEL_OP_2(0x82, 0x52)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LAST_GET               BLE_MESH_MODEL_OP_2(0x82, 0x53)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LAST_STATUS            BLE_MESH_MODEL_OP_2(0x82, 0x54)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_GET            BLE_MESH_MODEL_OP_2(0x82, 0x55)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_STATUS         BLE_MESH_MODEL_OP_2(0x82, 0x56)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_GET              BLE_MESH_MODEL_OP_2(0x82, 0x57)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_STATUS           BLE_MESH_MODEL_OP_2(0x82, 0x58)

/* Light Lightness Setup Message Opcode */
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_SET            BLE_MESH_MODEL_OP_2(0x82, 0x59)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_SET_UNACK      BLE_MESH_MODEL_OP_2(0x82, 0x5A)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_SET              BLE_MESH_MODEL_OP_2(0x82, 0x5B)
#define BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_SET_UNACK        BLE_MESH_MODEL_OP_2(0x82, 0x5C)

/* Light CTL Message Opcode */
#define BLE_MESH_MODEL_OP_LIGHT_CTL_GET                          BLE_MESH_MODEL_OP_2(0x82, 0x5D)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_SET                          BLE_MESH_MODEL_OP_2(0x82, 0x5E)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_SET_UNACK                    BLE_MESH_MODEL_OP_2(0x82, 0x5F)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_STATUS                       BLE_MESH_MODEL_OP_2(0x82, 0x60)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_GET              BLE_MESH_MODEL_OP_2(0x82, 0x61)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_GET        BLE_MESH_MODEL_OP_2(0x82, 0x62)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_STATUS     BLE_MESH_MODEL_OP_2(0x82, 0x63)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_SET              BLE_MESH_MODEL_OP_2(0x82, 0x64)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_SET_UNACK        BLE_MESH_MODEL_OP_2(0x82, 0x65)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_STATUS           BLE_MESH_MODEL_OP_2(0x82, 0x66)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_GET                  BLE_MESH_MODEL_OP_2(0x82, 0x67)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_STATUS               BLE_MESH_MODEL_OP_2(0x82, 0x68)

/* Light CTL Setup Message Opcode */
#define BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_SET                  BLE_MESH_MODEL_OP_2(0x82, 0x69)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_SET_UNACK            BLE_MESH_MODEL_OP_2(0x82, 0x6A)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_SET        BLE_MESH_MODEL_OP_2(0x82, 0x6B)
#define BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK  BLE_MESH_MODEL_OP_2(0x82, 0x6C)

/* Light HSL Message Opcode */
#define BLE_MESH_MODEL_OP_LIGHT_HSL_GET                          BLE_MESH_MODEL_OP_2(0x82, 0x6D)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_GET                      BLE_MESH_MODEL_OP_2(0x82, 0x6E)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_SET                      BLE_MESH_MODEL_OP_2(0x82, 0x6F)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_SET_UNACK                BLE_MESH_MODEL_OP_2(0x82, 0x70)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_STATUS                   BLE_MESH_MODEL_OP_2(0x82, 0x71)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_GET               BLE_MESH_MODEL_OP_2(0x82, 0x72)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_SET               BLE_MESH_MODEL_OP_2(0x82, 0x73)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_SET_UNACK         BLE_MESH_MODEL_OP_2(0x82, 0x74)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_STATUS            BLE_MESH_MODEL_OP_2(0x82, 0x75)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_SET                          BLE_MESH_MODEL_OP_2(0x82, 0x76)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_SET_UNACK                    BLE_MESH_MODEL_OP_2(0x82, 0x77)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_STATUS                       BLE_MESH_MODEL_OP_2(0x82, 0x78)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_TARGET_GET                   BLE_MESH_MODEL_OP_2(0x82, 0x79)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_TARGET_STATUS                BLE_MESH_MODEL_OP_2(0x82, 0x7A)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_GET                  BLE_MESH_MODEL_OP_2(0x82, 0x7B)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_STATUS               BLE_MESH_MODEL_OP_2(0x82, 0x7C)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_GET                    BLE_MESH_MODEL_OP_2(0x82, 0x7D)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_STATUS                 BLE_MESH_MODEL_OP_2(0x82, 0x7E)

/* Light HSL Setup Message Opcode */
#define BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_SET                  BLE_MESH_MODEL_OP_2(0x82, 0x7F)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_SET_UNACK            BLE_MESH_MODEL_OP_2(0x82, 0x80)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_SET                    BLE_MESH_MODEL_OP_2(0x82, 0x81)
#define BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_SET_UNACK              BLE_MESH_MODEL_OP_2(0x82, 0x82) /* Model spec is wrong */

/* Light xyL Message Opcode */
#define BLE_MESH_MODEL_OP_LIGHT_XYL_GET                          BLE_MESH_MODEL_OP_2(0x82, 0x83)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_SET                          BLE_MESH_MODEL_OP_2(0x82, 0x84)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_SET_UNACK                    BLE_MESH_MODEL_OP_2(0x82, 0x85)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_STATUS                       BLE_MESH_MODEL_OP_2(0x82, 0x86)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_TARGET_GET                   BLE_MESH_MODEL_OP_2(0x82, 0x87)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_TARGET_STATUS                BLE_MESH_MODEL_OP_2(0x82, 0x88)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_GET                  BLE_MESH_MODEL_OP_2(0x82, 0x89)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_STATUS               BLE_MESH_MODEL_OP_2(0x82, 0x8A)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_GET                    BLE_MESH_MODEL_OP_2(0x82, 0x8B)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_STATUS                 BLE_MESH_MODEL_OP_2(0x82, 0x8C)

/* Light xyL Setup Message Opcode */
#define BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_SET                  BLE_MESH_MODEL_OP_2(0x82, 0x8D)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_SET_UNACK            BLE_MESH_MODEL_OP_2(0x82, 0x8E)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_SET                    BLE_MESH_MODEL_OP_2(0x82, 0x8F)
#define BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_SET_UNACK              BLE_MESH_MODEL_OP_2(0x82, 0x90)

/* Light Control Message Opcode */
#define BLE_MESH_MODEL_OP_LIGHT_LC_MODE_GET                      BLE_MESH_MODEL_OP_2(0x82, 0x91)
#define BLE_MESH_MODEL_OP_LIGHT_LC_MODE_SET                      BLE_MESH_MODEL_OP_2(0x82, 0x92)
#define BLE_MESH_MODEL_OP_LIGHT_LC_MODE_SET_UNACK                BLE_MESH_MODEL_OP_2(0x82, 0x93)
#define BLE_MESH_MODEL_OP_LIGHT_LC_MODE_STATUS                   BLE_MESH_MODEL_OP_2(0x82, 0x94)
#define BLE_MESH_MODEL_OP_LIGHT_LC_OM_GET                        BLE_MESH_MODEL_OP_2(0x82, 0x95)
#define BLE_MESH_MODEL_OP_LIGHT_LC_OM_SET                        BLE_MESH_MODEL_OP_2(0x82, 0x96)
#define BLE_MESH_MODEL_OP_LIGHT_LC_OM_SET_UNACK                  BLE_MESH_MODEL_OP_2(0x82, 0x97)
#define BLE_MESH_MODEL_OP_LIGHT_LC_OM_STATUS                     BLE_MESH_MODEL_OP_2(0x82, 0x98)
#define BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_GET               BLE_MESH_MODEL_OP_2(0x82, 0x99)
#define BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_SET               BLE_MESH_MODEL_OP_2(0x82, 0x9A)
#define BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_SET_UNACK         BLE_MESH_MODEL_OP_2(0x82, 0x9B)
#define BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_STATUS            BLE_MESH_MODEL_OP_2(0x82, 0x9C)
#define BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_GET                  BLE_MESH_MODEL_OP_2(0x82, 0x9D)
#define BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_SET                  BLE_MESH_MODEL_OP_1(0x62)
#define BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_SET_UNACK            BLE_MESH_MODEL_OP_1(0x63)
#define BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_STATUS               BLE_MESH_MODEL_OP_1(0x64)

/* Statue_Error_Code */
#define STATUS_SUCCESS                     0x00
#define STATUS_INVALID_ADDRESS             0x01
#define STATUS_INVALID_MODEL               0x02
#define STATUS_INVALID_APPKEY              0x03
#define STATUS_INVALID_NETKEY              0x04
#define STATUS_INSUFF_RESOURCES            0x05
#define STATUS_IDX_ALREADY_STORED          0x06
#define STATUS_NVAL_PUB_PARAM              0x07
#define STATUS_NOT_SUB_MOD                 0x08
#define STATUS_STORAGE_FAIL                0x09
#define STATUS_FEAT_NOT_SUPP               0x0a
#define STATUS_CANNOT_UPDATE               0x0b
#define STATUS_CANNOT_REMOVE               0x0c
#define STATUS_CANNOT_BIND                 0x0d
#define STATUS_TEMP_STATE_CHG_FAIL         0x0e
#define STATUS_CANNOT_SET                  0x0f
#define STATUS_UNSPECIFIED                 0x10
#define STATUS_INVALID_BINDING             0x11

/* Foundation Models */
#define BLE_MESH_MODEL_ID_CFG_SRV                   0x0000
#define BLE_MESH_MODEL_ID_CFG_CLI                   0x0001
#define BLE_MESH_MODEL_ID_HEALTH_SRV                0x0002
#define BLE_MESH_MODEL_ID_HEALTH_CLI                0x0003

/* Models from the Mesh Model Specification */
#define BLE_MESH_MODEL_ID_GEN_ONOFF_SRV             0x1000
#define BLE_MESH_MODEL_ID_GEN_ONOFF_CLI             0x1001
#define BLE_MESH_MODEL_ID_GEN_LEVEL_SRV             0x1002
#define BLE_MESH_MODEL_ID_GEN_LEVEL_CLI             0x1003
#define BLE_MESH_MODEL_ID_GEN_DEF_TRANS_TIME_SRV    0x1004
#define BLE_MESH_MODEL_ID_GEN_DEF_TRANS_TIME_CLI    0x1005
#define BLE_MESH_MODEL_ID_GEN_POWER_ONOFF_SRV       0x1006
#define BLE_MESH_MODEL_ID_GEN_POWER_ONOFF_SETUP_SRV 0x1007
#define BLE_MESH_MODEL_ID_GEN_POWER_ONOFF_CLI       0x1008
#define BLE_MESH_MODEL_ID_GEN_POWER_LEVEL_SRV       0x1009
#define BLE_MESH_MODEL_ID_GEN_POWER_LEVEL_SETUP_SRV 0x100a
#define BLE_MESH_MODEL_ID_GEN_POWER_LEVEL_CLI       0x100b
#define BLE_MESH_MODEL_ID_GEN_BATTERY_SRV           0x100c
#define BLE_MESH_MODEL_ID_GEN_BATTERY_CLI           0x100d
#define BLE_MESH_MODEL_ID_GEN_LOCATION_SRV          0x100e
#define BLE_MESH_MODEL_ID_GEN_LOCATION_SETUPSRV     0x100f
#define BLE_MESH_MODEL_ID_GEN_LOCATION_CLI          0x1010
#define BLE_MESH_MODEL_ID_GEN_ADMIN_PROP_SRV        0x1011
#define BLE_MESH_MODEL_ID_GEN_MANUFACTURER_PROP_SRV 0x1012
#define BLE_MESH_MODEL_ID_GEN_USER_PROP_SRV         0x1013
#define BLE_MESH_MODEL_ID_GEN_CLIENT_PROP_SRV       0x1014
#define BLE_MESH_MODEL_ID_GEN_PROP_CLI              0x1015
#define BLE_MESH_MODEL_ID_SENSOR_SRV                0x1100
#define BLE_MESH_MODEL_ID_SENSOR_SETUP_SRV          0x1101
#define BLE_MESH_MODEL_ID_SENSOR_CLI                0x1102
#define BLE_MESH_MODEL_ID_TIME_SRV                  0x1200
#define BLE_MESH_MODEL_ID_TIME_SETUP_SRV            0x1201
#define BLE_MESH_MODEL_ID_TIME_CLI                  0x1202
#define BLE_MESH_MODEL_ID_SCENE_SRV                 0x1203
#define BLE_MESH_MODEL_ID_SCENE_SETUP_SRV           0x1204
#define BLE_MESH_MODEL_ID_SCENE_CLI                 0x1205
#define BLE_MESH_MODEL_ID_SCHEDULER_SRV             0x1206
#define BLE_MESH_MODEL_ID_SCHEDULER_SETUP_SRV       0x1207
#define BLE_MESH_MODEL_ID_SCHEDULER_CLI             0x1208
#define BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_SRV       0x1300
#define BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_SETUP_SRV 0x1301
#define BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_CLI       0x1302
#define BLE_MESH_MODEL_ID_LIGHT_CTL_SRV             0x1303
#define BLE_MESH_MODEL_ID_LIGHT_CTL_SETUP_SRV       0x1304
#define BLE_MESH_MODEL_ID_LIGHT_CTL_CLI             0x1305
#define BLE_MESH_MODEL_ID_LIGHT_CTL_TEMP_SRV        0x1306
#define BLE_MESH_MODEL_ID_LIGHT_HSL_SRV             0x1307
#define BLE_MESH_MODEL_ID_LIGHT_HSL_SETUP_SRV       0x1308
#define BLE_MESH_MODEL_ID_LIGHT_HSL_CLI             0x1309
#define BLE_MESH_MODEL_ID_LIGHT_HSL_HUE_SRV         0x130a
#define BLE_MESH_MODEL_ID_LIGHT_HSL_SAT_SRV         0x130b
#define BLE_MESH_MODEL_ID_LIGHT_XYL_SRV             0x130c
#define BLE_MESH_MODEL_ID_LIGHT_XYL_SETUP_SRV       0x130d
#define BLE_MESH_MODEL_ID_LIGHT_XYL_CLI             0x130e
#define BLE_MESH_MODEL_ID_LIGHT_LC_SRV              0x130f
#define BLE_MESH_MODEL_ID_LIGHT_LC_SETUPSRV         0x1310
#define BLE_MESH_MODEL_ID_LIGHT_LC_CLI              0x1311

// link close reason
#define CLOSE_REASON_SUCCESS 0x00
#define CLOSE_REASON_NVAL_PDU 0x01
#define CLOSE_REASON_NVAL_FMT 0x02
#define CLOSE_REASON_UNEXP_PDU 0x03
#define CLOSE_REASON_CFM_FAILED 0x04
#define CLOSE_REASON_RESOURCES 0x05
#define CLOSE_REASON_DECRYPT 0x06
#define CLOSE_REASON_UNEXP_ERR 0x07
#define CLOSE_REASON_ADDR 0x08
#define CLOSE_REASON_TIMEOUT 0x09

/** Special TTL value to request using configured default TTL */
#define BLE_MESH_TTL_DEFAULT    0xff

/** Maximum allowed TTL value */
#define BLE_MESH_TTL_MAX        0x7f

/** @def BLE_MESH_MODEL_CB
 *
 * @brief Composition data SIG model entry with callback functions.
 *
 * @param _id Model ID.
 * @param _op Array of model opcode handlers.
 * @param _pub Model publish parameters.
 * @param _user_data User data for the model.
 * @param _cb Callback structure, or NULL to keep no callbacks.
 */
#define BLE_MESH_MODEL_CB(_id, _op, _pub, _keys, _groups, _user_data, _cb)                  \
{                                                                           \
	.id = (_id),                                                            \
	.op = _op,                                                              \
	.keys = _keys,                                        \
	.pub = _pub,                                                            \
	.groups = _groups,                                 \
	.user_data = _user_data,                                                \
	.cb = _cb,                                                              \
}

/** @def BLE_MESH_MODEL_VND_CB
 *
 * @brief Composition data vendor model entry with callback functions.
 *
 * @param _company Company ID.
 * @param _id Model ID.
 * @param _op Array of model opcode handlers.
 * @param _pub Model publish parameters.
 * @param _user_data User data for the model.
 * @param _cb Callback structure, or NULL to keep no callbacks.
 */
#define BLE_MESH_MODEL_VND_CB(_company, _id, _op, _pub, _keys, _groups, _user_data, _cb)    \
{                                                                           \
	.vnd.company = (_company),                                              \
	.vnd.id = (_id),                                                        \
	.op = _op,                                                              \
	.pub = _pub,                                                            \
	.keys = _keys,                                        \
	.groups = _groups,                                 \
	.user_data = _user_data,                                                \
	.cb = _cb,                                                              \
}

/** @def BLE_MESH_MODEL
 *
 * @brief Composition data SIG model entry.
 *
 * @param _id Model ID.
 * @param _op Array of model opcode handlers.
 * @param _pub Model publish parameters.
 * @param _user_data User data for the model.
 */
#define BLE_MESH_MODEL(_id, _op, _pub, _keys, _group, _user_data)                          \
	BLE_MESH_MODEL_CB(_id, _op, _pub, _keys, _group, _user_data, NULL)

/** @def BLE_MESH_MODEL_VND
 *
 * @brief Composition data vendor model entry.
 *
 * @param _company Company ID.
 * @param _id Model ID.
 * @param _op Array of model opcode handlers.
 * @param _pub Model publish parameters.
 * @param _user_data User data for the model.
 */
#define BLE_MESH_MODEL_VND(_company, _id, _op, _pub, _keys, _group, _user_data)            \
	BLE_MESH_MODEL_VND_CB(_company, _id, _op, _pub, _keys, _group, _user_data, NULL)

/** @def BLE_MESH_TRANSMIT
 *
 *  @brief Encode transmission count & interval steps.
 *
 *  @param count   Number of retransmissions (first transmission is excluded).
 *  @param int_ms  Interval steps in milliseconds. Must be greater than 0
 *                 and a multiple of 10.
 *
 *  @return Mesh transmit value that can be used e.g. for the default
 *          values of the configuration model data.
 */
#define BLE_MESH_TRANSMIT(count, int_ms) ((count) | (((int_ms / 10) - 1) << 3))

/** @def BLE_MESH_TRANSMIT_COUNT
 *
 *  @brief Decode transmit count from a transmit value.
 *
 *  @param transmit Encoded transmit count & interval value.
 *
 *  @return Transmission count (actual transmissions is N + 1).
 */
#define BLE_MESH_TRANSMIT_COUNT(transmit) (((transmit) & (uint8_t)BIT_MASK(3)))

/** @def BLE_MESH_TRANSMIT_INT
 *
 *  @brief Decode transmit interval from a transmit value.
 *
 *  @param transmit Encoded transmit count & interval value.
 *
 *  @return Transmission interval in milliseconds.
 */
#define BLE_MESH_TRANSMIT_INT(transmit) ((((transmit) >> 3) + 1) * 10)

/** @def BLE_MESH_PUB_TRANSMIT
 *
 *  @brief Encode Publish Retransmit count & interval steps.
 *
 *  @param count   Number of retransmissions (first transmission is excluded).
 *  @param int_ms  Interval steps in milliseconds. Must be greater than 0
 *                 and a multiple of 50.
 *
 *  @return Mesh transmit value that can be used e.g. for the default
 *          values of the configuration model data.
 */
#define BLE_MESH_PUB_TRANSMIT(count, int_ms) BLE_MESH_TRANSMIT(count, (int_ms) / 5)

/** @def BLE_MESH_PUB_TRANSMIT_COUNT
 *
 *  @brief Decode Pubhlish Retransmit count from a given value.
 *
 *  @param transmit Encoded Publish Retransmit count & interval value.
 *
 *  @return Retransmission count (actual transmissions is N + 1).
 */
#define BLE_MESH_PUB_TRANSMIT_COUNT(transmit) BLE_MESH_TRANSMIT_COUNT(transmit)

/** @def BLE_MESH_PUB_TRANSMIT_INT
 *
 *  @brief Decode Publish Retransmit interval from a given value.
 *
 *  @param transmit Encoded Publish Retransmit count & interval value.
 *
 *  @return Transmission interval in milliseconds.
 */
#define BLE_MESH_PUB_TRANSMIT_INT(transmit) ((((transmit) >> 3) + 1) * 50)

/**************************************foundation****************************************/

#ifndef BIT
#define BIT(n) (1UL << (n))
#endif

#ifndef BIT_MASK
#define BIT_MASK(n) (BIT(n) - 1)
#endif

#define ARRAY_SIZE(array)              \
    ((unsigned long)((sizeof(array) / sizeof((array)[0]))))

#define ALIGN_TYPE(_type) ((size_t) & ((struct {uint8_t _1; _type _2; } *)NULL)->_2)

/**************************************MODEL****************************************/

#define BLE_MESH_MODEL_OP_1(b0)         (b0)
#define BLE_MESH_MODEL_OP_2(b0, b1)     (((b0) << 8) | (b1))
#define BLE_MESH_MODEL_OP_3(b0, cid)    ((((b0) << 16) | 0xc00000) | (cid))

#define BLE_MESH_MODEL_OP_END           { 0, 0, NULL }
#define BLE_MESH_MODEL_NO_OPS           ((struct bt_mesh_model_op []) \
                                        { BLE_MESH_MODEL_OP_END })

/**************************************SHA256****************************************/

#define TC_SHA256_BLOCK_SIZE (64)
#define TC_SHA256_DIGEST_SIZE (32)
#define TC_SHA256_STATE_BLOCKS (TC_SHA256_DIGEST_SIZE/4)

#define TC_CRYPTO_SUCCESS 1
#define TC_CRYPTO_FAIL 0

struct tc_sha256_state_struct
{
    unsigned int iv[TC_SHA256_STATE_BLOCKS];
    uint64_t bits_hashed;
    uint8_t leftover[TC_SHA256_BLOCK_SIZE];
    size_t leftover_offset;
};

typedef struct tc_sha256_state_struct *TCSha256State_t;

/***************************************mesh_cfg***************************************/

typedef struct
{
    uint8_t * base_addr;
    uint32_t mem_len;
} mem_info_t;

typedef struct
{
    uint8_t adv_buf_count;
    uint8_t rpl_count;
    uint8_t allow_rpl_cycle;
    uint8_t ivu_divider;
    uint8_t allow_same_addr;
    uint8_t mod_key_count;
    uint8_t mod_group_count;
} ble_mesh_common_cfg_t;

typedef struct
{
    uint8_t pxyfilter_count;
} ble_mesh_proxy_cfg_t;

typedef struct
{
    uint8_t msgcache_count;

    uint8_t subnet_count;
    uint8_t appkey_count;

    uint8_t unseg_length;
    uint8_t txseg_max;
    uint8_t txseg_count;

    uint8_t rxseg_count;
    uint16_t rxsdu_max;

    uint8_t label_count;
} ble_mesh_net_cfg_t;

typedef struct
{
    uint16_t seq_store_rate;
    uint16_t rpl_store_rate;
    uint16_t store_rate;
} ble_mesh_store_cfg_t;

typedef struct
{
    uint8_t frndseg_rx;
    uint8_t frndsub_size;
    uint8_t frndlpn_count;
    uint8_t frndqueue_size;
    uint16_t frndrecv_win;
} ble_mesh_friend_cfg_t;

typedef struct
{
    uint8_t lpnmin_size;
    uint8_t lpnrssi_factor;
    uint8_t lpnrecv_factor;
    uint32_t lpnpoll_interval;
    uint32_t lpnpoll_timeout;
    uint16_t lpnrecv_delay;
    uint16_t lpnretry_timeout;
} ble_mesh_lpn_cfg_t;

typedef struct
{
    uint8_t node_count;
} ble_mesh_prov_cfg_t;

typedef struct
{
    const uint32_t rf_accessAddress;
    const uint8_t rf_channel_37;
    const uint8_t rf_channel_38;
    const uint8_t rf_channel_39;
} ble_mesh_rf_cfg_t;

typedef struct
{
    ble_mesh_common_cfg_t common_cfg;
    ble_mesh_proxy_cfg_t proxy_cfg;
    ble_mesh_net_cfg_t net_cfg;
    ble_mesh_store_cfg_t store_cfg;
    ble_mesh_friend_cfg_t friend_cfg;
    ble_mesh_lpn_cfg_t lpn_cfg;
    ble_mesh_prov_cfg_t prov_cfg;
    ble_mesh_rf_cfg_t rf_cfg;
} ble_mesh_cfg_t;

/************************************k_work******************************************/

struct k_work;
typedef void (*k_work_handler_t)( struct k_work *work );

struct _snode
{
    struct _snode *next;
};
typedef struct _snode sys_snode_t;

struct k_work
{
    sys_snode_t node;
    k_work_handler_t handler;
    bt_mesh_atomic_t flags[1];
};

struct _dnode
{
    union
    {
        struct _dnode *head; /* ptr to head of list (sys_dlist_t) */
        struct _dnode *next; /* ptr to next node    (sys_dnode_t) */
    };
    union
    {
        struct _dnode *tail; /* ptr to tail of list (sys_dlist_t) */
        struct _dnode *prev; /* ptr to previous node (sys_dnode_t) */
    };
};

typedef struct _dnode sys_dlist_t;
typedef struct _dnode sys_dnode_t;

struct _timeout
{
    sys_dnode_t node;
    int32_t dticks;
};

struct k_delayed_work
{
    struct k_work work;
    struct _timeout timeout;
    struct k_work_q *work_q;
};

/************************************net******************************************/

// APP key
struct bt_mesh_app_key
{
    uint16_t net_idx;
    uint16_t app_idx;
    uint8_t updated;
    struct bt_mesh_app_keys
    {
        uint8_t id;
        uint8_t val[16];
    } keys[2];
};

// reply
struct bt_mesh_rpl {
    u16_t src;
    BOOL  old_iv;
    BOOL  store;
    u32_t seq;
};

/************************************model******************************************/

struct bt_mesh_model;

/** Model publication context. */
struct bt_mesh_model_pub
{
    /** The model the context belongs to. Initialized by the stack. */
    struct bt_mesh_model *mod;

    uint16_t addr; /**< Publish Address. */
    uint16_t key :12, /**< Publish AppKey Index. */
          cred :1, /**< Friendship Credentials Flag. */
          send_rel :1; /**< Force reliable sending (segment acks) */

    uint8_t ttl; /**< Publish Time to Live. */
    uint8_t retransmit; /**< Retransmit Count & Interval Steps. */
    uint8_t period; /**< Publish Period. */
    uint16_t period_div :4, /**< Divisor for the Period. */
          fast_period :1, /**< Use FastPeriodDivisor */
          count :3; /**< Retransmissions left. */

    uint32_t period_start; /**< Start of the current period. */

    /** @brief Publication buffer, containing the publication message.
     *
     *  This will get correctly created when the publication context
     *  has been defined using the BLE_MESH_MODEL_PUB_DEFINE macro.
     *
     *  BLE_MESH_MODEL_PUB_DEFINE(name, update, size);
     */
    struct net_buf_simple *msg;

    /** @brief Callback for updating the publication buffer.
     *
     *  When set to NULL, the model is assumed not to support
     *  periodic publishing. When set to non-NULL the callback
     *  will be called periodically and is expected to update
     *  @ref bt_mesh_model_pub.msg with a valid publication
     *  message.
     *
     *  @param mod The Model the Publication Context belogs to.
     *
     *  @return Zero on success or (negative) error code otherwise.
     */
    int (*update)( struct bt_mesh_model *mod );

    /** Publish Period Timer. Only for stack-internal use. */
    struct k_delayed_work timer;
};

/** Message sending context. */
struct bt_mesh_msg_ctx
{
    /** NetKey Index of the subnet to send the message on. */
    uint16_t net_idx;

    /** AppKey Index to encrypt the message with. */
    uint16_t app_idx;

    /** Remote address. */
    uint16_t addr;

    /** Destination address of a received message. Not used for sending. */
    uint16_t recv_dst;

    /** RSSI of received packet. Not used for sending. */
    int8_t recv_rssi;

    /** Received TTL value. Not used for sending. */
    uint8_t recv_ttl :7;

    /** Force sending reliably by using segment acknowledgement */
    uint8_t send_rel :1;

    /** TTL, or BLE_MESH_TTL_DEFAULT for default TTL. */
    uint8_t send_ttl;
};

struct bt_mesh_model_op
{
    /* OpCode encoded using the BLE_MESH_MODEL_OP_* macros */
    const uint32_t opcode;

    /* Minimum required message length */
    const size_t min_len;

    /* Message handler for the opcode */
    void (* const func)( struct bt_mesh_model *model,
        struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf );
};

typedef ssize_t (*settings_read_cb)( void *cb_arg, void *data, size_t len );

/** Model callback functions. */
struct bt_mesh_model_cb
{
    /** @brief Set value handler of user data tied to the model.
     *
     * @sa settings_handler::h_set
     *
     * @param model Model to set the persistent data of.
     * @param len_rd The size of the data found in the backend.
     * @param read_cb Function provided to read the data from the backend.
     * @param cb_arg Arguments for the read function provided by the
     * backend.
     *
     * @return 0 on success, error otherwise.
     */
    int (* const settings_set)( struct bt_mesh_model *model, size_t len_rd,
        settings_read_cb read_cb, void *cb_arg );

    /** @brief Callback called when the mesh is started.
     *
     *  This handler gets called after the node has been provisioned, or
     *  after all mesh data has been loaded from persistent storage.
     *
     *  When this callback fires, the mesh model may start its behavior,
     *  and all Access APIs are ready for use.
     *
     *  @param model      Model this callback belongs to.
     *  @param first_time Flag indicating whether this is the first time the
     *                    node is started. If false, provisioning data was
     *                    loaded from persistent storage.
     */
    int (* const start)( struct bt_mesh_model *model, uint8_t first_time );

    /** @brief Model init callback.
     *
     * Called on every model instance during mesh initialization.
     *
     * @param model Model to be initialized.
     *
     * @return 0 on success, error otherwise.
     */
    int (* const init)( struct bt_mesh_model *model );

    /** @brief Model reset callback.
     *
     * Called when the mesh node is reset. All model data is deleted on
     * reset, and the model should clear its state.
     *
     * @param model Model this callback belongs to.
     */
    void (* const reset)( struct bt_mesh_model *model );
};

/** Abstraction that describes a Mesh Model instance */
struct bt_mesh_model
{
    union
    {
        const uint16_t id;
        struct
        {
            uint16_t id;
            uint16_t company;
        } vnd;
    };

    /* Internal information, mainly for persistent storage */
    uint8_t elem_idx; /* Belongs to Nth element */
    uint8_t model_idx; /* Is the Nth model in the element */
    uint16_t flags; /* Information about what has changed */

    /* Model Publication */
    struct bt_mesh_model_pub * const pub;

    /* AppKey List */
    uint16_t *keys;

    /* Subscription List (group or virtual addresses) */
    uint16_t *groups;

    const struct bt_mesh_model_op * op;

    /* Model callback structure. */
    const struct bt_mesh_model_cb * cb;

    /* Model-specific user data */
    void *user_data;
};

struct bt_mesh_send_cb
{
    void (*start)( uint16_t duration, int err, void *cb_data );
    void (*end)( int err, void *cb_data );
};

typedef void (*elem_unkonw_op_cb_t)( uint32_t opcode, struct bt_mesh_model *model,
    struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf );

/************************************elem******************************************/

/** Abstraction that describes a Mesh Element */
struct bt_mesh_elem
{
    /* Unicast Address. Set at runtime during provisioning. */
    uint16_t addr;

    /* Location Descriptor (GATT Bluetooth Namespace Descriptors) */
    const uint16_t loc;

    const uint8_t model_count;
    const uint8_t vnd_model_count;

    struct bt_mesh_model * const models;
    struct bt_mesh_model * const vnd_models;
};

/** Node Composition */
struct bt_mesh_comp
{
    uint16_t cid;
    uint16_t pid;
    uint16_t vid;

    size_t elem_count;
    struct bt_mesh_elem *elem;
};

typedef enum
{
    BLE_MESH_PROV_OOB_OTHER = BIT( 0 ),
    BLE_MESH_PROV_OOB_URI = BIT( 1 ),
    BLE_MESH_PROV_OOB_2D_CODE = BIT( 2 ),
    BLE_MESH_PROV_OOB_BAR_CODE = BIT( 3 ),
    BLE_MESH_PROV_OOB_NFC = BIT( 4 ),
    BLE_MESH_PROV_OOB_NUMBER = BIT( 5 ),
    BLE_MESH_PROV_OOB_STRING = BIT( 6 ),
    /* 7 - 10 are reserved */
    BLE_MESH_PROV_OOB_ON_BOX = BIT( 11 ),
    BLE_MESH_PROV_OOB_IN_BOX = BIT( 12 ),
    BLE_MESH_PROV_OOB_ON_PAPER = BIT( 13 ),
    BLE_MESH_PROV_OOB_IN_MANUAL = BIT( 14 ),
    BLE_MESH_PROV_OOB_ON_DEV = BIT( 15 ),
} bt_mesh_prov_oob_info_t;

typedef enum
{
    BLE_MESH_NO_OUTPUT = 0,
    BLE_MESH_BLINK = BIT( 0 ),
    BLE_MESH_BEEP = BIT( 1 ),
    BLE_MESH_VIBRATE = BIT( 2 ),
    BLE_MESH_DISPLAY_NUMBER = BIT( 3 ),
    BLE_MESH_DISPLAY_STRING = BIT( 4 ),
} bt_mesh_output_action_t;

typedef enum
{
    BLE_MESH_NO_INPUT = 0,
    BLE_MESH_PUSH = BIT( 0 ),
    BLE_MESH_TWIST = BIT( 1 ),
    BLE_MESH_ENTER_NUMBER = BIT( 2 ),
    BLE_MESH_ENTER_STRING = BIT( 3 ),
} bt_mesh_input_action_t;

typedef enum
{
    BLE_MESH_PROV_ADV = BIT( 0 ),
    BLE_MESH_PROV_GATT = BIT( 1 ),
} bt_mesh_prov_bearer_t;

typedef struct
{
    uint8_t type;
    uint8_t val[6];
} bt_mesh_addr_t;

typedef union
{
    struct pb_adv
    {
        const uint32_t *uri_hash;
    } adv;

    struct pb_gatt
    {
        const bt_mesh_addr_t *addr;
    } gatt;
} unprivison_info_t;

/** Provisioning properties & capabilities. */
struct bt_mesh_prov
{
    const uint8_t *uuid;                       /** The UUID that's used when advertising as unprovisioned */

    /** Optional URI. This will be advertised separately from the
     *  unprovisioned beacon, however the unprovisioned beacon will
     *  contain a hash of it so the two can be associated by the
     *  provisioner.
     */
    const char *uri;
    bt_mesh_prov_oob_info_t oob_info;       /** Out of Band information field. */
    uint8_t oob_pub_key;                       /** Flag indicates whether unprovisioned devices support OOB public key */

    /** @brief Set device OOB public key.
     *
     *  This callback notifies the application to
     *  set OOB public key & private key pair.
     */
    void (*oob_pub_key_cb)( void );
    const uint8_t *static_val;                 /** Static OOB value */
    uint8_t static_val_len;                    /** Static OOB value length */
    uint8_t output_size;                       /** Maximum size of Output OOB supported */
    uint16_t output_actions;                   /** Supported Output OOB Actions */
    uint8_t input_size;                        /** Maximum size of Input OOB supported */
    uint16_t input_actions;                    /** Supported Input OOB Actions */

    /** @brief Output of a number is requested.
     *
     *  This callback notifies the application to
     *  output the given number using the given action.
     *
     *  @param act Action for outputting the number.
     *  @param num Number to be out-put.
     *
     *  @return Zero on success or negative error code otherwise
     */
    int (*output_number)( bt_mesh_output_action_t act, uint32_t num );

    /** @brief Output of a string is requested.
     *
     *  This callback notifies the application to
     *  display the given string to the user.
     *
     *  @param str String to be displayed.
     *
     *  @return Zero on success or negative error code otherwise
     */
    int (*output_string)( const char *str );

    /** @brief Input is requested.
     *
     *  This callback notifies the application to request
     *  input from the user using the given action. The
     *  requested input will either be a string or a number, and
     *  the application needs to consequently call the
     *  bt_mesh_input_string() or bt_mesh_input_number() functions
     *  once the data has been acquired from the user.
     *
     *  @param act Action for inputting data.
     *  @param num Maximum size of the in-put data.
     *
     *  @return Zero on success or negative error code otherwise
     */
    int (*input)( bt_mesh_input_action_t act, uint8_t size );

    /** @brief Unprovisioned beacon has been received.
     *
     * This callback notifies the application that an unprovisioned
     * beacon has been received.
     *
     * @param uuid UUID
     * @param oob_info OOB Information
     * @param uri_hash Pointer to URI Hash value. NULL if no hash was
     *                 present in the beacon.
     */
    void (*unprovisioned_beacon)( bt_mesh_prov_bearer_t bearer,
        const uint8_t uuid[16], bt_mesh_prov_oob_info_t oob_info,
        const unprivison_info_t *info );

    void (*pear_oob_pub_key)( void );

    /** @brief Provisioning link has been opened.
     *
     *  This callback notifies the application that a provisioning
     *  link has been opened on the given provisioning bearer.
     *
     *  @param bearer Provisioning bearer.
     */
    void (*link_open)( bt_mesh_prov_bearer_t bearer );

    /** @brief Provisioning link has been closed.
     *
     *  This callback notifies the application that a provisioning
     *  link has been closed on the given provisioning bearer.
     *
     *  @param bearer Provisioning bearer.
     */
    void (*link_close)( bt_mesh_prov_bearer_t bearer, uint8_t reason );

    /** @brief Provisioning is complete.
     *
     *  This callback notifies the application that provisioning has
     *  been successfully completed, and that the local node has been
     *  assigned the specified NetKeyIndex and primary element address.
     *
     *  @param net_idx  NetKeyIndex given during provisioning.
     *  @param addr     Primary element address.
     *  @param flags    Key Refresh & IV Update flags
     *  @param iv_index IV Index.
     */
    void (*complete)( uint16_t net_idx, uint16_t addr, uint8_t flags, uint32_t iv_index );

    /** @brief A new node has been added to the provisioning database.
     *
     *  This callback notifies the application that provisioning has
     *  been successfully completed, and that a node has been assigned
     *  the specified NetKeyIndex and primary element address.
     *
     *  @param net_idx NetKeyIndex given during provisioning.
     *  @param addr Primary element address.
     *  @param num_elem Number of elements that this node has.
     */
    void (*node_added)( uint16_t net_idx, uint16_t addr, uint8_t num_elem );

    /** @brief Node has been reset.
     *
     *  This callback notifies the application that the local node
     *  has been reset and needs to be reprovisioned. The node will
     *  not automatically advertise as unprovisioned, rather needs
     *  to enable unprovisioned advertising on one or more
     *  provisioning bearers.
     */
    void (*reset)( void );
};

struct bt_mesh_conn
{
    uint16_t handle;
    bt_mesh_atomic_t ref;
};

/** Proxy Client. */
struct bt_mesh_proxy_cli
{
    void (*connected)( struct bt_mesh_conn *conn );

    void (*disconnected)( struct bt_mesh_conn *conn, uint8_t reason );

    void (*net_id_recv)( const bt_mesh_addr_t *addr, uint16_t net_idx );

    void (*node_id_recv)( const bt_mesh_addr_t *bt_addr, uint16_t net_idx,
        uint16_t addr );
};

extern const struct bt_mesh_model_op bt_mesh_cfg_srv_op[];
extern const struct bt_mesh_model_cb bt_mesh_cfg_srv_cb;

#define BLE_MESH_MODEL_CFG_SRV(_keys, _groups, srv_data)                                  \
    BLE_MESH_MODEL_CB(BLE_MESH_MODEL_ID_CFG_SRV, bt_mesh_cfg_srv_op, NULL, \
                     _keys, _groups, srv_data, &bt_mesh_cfg_srv_cb)
#define BLE_MESH_MODEL_CFG_CLI(_keys, _groups, cli_data)	\
	BLE_MESH_MODEL_CB(BLE_MESH_MODEL_ID_CFG_CLI, NULL, NULL, _keys, _groups, cli_data, NULL)

extern const struct bt_mesh_model_op bt_mesh_health_srv_op[];
extern const struct bt_mesh_model_cb bt_mesh_health_mod_srv_cb;

#define BLE_MESH_MODEL_HEALTH_SRV(_keys, _groups, srv, pub)                               \
    BLE_MESH_MODEL_CB(BLE_MESH_MODEL_ID_HEALTH_SRV, bt_mesh_health_srv_op, \
                     pub, _keys, _groups, srv, &bt_mesh_health_mod_srv_cb)

/*****************************************buf*************************************/

#define BLE_MESH_NET_BUF_USER_DATA_SIZE     1

struct net_buf_simple
{
    /** Pointer to the start of data in the buffer. */
    uint8_t *data;

    /** Length of the data behind the data pointer. */
    uint16_t len;

    /** Amount of data that this buffer can store. */
    uint16_t size;

    /** Start of the data storage. Not to be accessed directly
     *  (the data pointer should be used instead).
     */
    uint8_t *__buf;
};

struct net_buf
{
    union
    {
        /** Allow placing the buffer into sys_slist_t */
        sys_snode_t node;

        /** Fragments associated with this buffer. */
        struct net_buf *frags;
    };

    /** Reference count. */
    uint8_t ref;

    /** Bit-field of buffer flags. */
    uint8_t flags;

    /** Where the buffer should go when freed up. */
    struct net_buf_pool *pool;

    /* Union for convenience access to the net_buf_simple members, also
     * preserving the old API.
     */
    union
    {
        /* The ABI of this struct must match net_buf_simple */
        struct
        {
            /** Pointer to the start of data in the buffer. */
            uint8_t *data;

            /** Length of the data behind the data pointer. */
            uint16_t len;

            /** Amount of data that this buffer can store. */
            uint16_t size;

            /** Start of the data storage. Not to be accessed
             *  directly (the data pointer should be used
             *  instead).
             */
            uint8_t *__buf;
        };

        struct net_buf_simple b;
    };

    /** System metadata for this buffer. */
    uint32_t user_data[BLE_MESH_NET_BUF_USER_DATA_SIZE];
};

// Define a net_buf_simple stack variable.
#define NET_BUF_SIMPLE_DEFINE(_name, _size) \
    uint8_t net_buf_data_##_name[_size];       \
    struct net_buf_simple _name = {         \
        .data  = net_buf_data_##_name,      \
        .len   = 0,                         \
        .size  = _size,                     \
        .__buf = net_buf_data_##_name,      \
    }

// Define a static net_buf_simple variable.
#define NET_BUF_SIMPLE_DEFINE_STATIC(_name, _size)  \
    static uint8_t net_buf_data_##_name[_size];        \
    static struct net_buf_simple _name = {          \
        .data  = net_buf_data_##_name,              \
        .len   = 0,                                 \
        .size  = _size,                             \
        .__buf = net_buf_data_##_name,              \
    }

// Define a model publication context.
#define BLE_MESH_MODEL_PUB_DEFINE(_name, _update, _msg_len) \
    NET_BUF_SIMPLE_DEFINE_STATIC(bt_mesh_pub_msg_##_name, _msg_len); \
    static struct bt_mesh_model_pub _name = { \
        .update = _update, \
        .msg = &bt_mesh_pub_msg_##_name, \
    }

// A helper to define a health publication context
#define BLE_MESH_HEALTH_PUB_DEFINE(_name, _max_faults) \
    BLE_MESH_MODEL_PUB_DEFINE(_name, NULL, (1 + 3 + (_max_faults)))

#define CONTAINER_OF(ptr, type, field) \
    ((type *)(((char *)(ptr)) - offsetof(type, field)))

/*****************************************list*************************************/

typedef struct
{
    sys_snode_t *head;
    sys_snode_t *tail;
} sys_slist_t;

/*****************************************NVS*************************************/

typedef int (*flash_api_read)( int offset, void *data, unsigned int len );
typedef int (*flash_api_write)( int offset, const void *data, unsigned int len );
typedef int (*flash_api_erase)( int offset, unsigned int len );
typedef int (*flash_api_write_protection)( uint8_t enable );

struct flash_config_info
{
    uint8_t nvs_sector_cnt;
    uint8_t nvs_write_size;
    uint16_t nvs_sector_size;
    uint32_t nvs_store_baddr;
};

struct flash_driver_api
{
    flash_api_read read;
    flash_api_write write;
    flash_api_erase erase;
    flash_api_write_protection write_protection;
};

struct device
{
    struct flash_driver_api api;
    struct flash_config_info info;
};

/*****************************************health_srv*************************************/

/** Mesh Health Server Model Callback */
struct bt_mesh_health_srv_cb
{
    /* Fetch current faults */
    int (*fault_get_cur)( struct bt_mesh_model *model, uint8_t *test_id,
        uint16_t *company_id, uint8_t *faults, uint8_t *fault_count );

    /* Fetch registered faults */
    int (*fault_get_reg)( struct bt_mesh_model *model, uint16_t company_id,
        uint8_t *test_id, uint8_t *faults, uint8_t *fault_count );

    /* Clear registered faults */
    int (*fault_clear)( struct bt_mesh_model *model, uint16_t company_id );

    /* Run a specific test */
    int (*fault_test)( struct bt_mesh_model *model, uint8_t test_id,
        uint16_t company_id );

    /* Attention on */
    void (*attn_on)( struct bt_mesh_model *model );

    /* Attention off */
    void (*attn_off)( struct bt_mesh_model *model );
};

/** Mesh Health Server Model Context */
struct bt_mesh_health_srv
{
    struct bt_mesh_model *model;

    /* Optional callback struct */
    const struct bt_mesh_health_srv_cb *cb;

    /* Attention Timer state */
    struct k_delayed_work attn_timer;
};

/* Heartbeat Publication */
struct bt_mesh_hb_pub
{
    struct k_delayed_work timer;

    uint16_t dst;
    uint16_t count;
    uint8_t period;
    uint8_t ttl;
    uint16_t feat;
    uint16_t net_idx;
};

/* Heartbeat Subscription */
struct bt_mesh_hb_sub
{
    int64_t expiry;

    uint16_t src;
    uint16_t dst;
    uint16_t count;
    uint8_t min_hops;
    uint8_t max_hops;

    /* Optional subscription tracking function */
    void (*func)( uint8_t hops, uint16_t feat );
};

/*****************************************cfg_srv*************************************/

struct cfgEventHdr
{
    uint8_t status;
    uint32_t opcode;
};

typedef struct
{
    struct cfgEventHdr cfgHdr;
    struct bt_mesh_model *model;
} cfg_srv_status_t;

typedef void (*cfg_srv_rsp_handler_t)( const cfg_srv_status_t *val );

/** Mesh Configuration Server Model Context */
struct bt_mesh_cfg_srv
{
    struct bt_mesh_model *model;

    cfg_srv_rsp_handler_t handler;

    uint8_t net_transmit; /* Network Transmit state */
    uint8_t relay; /* Relay Mode state */
    uint8_t relay_retransmit; /* Relay Retransmit state */
    uint8_t beacon; /* Secure Network Beacon state */
    uint8_t gatt_proxy; /* GATT Proxy state */
    uint8_t frnd; /* Friend state */
    uint8_t default_ttl; /* Default TTL */

    struct bt_mesh_hb_pub hb_pub;

    struct bt_mesh_hb_sub hb_sub;
};

/*****************************************cfg_cli*************************************/

struct bt_mesh_cfg_comp_data
{
    struct net_buf_simple *buf;
};

struct bt_mesh_cfg_relay
{
    uint8_t transmit;
};

struct bt_mesh_cfg_mod_pub
{
    uint16_t addr;
    uint16_t app_idx;
    uint8_t cred_flag;
    uint8_t ttl;
    uint8_t period;
    uint8_t transmit;
};

struct bt_mesh_cfg_mod_pub_va
{
    uint16_t addr[16];
    uint16_t app_idx;
    uint8_t cred_flag;
    uint8_t ttl;
    uint8_t period;
    uint8_t transmit;
};

struct bt_mesh_cfg_hb_sub
{
    uint16_t src;
    uint16_t dst;
    uint8_t period;
    uint8_t count;
    uint8_t min;
    uint8_t max;
};

struct bt_mesh_cfg_hb_pub
{
    uint16_t dst;
    uint8_t count;
    uint8_t period;
    uint8_t ttl;
    uint16_t feat;
    uint16_t net_idx;
};

struct bt_mesh_cfg_lpn_timeout
{
    uint16_t addr;
    uint32_t polltimeout;
};

struct bt_mesh_cfg_net_transmit
{
    uint8_t transmit;
};

struct bt_mesh_node_id
{
    uint8_t identity;
    uint16_t net_idx;
};

struct bt_mesh_krp
{
    uint8_t phase;
    uint16_t net_idx;
};

struct bt_mesh_cfg_app_list
{
    uint16_t elem_addr;
    uint16_t mod_id;
    uint16_t cid;
    uint16_t *app_lists;
    uint8_t app_count;
};

struct bt_mesh_app_key_list
{
    uint16_t net_idx;
    uint16_t *app_key_lists;
    uint8_t app_key_count;
};

struct bt_mesh_sub_list
{
    uint16_t elem_addr;
    uint16_t cid;
    uint16_t mod_id;
    uint16_t *addr;
    uint8_t addr_count;
};

union cfgEvent_t
{
    struct bt_mesh_cfg_comp_data comp;
    struct bt_mesh_cfg_relay relay;
    struct bt_mesh_cfg_mod_pub m_pub;
    struct bt_mesh_cfg_hb_pub h_pub;
    struct bt_mesh_cfg_hb_sub sub;
    struct bt_mesh_cfg_lpn_timeout lpn;
    struct bt_mesh_cfg_net_transmit transmit;
    struct bt_mesh_node_id id;
    struct bt_mesh_krp krp;
    struct bt_mesh_cfg_app_list aplist;
    struct bt_mesh_app_key_list apklist;
    struct bt_mesh_sub_list sublist;
};

typedef struct
{
    struct cfgEventHdr cfgHdr;
    union cfgEvent_t cfgEvent;
} cfg_cli_status_t;

typedef void (*cfg_cli_rsp_handler_t)( const cfg_cli_status_t *val );

/** Mesh Configuration Client Model Context */
struct bt_mesh_cfg_cli
{
    struct bt_mesh_model *model;

    cfg_cli_rsp_handler_t handler;

    uint32_t op_req;
    uint32_t op_pending;

    struct k_delayed_work op_sync;
};

/*****************************************adapt*************************************/
typedef int (*gatts_notify_cb_t)( struct bt_mesh_conn *conn, const void *data,
    uint16_t len );

/*****************************************proxy*************************************/

typedef void (*proxy_beacon_init_cb_t)( void *client );
typedef int (*proxy_prov_enable_cb_t)( void );
typedef int (*proxy_gatt_enable_cb_t)( void );

/*****************************************friend*************************************/

#define FRIEND_FRIENDSHIP_ESTABLISHED     0x01
#define FRIEND_FRIENDSHIP_TERMINATED        0x02

typedef int (*friend_init_cb_t)( void );
typedef void (*friend_state_cb_t)( uint16_t lpn_addr, uint8_t state );

/*****************************************lpn*************************************/

#define LPN_FRIENDSHIP_ESTABLISHED      0x01
#define LPN_FRIENDSHIP_TERMINATED       0x02

typedef int (*lpn_init_cb_t)( void );
typedef void (*lpn_state_cb_t)( uint16_t friend_addr, uint8_t state );

/*****************************************app_rf*************************************/

typedef void (*pfnAppRFStatusCB_t)( uint8_t sta, uint8_t rsr, uint8_t *rxBuf );
// sta - current status@ref RF_ROLE_STATUS_TYPE
// rsr - receive status: bit0- crc check result,bit1- type matching result
// rxBuf - receive data buffer

typedef struct app_tag_rf_config
{
    uint8_t LLEMode;                              // BIT0   0=LLE_MODE_BASIC
                                             // BIT6   0=data channel(0-39)
                                             //        1=rf frequency (2400000kHz-2483500kHz)
                                             // BIT7   0=the first byte of the receive buffer is rssi
                                             //        1=the first byte of the receive buffer is package type
    uint8_t Channel;                              // rf channel(0-39)
    uint32_t Frequency;                           // rf frequency (2400000kHz-2483500kHz)
    uint32_t accessAddress;                       // access address,32bit PHY address
    uint32_t CRCInit;                             // crc initial value
    pfnAppRFStatusCB_t rfStatusCB;           // status call back
} app_rfConfig_t;

/*****************************************buf*************************************/

/**
 * @brief Add the length of already used buffer in buffer struct.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] len          length to be added
 *
 * @note  No valid data added in buffer.
 *
 * @return The tail pointer for a buffer.
 */
void *net_buf_simple_add( struct net_buf_simple *buf, size_t len );
#define net_buf_add(buf, len) net_buf_simple_add(&(buf)->b, len)

/**
 * @brief Add data to buffer in buffer struct.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] mem          data to be added
 * @param[in] len          length to be added
 *
 * @return Null.
 */
void *net_buf_simple_add_mem( struct net_buf_simple *buf, const void *mem,
    size_t len );
#define net_buf_add_mem(buf, mem, len) net_buf_simple_add_mem(&(buf)->b, mem, len)

/**
 * @brief Add 8bit value to buffer in buffer struct.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] val          value to be added
 *
 * @return Value to be added.
 */
uint8_t *net_buf_simple_add_u8( struct net_buf_simple *buf, uint8_t val );
#define net_buf_add_u8(buf, val) net_buf_simple_add_u8(&(buf)->b, val)

/**
 * @brief Add little-endian 16bit value to buffer in buffer struct.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] val          value to be added
 *
 * @return Null.
 */
void net_buf_simple_add_le16( struct net_buf_simple *buf, uint16_t val );
#define net_buf_add_le16(buf, val) net_buf_simple_add_le16(&(buf)->b, val)

/**
 * @brief Add big-endian 16bit value to buffer in buffer struct.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] val          value to be added
 *
 * @return Null.
 */
void net_buf_simple_add_be16( struct net_buf_simple *buf, uint16_t val );
#define net_buf_add_be16(buf, val) net_buf_simple_add_be16(&(buf)->b, val)

/**
 * @brief Add little-endian 32bit value to buffer in buffer struct.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] val          value to be added
 *
 * @return Null.
 */
void net_buf_simple_add_le32( struct net_buf_simple *buf, uint32_t val );
#define net_buf_add_le32(buf, val) net_buf_simple_add_le32(&(buf)->b, val)

/**
 * @brief Add big-endian 32bit value to buffer in buffer struct.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] val          value to be added
 *
 * @return Null.
 */
void net_buf_simple_add_be32( struct net_buf_simple *buf, uint32_t val );
#define net_buf_add_be32(buf, val) net_buf_simple_add_be32(&(buf)->b, val)

/**
 * @brief Push the data to the start of the buffer.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] len          length to be pushed
 *
 * @return The start pointer for a buffer after pushed.
 */
void *net_buf_simple_push( struct net_buf_simple *buf, size_t len );
#define net_buf_push(buf, len) net_buf_simple_push(&(buf)->b, len)

/**
 * @brief Push little-endian 16bit value to buffer.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] val          value to be pushed
 *
 * @return Null.
 */
void net_buf_simple_push_le16( struct net_buf_simple *buf, uint16_t val );
#define net_buf_push_le16(buf, val) net_buf_simple_push_le16(&(buf)->b, val)

/**
 * @brief Push big-endian 16bit value to buffer.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] val          value to be pushed
 *
 * @return Null.
 */
void net_buf_simple_push_be16( struct net_buf_simple *buf, uint16_t val );
#define net_buf_push_be16(buf, val) net_buf_simple_push_be16(&(buf)->b, val)

/**
 * @brief Push 8bit value to buffer.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] val          value to be pushed
 *
 * @return Null.
 */
void net_buf_simple_push_u8( struct net_buf_simple *buf, uint8_t val );
#define net_buf_push_u8(buf, val) net_buf_simple_push_u8(&(buf)->b, val)

/**
 * @brief Pull the data from the start of the buffer.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] len          length to be pulled
 *
 * @return The start pointer for a buffer after pulled.
 */
void *net_buf_simple_pull( struct net_buf_simple *buf, size_t len );
#define net_buf_pull(buf, len) net_buf_simple_pull(&(buf)->b, len)

/**
 * @brief Pull the data from the start of the buffer.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] len          length to be pulled
 *
 * @return The pointer for the data be pulled.
 */
void *net_buf_simple_pull_mem( struct net_buf_simple *buf, size_t len );
#define net_buf_pull_mem(buf, len) net_buf_simple_pull_mem(&(buf)->b, len)

/**
 * @brief Pull 8bit value from buffer.
 *
 * @param[in] buf          buffer struct to be operated
 *
 * @return The value be pulled.
 */
uint8_t net_buf_simple_pull_u8( struct net_buf_simple *buf );
#define net_buf_pull_u8(buf) net_buf_simple_pull_u8(&(buf)->b)

/**
 * @brief Pull little-endian 16bit value from buffer.
 *
 * @param[in] buf          buffer struct to be operated
 *
 * @return The value be pulled.
 */
uint16_t net_buf_simple_pull_le16( struct net_buf_simple *buf );
#define net_buf_pull_le16(buf) net_buf_simple_pull_le16(&(buf)->b)

/**
 * @brief Pull big-endian 16bit value from buffer.
 *
 * @param[in] buf          buffer struct to be operated
 *
 * @return The value be pulled.
 */
uint16_t net_buf_simple_pull_be16( struct net_buf_simple *buf );
#define net_buf_pull_be16(buf) net_buf_simple_pull_be16(&(buf)->b)

/**
 * @brief Pull little-endian 32bit value from buffer.
 *
 * @param[in] buf          buffer struct to be operated
 *
 * @return The value be pulled.
 */
uint32_t net_buf_simple_pull_le32( struct net_buf_simple *buf );
#define net_buf_pull_le32(buf) net_buf_simple_pull_le32(&(buf)->b)

/**
 * @brief Pull big-endian 32bit value from buffer.
 *
 * @param[in] buf          buffer struct to be operated
 *
 * @return The value be pulled.
 */
uint32_t net_buf_simple_pull_be32( struct net_buf_simple *buf );
#define net_buf_pull_be32(buf) net_buf_simple_pull_be32(&(buf)->b)

/**
 * @brief Check how much free space there is at the end of the buffer.
 *
 * @param[in] buf          buffer struct to be operated
 *
 * @return Size of free space.
 */
size_t net_buf_simple_tailroom( struct net_buf_simple *buf );
#define net_buf_tailroom(buf) net_buf_simple_tailroom(&(buf)->b)

/**
 * @brief Check how much free space there is at the start of the buffer.
 *
 * @param[in] buf          buffer struct to be operated
 *
 * @return Size of free space.
 */
size_t net_buf_simple_headroom( struct net_buf_simple *buf );
#define net_buf_headroom(buf) net_buf_simple_headroom(&(buf)->b)

/**
 * @brief Reserve space at the start of the buffer.
 *
 * @param[in] buf          buffer struct to be operated
 * @param[in] reserve      size to be reserved
 *
 * @return Null.
 */
void net_buf_simple_reserve( struct net_buf_simple *buf, size_t reserve );
#define net_buf_reserve(buf, reserve) net_buf_simple_reserve(&(buf)->b, reserve);

/**
 * @brief Destroy the buffer.
 *
 * @param[in] buf          buffer struct to be operated
 *
 * @return Null.
 */
void net_buf_unref( struct net_buf *buf );

/*****************************************sha256*************************************/

/**
 * @brief Init the sha256.
 *
 * @param[in] s          @ref tc_sha256_state_struct
 *
 * @return TC_CRYPTO_SUCCESS or TC_CRYPTO_FAIL.
 */
int tc_sha256_init( TCSha256State_t s );

/**
 * @brief Update the sha256.
 *
 * @param[in] s          @ref tc_sha256_state_struct
 * @param[in] data       data to update
 * @param[in] datalen    length of data
 *
 * @return TC_CRYPTO_SUCCESS or TC_CRYPTO_FAIL.
 */
int tc_sha256_update( TCSha256State_t s, const uint8_t *data, size_t datalen );

/**
 * @brief Sha256 final.
 *
 * @param[in] digest     sha256 output
 * @param[in] s          @ref tc_sha256_state_struct
 *
 * @return TC_CRYPTO_SUCCESS or TC_CRYPTO_FAIL.
 */
int tc_sha256_final( uint8_t *digest, TCSha256State_t s );

/*****************************************SNV*************************************/

/**
 * @brief Load settings from flash.
 *
 * @return @ref Global_Error_Code.
 */
int settings_load( void );

/**
 * @brief Init NVS settings store.
 */
void bt_mesh_settings_init( void );

/*****************************************beacon*************************************/

/**
 * @brief Init beacon.
 */
void bt_mesh_beacon_init( void );

/*****************************************cfg_srv*************************************/

/**
 * @brief Init relay.
 */
void bt_mesh_relay_init( void );

/**
 * @brief Get transmission count & interval steps of net @ref BLE_MESH_TRANSMIT.
 *
 * @return transmission count & interval steps.
 */
uint8_t bt_mesh_net_transmit_get( void );

/**
 * @brief Is relay enable.
 *
 * @return @ref Function_State_Define.
 */
uint8_t bt_mesh_relay_get( void );

/**
 * @brief Is friend enable.
 *
 * @return @ref Function_State_Define.
 */
uint8_t bt_mesh_friend_get( void );

/**
 * @brief Get transmission count & interval steps of relay @ref BLE_MESH_TRANSMIT.
 *
 * @return transmission count & interval steps.
 */
uint8_t bt_mesh_relay_retransmit_get( void );

/**
 * @brief Is beacon enable.
 *
 * @return @ref Function_State_Define.
 */
uint8_t bt_mesh_beacon_get( void );

/**
 * @brief Is gatt proxy support.
 *
 * @return @ref Function_State_Define.
 */
uint8_t bt_mesh_gatt_proxy_get( void );

/**
 * @brief Get default TTL.
 *
 * @return TTL of net.
 */
uint8_t bt_mesh_default_ttl_get( void );

/**
 * @brief Get label uuid by address.
 *
 * @param[in] addr          The address of label uuid
 *
 * @return 16 byte label uuid.
 */
uint8_t *bt_mesh_label_uuid_get( uint16_t addr );

/**
 * @brief Set application key.
 *
 * @param[in] net_idx       Pointer to 16 byte network key.
 * @param[in] app_idx       Pointer to 16 byte application key.
 * @param[in] val           16 byte application key.
 * @param[in] update        Whether it is in the process of key update.
 *
 * @return @ref Statue_Error_Code.
 */
uint8_t bt_mesh_app_key_set(uint16_t net_idx, uint16_t app_idx, const uint8_t val[16],
        uint8_t update);

/**
 * @brief Deletes stored info by address.
 *
 * @param[in] addr          Address of replay item.
 * @param[in] num_elem      Numble of node element.
 *
 * @return none.
 */
void bt_mesh_delete_node_info(uint16_t addr, uint8_t num_elem);

/*****************************************cfg_cli*************************************/

/**
 * @brief Sends a composition data GET request.
 *
 * @note Request:  @ref OP_DEV_COMP_DATA_GET
 * @note Response: @ref OP_DEV_COMP_DATA_STATUS
 *
 * @param[in] page          Device composition page number to be requested from the server
 *
 * @note Page 0x00 is the only mandatory page in Mesh 1.0.
 *       It is possible to read all supported Composition Data Pages by reading 0xFF first,
 *       and then reading one less than the returned page number until the page number is 0x00.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_comp_data_get( uint16_t net_idx, uint16_t addr, uint8_t page );

/**
 * @brief Sends a secure network beacon state get request.
 *
 * @note Request:  @ref OP_BEACON_GET
 * @note Response: @ref OP_BEACON_STATUS
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_beacon_get( uint16_t net_idx, uint16_t addr );

/**
 * @brief Sends a secure network beacon state set request.
 *
 * @note Request:  @ref OP_BEACON_SET
 * @note Response: @ref OP_BEACON_STATUS
 *
 * @param[in] state New secure network beacon state.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_beacon_set( uint16_t net_idx, uint16_t addr, uint8_t val );

/**
 * @brief Sends a default TTL get request.
 *
 * @note Request:  @ref OP_DEFAULT_TTL_GET
 * @note Response: @ref OP_DEFAULT_TTL_STATUS
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_ttl_get( uint16_t net_idx, uint16_t addr );

/**
 * @brief Sends a default TTL set request.
 *
 * @note Request:  @ref OP_DEFAULT_TTL_SET
 * @note Response: @ref OP_DEFAULT_TTL_STATUS
 *
 * @param[in] val Default TTL value. Must be less than @ref BLE_MESH_TTL_MAX.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_ttl_set( uint16_t net_idx, uint16_t addr, uint8_t val );

/**
 * @brief Sends a friend state get request.
 *
 * @note Request:  @ref OP_FRIEND_GET
 * @note Response: @ref OP_FRIEND_STATUS
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_friend_get( uint16_t net_idx, uint16_t addr );

/**
 * @brief Sends a friend state set request.
 *
 * @note Request:  @ref OP_FRIEND_SET
 * @note Response: @ref OP_FRIEND_STATUS
 *
 * @param[in] val   New friend state.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_friend_set( uint16_t net_idx, uint16_t addr, uint8_t val );

/**
 * @brief Sends a gatt proxy state get request.
 *
 * @note Request:  @ref OP_GATT_PROXY_GET
 * @note Response: @ref OP_GATT_PROXY_STATUS
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_gatt_proxy_get( uint16_t net_idx, uint16_t addr );

/**
 * @brief Sends a gatt proxy state set request.
 *
 * @note Request:  @ref OP_GATT_PROXY_SET
 * @note Response: @ref OP_GATT_PROXY_STATUS
 *
 * @param[in] val   New gatt proxy state.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_gatt_proxy_set( uint16_t net_idx, uint16_t addr, uint8_t val );

/**
 * @brief Sends a relay state get request.
 *
 * @note Request:  @ref OP_RELAY_GET
 * @note Response: @ref OP_RELAY_STATUS
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_relay_get( uint16_t net_idx, uint16_t addr );

/**
 * @brief Sends a relay state set request.
 *
 * @note Request:  @ref OP_RELAY_SET
 * @note Response: @ref OP_RELAY_STATUS
 *
 * @param[in] new_relay                 Relay state.
 * @param[in] new_transmit              Transmission count & interval steps @ref BLE_MESH_TRANSMIT.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_relay_set( uint16_t net_idx, uint16_t addr, uint8_t new_relay,
    uint8_t new_transmit );

/**
 * @brief Sends a network key(s) get request.
 *
 * @note Request:  @ref OP_NET_KEY_GET
 * @note Response: @ref OP_NET_KEY_LIST
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_net_key_get( uint16_t net_idx, uint16_t addr );

/**
 * @brief Sends a network key add request.
 *
 * @note Request:  @ref OP_NET_KEY_ADD
 * @note Response: @ref OP_NET_KEY_STATUS
 *
 * @param[in] key_net_idx   Network key index.
 * @param[in] net_key       Pointer to 16 byte network key.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_net_key_add( uint16_t net_idx, uint16_t addr, uint16_t key_net_idx,
    const uint8_t net_key[16] );

/**
 * @brief Sends a network key update request.
 *
 * @note Request:  @ref OP_NET_KEY_UPDATE
 * @note Response: @ref OP_NET_KEY_STATUS
 *
 * @param[in] key_net_idx   Network key index.
 * @param[in] net_key       Pointer to 16 byte network key.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_net_key_update( uint16_t net_idx, uint16_t addr, uint16_t key_net_idx,
    const uint8_t net_key[16] );

/**
 * @brief Sends a network key delete request.
 *
 * @note Request:  @ref OP_NET_KEY_DEL
 * @note Response: @ref OP_NET_KEY_STATUS
 *
 * @param[in] key_net_idx   Network key index.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_net_key_delete( uint16_t net_idx, uint16_t addr, uint16_t key_net_idx );

/**
 * @brief Sends an application key add request.
 *
 * @note Request:  @ref OP_APP_KEY_ADD
 * @note Response: @ref OP_APP_KEY_STATUS
 *
 * @param[in] key_net_idx   Network key index.
 * @param[in] key_app_idx   Application key index.
 * @param[in] app_key       Pointer to 16 byte application key.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_app_key_add( uint16_t net_idx, uint16_t addr, uint16_t key_net_idx,
    uint16_t key_app_idx, const uint8_t app_key[16] );

/**
 * @brief Sends an application key update request.
 *
 * @note Request:  @ref OP_APP_KEY_UPDATE
 * @note Response: @ref OP_APP_KEY_STATUS
 *
 * @param[in] key_net_idx   Network key index.
 * @param[in] key_app_idx   Application key index.
 * @param[in] app_key       Pointer to 16 byte application key.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_app_key_update( uint16_t net_idx, uint16_t addr, uint16_t key_net_idx,
    uint16_t key_app_idx, const uint8_t app_key[16] );

/**
 * @brief Sends an application key delete request.
 *
 * @note Request:  @ref OP_APP_KEY_DEL
 * @note Response: @ref OP_APP_KEY_STATUS
 *
 * @param[in] key_net_idx   Network key index.
 * @param[in] key_app_idx   Application key index.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_app_key_delete( uint16_t net_idx, uint16_t addr, uint16_t key_net_idx,
    uint16_t key_app_idx );

/**
 * @brief Sends an application bind request.
 *
 * @note Request:  @ref OP_MOD_APP_BIND
 * @note Response: @ref OP_MOD_APP_STATUS
 *
 * @param[in] elem_addr       Element address of the model.
 * @param[in] mod_app_idx     Application key index to bind/unbind.
 * @param[in] mod_id          Model ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_app_bind( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_app_idx, uint16_t mod_id );

/**
 * @brief Sends an vendor application bind request.
 *
 * @note Request:  @ref OP_MOD_APP_BIND
 * @note Response: @ref OP_MOD_APP_STATUS
 *
 * @param[in] elem_addr       Element address of the model.
 * @param[in] mod_app_idx     Application key index to bind/unbind.
 * @param[in] mod_id          Model ID of the model.
 * @param[in] cid             Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_app_bind_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_app_idx, uint16_t mod_id, uint16_t cid );

/**
 * @brief Sends an application unbind request.
 *
 * @note Request:  @ref OP_MOD_APP_UNBIND
 * @note Response: @ref OP_MOD_APP_STATUS
 *
 * @param[in] element_address Element address of the model.
 * @param[in] appkey_index    Application key index to bind/unbind.
 * @param[in] model_id        Model ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_app_unbind( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_app_idx, uint16_t mod_id );

/**
 * @brief Sends an vendor application unbind request.
 *
 * @note Request:  @ref OP_MOD_APP_UNBIND
 * @note Response: @ref OP_MOD_APP_STATUS
 *
 * @param[in] element_address Element address of the model.
 * @param[in] appkey_index    Application key index to bind/unbind.
 * @param[in] model_id        Model ID of the model.
 * @param[in] cid             Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_app_unbind_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_app_idx, uint16_t mod_id, uint16_t cid );

/**
 * @brief Sends a publication get request.
 *
 * @note Request:  @ref OP_MOD_PUB_GET
 * @note Response: @ref OP_MOD_PUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] mod_id        Model identifier.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_pub_get( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id );

/**
 * @brief Sends an vendor publication get request.
 *
 * @note Request:  @ref OP_MOD_PUB_GET
 * @note Response: @ref OP_MOD_PUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] mod_id        Model identifier.
 * @param[in] cid             Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_pub_get_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id, uint16_t cid );

/**
 * @brief Sends a model publication set request.
 *
 * @note Request:  @ref OP_MOD_PUB_SET
 * @note Response: @ref OP_MOD_PUB_STATUS
 *
 * @param[in] pub   Publication state parameter struct pointer.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_pub_set( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id, const struct bt_mesh_cfg_mod_pub *pub );

/**
 * @brief Sends an vendor model publication set request.
 *
 * @note Request:  @ref OP_MOD_PUB_SET
 * @note Response: @ref OP_MOD_PUB_STATUS
 *
 * @param[in] pub   Publication state parameter struct pointer.
 * @param[in] cid             Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_pub_set_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id, uint16_t cid, const struct bt_mesh_cfg_mod_pub *pub );

/**
 * @brief Sends a model virtual address publication set request.
 *
 * @note Request:  @ref OP_MOD_PUB_VA_SET
 * @note Response: @ref OP_MOD_PUB_STATUS
 *
 * @param[in] pub   Virtual address publication state parameter struct pointer.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_pub_va_set( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id, const struct bt_mesh_cfg_mod_pub_va *pub );

/**
 * @brief Sends an vendor model virtual address publication set request.
 *
 * @note Request:  @ref OP_MOD_PUB_VA_SET
 * @note Response: @ref OP_MOD_PUB_STATUS
 *
 * @param[in] pub           Virtual address publication state parameter struct pointer.
 * @param[in] cid           Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_pub_va_set_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id, uint16_t cid, const struct bt_mesh_cfg_mod_pub_va *pub );

/**
 * @brief Sends a subscription add request.
 *
 * @note Request:  @ref OP_MOD_SUB_ADD
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] sub_addr      Address to add to the subscription list.
 * @param[in] mod_id        Model ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_add( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t sub_addr, uint16_t mod_id );

/**
 * @brief Sends an vendor subscription add request.
 *
 * @note Request:  @ref OP_MOD_SUB_ADD
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] sub_addr      Address to add to the subscription list.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] cid           Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_add_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t sub_addr, uint16_t mod_id, uint16_t cid );

/**
 * @brief Sends a subscription delete request.
 *
 * @note Request:  @ref OP_MOD_SUB_DEL
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] sub_addr      Address to add to the subscription list.
 * @param[in] mod_id        Model ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_del( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t sub_addr, uint16_t mod_id );

/**
 * @brief Sends an vendor subscription delete request.
 *
 * @note Request:  @ref OP_MOD_SUB_DEL
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] sub_addr      Address to add to the subscription list.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] cid           Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_del_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t sub_addr, uint16_t mod_id, uint16_t cid );

/**
 * @brief Sends a subscription delete all request.
 *
 * @note Request:  @ref OP_MOD_SUB_DEL_ALL
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] mod_id        Model ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_del_all( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id );

/**
 * @brief Sends an vendor subscription delete all request.
 *
 * @note Request:  @ref OP_MOD_SUB_DEL_ALL
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] cid           Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_delall_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id, uint16_t cid );

/**
 * @brief Sends a subscription overwrite request.
 *
 * @note Request:  @ref OP_MOD_SUB_OVERWRITE
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @warning This will clear the subscription list of the model.
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] sub_addr      Address to add to the subscription list.
 * @param[in] mod_id        Model ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_overwrite( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t sub_addr, uint16_t mod_id );

/**
 * @brief Sends an vendor subscription overwrite request.
 *
 * @note Request:  @ref OP_MOD_SUB_OVERWRITE
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @warning This will clear the subscription list of the model.
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] sub_addr      Address to add to the subscription list.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] cid           Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_overwrite_vnd( uint16_t net_idx, uint16_t addr,
    uint16_t elem_addr, uint16_t sub_addr, uint16_t mod_id, uint16_t cid );

/**
 * @brief Sends an virtual address subscription add request.
 *
 * @note Request:  @ref OP_MOD_SUB_VA_ADD
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] label         Value of the Label UUID.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] virt_addr     Must be set to NULL.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_va_add( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    const uint8_t label[16], uint16_t mod_id, uint16_t virt_addr );

/**
 * @brief Sends an vendor virtual address subscription add request.
 *
 * @note Request:  @ref OP_MOD_SUB_VA_ADD
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] label         Value of the Label UUID.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] cid           Company ID of the model.
 * @param[in] virt_addr     Must be set to NULL.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_va_add_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    const uint8_t label[16], uint16_t mod_id, uint16_t cid, uint16_t virt_addr );

/**
 * @brief Sends an virtual address subscription delete request.
 *
 * @note Request:  @ref OP_MOD_SUB_VA_DEL
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] label         Value of the Label UUID.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] virt_addr     Must be set to NULL.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_va_del( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    const uint8_t label[16], uint16_t mod_id, uint16_t virt_addr );

/**
 * @brief Sends an vendor virtual address subscription delete request.
 *
 * @note Request:  @ref OP_MOD_SUB_VA_DEL
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] label         Value of the Label UUID.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] cid           Company ID of the model.
 * @param[in] virt_addr     Must be set to NULL.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_va_del_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    const uint8_t label[16], uint16_t mod_id, uint16_t cid, uint16_t virt_addr );

/**
 * @brief Sends an virtual address subscription overwrite request.
 *
 * @note Request:  @ref OP_MOD_SUB_VA_OVERWRITE
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] label         Value of the Label UUID.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] virt_addr     Must be set to NULL.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_va_overwrite( uint16_t net_idx, uint16_t addr,
    uint16_t elem_addr, const uint8_t label[16], uint16_t mod_id, uint16_t virt_addr );

/**
 * @brief Sends an vendor virtual address subscription overwrite request.
 *
 * @note Request:  @ref OP_MOD_SUB_VA_OVERWRITE
 * @note Response: @ref OP_MOD_SUB_STATUS
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] label         Value of the Label UUID.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] cid           Company ID of the model.
 * @param[in] virt_addr     Must be set to NULL.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_va_overwrite_vnd( uint16_t net_idx, uint16_t addr,
    uint16_t elem_addr, const uint8_t label[16], uint16_t mod_id, uint16_t cid,
    uint16_t virt_addr );

/**
 * @brief Sets the heartbeat publication state value of a node.
 *
 * @note Request:  @ref OP_HEARTBEAT_PUB_SET
 * @note Response: @ref OP_HEARTBEAT_PUB_STATUS
 *
 * @param[in]  param    Pointer to the @ref bt_mesh_cfg_hb_pub structure
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_hb_pub_set( uint16_t net_idx, uint16_t addr,
    const struct bt_mesh_cfg_hb_pub *param );

/**
 * @brief Gets the heartbeat publication state value of a node.
 *
 * @note Request:  @ref OP_HEARTBEAT_PUB_GET
 * @note Response: @ref OP_HEARTBEAT_PUB_STATUS
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_hb_pub_get( uint16_t net_idx, uint16_t addr );

/**
 * @brief Gets the lpn poll timeout of a node.
 *
 * @note Request:  @ref OP_LPN_TIMEOUT_GET
 * @note Response: @ref OP_LPN_TIMEOUT_STATUS
 *
 * @param[in] lpn_addr          The address of lpn.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_lpn_polltimeout_get( uint16_t net_idx, uint16_t addr, uint16_t lpn_addr );

/**
 * @brief Sends a network transmit get request.
 *
 * @note Request:  @ref OP_NET_TRANSMIT_GET
 * @note Response: @ref OP_NET_TRANSMIT_STATUS
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_net_transmit_get( uint16_t net_idx, uint16_t addr );

/**
 * @brief Sends a network transmit set request.
 *
 * @note Request:  @ref OP_NET_TRANSMIT_SET
 * @note Response: @ref OP_NET_TRANSMIT_STATUS
 *
 * @param[in] val          Transmission count & interval steps @ref BLE_MESH_TRANSMIT.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_net_transmit_set( uint16_t net_idx, uint16_t addr, uint8_t val );

/**
 * @brief Sends a node reset request.
 *
 * @note Request:  @ref OP_NODE_RESET
 * @note Response: @ref OP_NODE_RESET_STATUS
 *
 * @warning This will "un-provision" the node and remove it from the network.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_node_reset( uint16_t net_idx, uint16_t addr );

/**
 * @brief Sends a node identity get request.
 *
 * @note Request:  @ref OP_NODE_IDENTITY_GET
 * @note Response: @ref OP_NODE_IDENTITY_STATUS
 *
 * @param[in] id_net_idx    Node identity index.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_node_id_get( uint16_t net_idx, uint16_t addr, uint16_t id_net_idx );

/**
 * @brief Sends a node identity set request.
 *
 * @note Request:  @ref OP_NODE_IDENTITY_SET
 * @note Response: @ref OP_NODE_IDENTITY_STATUS
 *
 * @param[in] id_net_idx    Node identity index.
 * @param[in] val           Value of node identity.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_node_id_set( uint16_t net_idx, uint16_t addr, uint16_t id_net_idx,
    uint8_t val );

/**
 * @brief Gets the current key refresh phase of a node.
 *
 * @note Request:  @ref OP_KRP_GET
 * @note Response: @ref OP_KRP_STATUS
 *
 * @param[in] krp_net_idx   Network key index.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_krp_get( uint16_t net_idx, uint16_t addr, uint16_t krp_net_idx );

/**
 * @brief Sets the current key refresh phase of a node.
 *
 * @note Request:  @ref OP_KRP_SET
 * @note Response: @ref OP_KRP_STATUS
 *
 * @param[in] krp_net_idx   Network key index.
 * @param[in] val           Key refresh phase to set for the node.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_krp_set( uint16_t net_idx, uint16_t addr, uint16_t krp_net_idx,
    uint8_t val );

/**
 * @brief Sends a SIG models application get request.
 *
 * @note Request:  @ref OP_SIG_MOD_APP_GET
 * @note Response: @ref OP_SIG_MOD_APP_LIST
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] mod_id        Model ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_app_get_sig( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id );

/**
 * @brief Sends an vendor models application get request.
 *
 * @note Request:  @ref OP_VND_MOD_APP_GET
 * @note Response: @ref OP_VND_MOD_APP_LIST
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] cid           Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_app_get_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id, uint16_t cid );

/**
 * @brief Sends an application key(s) get request.
 *
 * @note Request:  @ref OP_APP_KEY_GET
 * @note Response: @ref OP_APP_KEY_LIST
 *
 * @param[in] key_net_idx   Network key index.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_app_key_get( uint16_t net_idx, uint16_t addr, uint16_t key_net_idx );

/**
 * @brief Sends a SIG models subscription get request.
 *
 * @note Request:  @ref OP_MOD_SUB_GET
 * @note Response: @ref OP_MOD_SUB_LIST
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] mod_id        Model ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_get( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id );

/**
 * @brief Sends an vendor models subscription get request.
 *
 * @note Request:  @ref OP_MOD_SUB_GET_VND
 * @note Response: @ref OP_MOD_SUB_LIST_VND
 *
 * @param[in] elem_addr     Element address of the model.
 * @param[in] mod_id        Model ID of the model.
 * @param[in] cid           Company ID of the model.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_mod_sub_get_vnd( uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
    uint16_t mod_id, uint16_t cid );

/**
 * @brief Get the client request timeout for waiting response.
 *
 * @return The client request timeout.
 */
int32_t bt_mesh_cfg_cli_timeout_get( void );

/**
 * @brief Set the client request timeout for waiting response.
 *
 * @param[in] timeout        The client request timeout.
 */
void bt_mesh_cfg_cli_timeout_set( int32_t timeout );

/******************************************************************************/

/**
 * @brief Get the state of mesh provision.
 *
 * @return is mesh provisioned.
 */
uint8_t bt_mesh_is_provisioned( void );

/**
 * @brief Config the mesh.
 *
 * @param[in] p_cfg        Mesh parameter config.
 * @param[in] p_dev        Device flash config.
 * @param[in] p_mac        MAC config.
 * @param[in] info         Memory config.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_cfg_set( ble_mesh_cfg_t const *p_cfg, void const *p_dev,
    uint8_t const * p_mac, mem_info_t *info );

/**
 * @brief Provision the self.
 *
 * @param[in] net_key         The key of net to be provisioned.
 * @param[in] net_idx         The index of net key.
 * @param[in] flags           Net key refresh flag.
 * @param[in] iv_index        IV Index.
 * @param[in] addr            The address of primary element.
 * @param[in] dev_key         Device key.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_provision( const uint8_t net_key[16], uint16_t net_idx, uint8_t flags,
    uint32_t iv_index, uint16_t addr, const uint8_t dev_key[16] );

/**
 * @brief Provisions a device on ADV bearer.
 *
 * @param[in] uuid                      Device UUID of the device that is to be provisioned.
 * @param[in] net_idx                   The index of net key.
 * @param[in] addr                      The address of primary element of the device.
 * @param[in] attention_duration        Time in seconds during which the device will identify
 *                                      itself using any means it can.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_provision_adv( const uint8_t uuid[16], uint16_t net_idx, uint16_t addr,
    uint8_t attention_duration );

/**
 * @brief Cancle provisions, error code in CB is timeout.
 */
void bt_mesh_provision_cancle( void );

/*****************************************adapt*************************************/

/**
 * @brief Initialize GATT bearer.
 */
void bt_mesh_adapt_init( void );

/**
 * @brief Initialize ADV bearer.
 */
void bt_mesh_adv_init( void );

/**
 * @brief Initialize connectable advertising of GATT bearer.
 */
void bt_mesh_conn_adv_init( void );

/**
 * @brief Enable scanning.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_scan_enable( void );

/**
 * @brief Disable scanning.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_scan_disable( void );

/**
 * @brief Enable beacon or unprovisioned advertising.
 */
void bt_mesh_beacon_enable( void );

/**
 * @brief Disable beacon or unprovisioned advertising.
 */
void bt_mesh_beacon_disable( void );

/*****************************************proxy*************************************/

/**
 * @brief Initialize proxy beacon, used by @fun bt_mesh_proxy_beacon_init_register.
 *
 * @param[in] client      The client of proxy connect.
 */
void bt_mesh_proxy_beacon_init( void *client );

/**
 * @brief Register proxy beacon initialize function.
 *
 * @param[in] client      The client of proxy connect.
 */
void bt_mesh_proxy_beacon_init_register( void *cb );

/**
 * @brief Send notify to the given Mesh GATT connection, used by @fun gatts_notify_register.
 *
 * @param[in] conn      Mesh GATT connection.
 * @param[in] data      Data to be send.
 * @param[in] len       Length of data.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_gatts_notify( struct bt_mesh_conn *conn, const void *data,
    uint16_t len );

/**
 * @brief Register notify callback of GATT bearer.
 *
 * @param[in] cb         Notify callback of GATT.
 */
void gatts_notify_register( gatts_notify_cb_t cb );

/**
 * @brief Enable proxy GATT service function, used by @fun proxy_gatt_enable_register.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_proxy_gatt_enable( void );

/**
 * @brief Register proxy GATT service enable function.
 *
 * @param[in] cb         Function of enable proxy GATT service.
 */
void proxy_gatt_enable_register( proxy_gatt_enable_cb_t cb );

/**
 * @brief Enable proxy provision function, used by @fun proxy_prov_enable_register.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_proxy_prov_enable( void );

/**
 * @brief Register proxy provision service enable function.
 *
 * @param[in] cb         Function of enable proxy provision service.
 */
void proxy_prov_enable_register( proxy_prov_enable_cb_t cb );

/**
 * @brief Disable proxy provision function.
 *
 * @param[in] disconnect      Weather disconnect proxy connect or not.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_proxy_prov_disable( uint8_t disconnect );

/**
 * @brief Initialize proxy function.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_proxy_init( void );

/**
 * @brief Set proxy adv response data.
 *
 * @param[in] data        SCAN RSP data.
 * @param[in] len         Length of SCAN RSP data (max size = 31 bytes).
 */
void bt_mesh_proxy_set_adv_rsp(u8_t *data, u8_t len);

/**
 * @brief Set proxy adv interval.
 *
 * @param[in] interval    New adv interval in ms.
 */
void bt_mesh_proxy_set_adv_interval(u16_t interval);

/*****************************************prov*************************************/

/**
 * @brief Register receive data buffer of ADV link.
 *
 * @param[in] buf         Buffer to be registered.
 */
void adv_link_rx_buf_register( struct net_buf_simple *buf );

/**
 * @brief Enable provisioner function.
 *
 * @param[in] bearers         Only support BLE_MESH_PROV_ADV.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_provisioner_enable( bt_mesh_prov_bearer_t bearers );

/**
 * @brief Disable provisioner function.
 *
 * @param[in] bearers         Only support BLE_MESH_PROV_ADV.
 * @param[in] force           Weather reset link forcibly or not.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_provisioner_disable( bt_mesh_prov_bearer_t bearers, uint8_t force );

/**
 * @brief Initialize provision retransmit function.
 */
void bt_mesh_prov_retransmit_init( void );

/**
 * @brief Initialize provision function.
 *
 * @param[in] prov_info         Provisioning properties & capabilities.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_prov_init( const struct bt_mesh_prov *prov_info );

/**
 * @brief Delete node by address.
 *
 * @param[in] addr         The address of primary element of the node.
 */
void bt_mesh_node_del_by_addr( uint16_t addr );

/**
 * @brief Delete all node.
 */
void bt_mesh_node_clear( void );

/*****************************************tran*************************************/

/**
 * @brief Initialize translate layer.
 */
void bt_mesh_trans_init( void );

/*****************************************friend*************************************/

/**
 * @brief Initialize friend function, used by @fun friend_init_register.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_friend_init( void );

/**
 * @brief Register friend initialize function.
 *
 * @param[in] cb              The friend initialize function.
 * @param[in] state_cb        The state callback of friend.
 */
void friend_init_register( friend_init_cb_t cb, friend_state_cb_t state_cb );

/*****************************************lpn*************************************/

/**
 * @brief Initialize LPN function, used by @fun lpn_init_register.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_lpn_init( void );

/**
 * @brief Register LPN initialize function.
 *
 * @param[in] cb              The LPN initialize function.
 * @param[in] state_cb        The state callback of LPN.
 */
void lpn_init_register( lpn_init_cb_t cb, lpn_state_cb_t state_cb );

/**
 * @brief Enable or disable LPN function.
 *
 * @param[in] enable           Weather enable LPN function or not.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_lpn_set( uint8_t enable );

/**
 * @brief Add group address to LPN.
 *
 * @param[in] group           The address of group to be added.
 */
void bt_mesh_lpn_group_add( uint16_t group );

/**
 * @brief Delete group address to LPN.
 *
 * @param[in] groups          Pointer to the address of group to be Deleted.
 * @param[in] group_count     Number of the address of group to be Deleted.
 */
void bt_mesh_lpn_group_del( uint16_t *groups, size_t group_count );

/*****************************************net*************************************/

/**
 * @brief Initialize mesh net.
 */
void bt_mesh_net_init( void );

/**
 * @brief Get app keys by net index.
 *
 * @param[in] net_idx          Net index.
 *
 * @return Struct of app key @ref bt_mesh_app_key.
 */
struct bt_mesh_app_key *bt_mesh_app_keys_get( uint16_t net_idx );

/**
 * @brief Find app keys by app index.
 *
 * @param[in] app_idx          App index.
 *
 * @return Struct of app key @ref bt_mesh_app_key.
 */
struct bt_mesh_app_key *bt_mesh_app_key_find( uint16_t app_idx );

/*****************************************NVS*************************************/

/**
 * @brief Store app keys bind of given model.
 *
 * @param[in] mod             The model to be operated.
 */
void bt_mesh_store_mod_bind( struct bt_mesh_model *mod );

/**
 * @brief Store subscription of given model.
 *
 * @param[in] mod             The model to be operated.
 */
void bt_mesh_store_mod_sub( struct bt_mesh_model *mod );

/*****************************************access*************************************/

/**
 * @brief Initialize mesh model.
 */
void bt_mesh_mod_init( void );

/**
 * @brief Register node composition.
 *
 * @param[in] comp             Node composition.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_comp_register( const struct bt_mesh_comp *comp );

/**
 * @brief Get count of element.
 *
 * @return Count of element.
 */
uint8_t bt_mesh_elem_count( void );

/**
 * @brief Get address of primary element.
 *
 * @return The address of primary element.
 */
uint16_t bt_mesh_primary_addr( void );

/**
 * @brief Initialize the message to be send.
 *
 * @param[in] msg               The message to initialize.
 * @param[in] opcode            The opcode of message.
 */
void bt_mesh_model_msg_init( struct net_buf_simple *msg, uint32_t opcode );

/**
 * @brief Send a message of model.
 *
 * @param[in] model               The model need to send message.
 * @param[in] ctx                 Message sending context.
 * @param[in] msg                 The message to send.
 * @param[in] cb                  Send callback.
 * @param[in] cb_data             The data used by send callback.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_model_send( struct bt_mesh_model *model,
    struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *msg,
    const struct bt_mesh_send_cb *cb, void *cb_data );

/**
 * @brief Register receive unknow opcode callback of a element.
 *
 * @param[in] elem_unkonw_op_cb   The receive unknow opcode callback.
 */
void bt_mesh_model_reg_elem_unkonw_op_cb(
    elem_unkonw_op_cb_t elem_unkonw_op_cb );

/*****************************************t_timer*************************************/

/**
 * @brief Initialize timer of mesh.
 */
void MeshTimer_Init( void );

/**
 * @brief Initialize deamon of mesh.
 */
void MeshDeamon_Init( void );

/*****************************************rf*************************************/

/**
 * @brief Initialize RF.
 */
void hal_rf_init( void );

/**
 * @brief Weather wait for end when RF tx.
 */
void hal_rf_tx_wait_enable(uint8_t enable );

/**
 * @brief Config the RF.
 *
 * @param[in] app_rfConfig   Paramter of RF.
 */
void app_rf_config( app_rfConfig_t *app_rfConfig );

/**
 * @brief Tx data.
 *
 * @param[in] txBuf       Tx data.
 * @param[in] txLen       Tx length.
 * @param[in] pktTxType   Tx package type
 *                        broadcast type(0xFF):received by all matching types;
 *                        others:only received by matching type.
 *
 * @return @ref Global_Error_Code.
 */
int app_rf_tx( uint8_t *txBuf, uint8_t txLen, uint8_t pktTxType );

/**
 * @brief Rx data.
 *
 * @param[in] pktTxType   Rx package type
 *                        broadcast type(0xFF):receive all matching types,
 *                        others:receive match type or broadcast type.
 *
 * @return @ref Global_Error_Code.
 */
int app_rf_rx( uint8_t pktRxType );

/**
 * @brief Stop tx/rx mode.
 */
void app_rf_stop( void );

/*****************************************api*************************************/

/**
 * @brief Reset mesh.
 */
void bt_mesh_reset( void );

/**
 * @brief Initialize mesh ECC.
 */
void ble_sm_alg_ecc_init( void );

/**
 * @brief Get IV index.
 *
 * @return IV index.
 */
uint32_t bt_mesh_iv_index_get( void );

/**
 * @brief Get key refresh and IV update flag of net.
 *
 * @param[in] net_idx   Net index
 */
uint8_t bt_mesh_net_flags_get( uint16_t net_idx );

/**
 * @brief Get SEQ number.
 *
 * @return SEQ number.
 */
uint32_t bt_mesh_seq_get( void );

/**
 * @brief Set SEQ number.
 *
 * @param[in] new_seq   New SEQ number, shall be less than 0xb0e500 and greater than current SEQ.
 *
 * @return @ref Global_Error_Code.
 */
int bt_mesh_seq_set( uint32_t new_seq );

/**
 * @brief Get RPL list.
 *
 * @param[in] rpl   Buffer to copy RPL list, the size of the buffer should not be less than CONFIG_MESH_RPL_COUNT_DEF.
 *
 * @return RPL list struct.
 */
struct bt_mesh_rpl* bt_mesh_rpl_list_get( struct bt_mesh_rpl *rpl );

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
