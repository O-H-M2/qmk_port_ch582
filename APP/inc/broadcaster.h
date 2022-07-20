/********************************** (C) COPYRIGHT *******************************
* File Name          : broadcaster.h
* Author             : WCH
* Version            : V1.0
* Date               : 2018/12/11
* Description        : 
*******************************************************************************/

#ifndef BROADCASTER_H
#define BROADCASTER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Simple BLE Broadcaster Task Events
#define SBP_START_DEVICE_EVT                              0x0001
#define SBP_PERIODIC_EVT                                  0x0002
#define SBP_ADV_IN_CONNECTION_EVT                         0x0004

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the BLE Broadcaster Application
 */
extern void Broadcaster_Init( void );

/*
 * Task Event Processor for the BLE Broadcaster Application
 */
extern uint16 Broadcaster_ProcessEvent( uint8 task_id, uint16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif 
