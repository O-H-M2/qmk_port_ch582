/********************************** (C) COPYRIGHT ********************************************
 * File Name         : WCH_LWNS_LIB.h
 * Author            : WCH
 * Version           : V1.61
 * Date              : 2021/12/15
 * Description       : this file is a head file for WCH LightWeight Networking Stack.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef _WCH_LWNS_LIB_H_
#define _WCH_LWNS_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"

//@@***********************************************************************general definitions begin

#define VER_LWNS_FILE    "WCH_LWNS_LIB_V1.61"

#ifndef BOOL
typedef unsigned char BOOL;
#endif
#ifndef TRUE
  #define TRUE    1
#endif
#ifndef FALSE
  #define FALSE    0
#endif
#ifndef NULL
  #define NULL    0
#endif

//@@***********************************************************************general definitions end

//@@***********************************************************************lwns_addr definitions begin

//can not be modified
#define LWNS_ADDR_SIZE    6
typedef union
{
    uint8_t v8[LWNS_ADDR_SIZE];
} lwns_addr_t;

/*******************************************************************************
 * @fn          lwns_addr_cmp
 *
 * @brief       compare two address£¬return 1 if they are same.use lwns_memcmp ,the size is 6.
 *
 * input parameters
 *
 * @param       src1 - pointer to addr 1 data
 * @param       src2 - pointer to addr 2 data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      1 - same.
 *              0 - not same.
 */
extern int lwns_addr_cmp(const void *src1, const void *src2);

/*******************************************************************************
 * @fn          lwns_addr_set
 *
 * @brief       set lwns_addr with f.
 *
 * input parameters
 *
 * @param       f - pointer to lwns_addr need to set.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 *
 */
extern void lwns_addr_set(lwns_addr_t *f);

/*******************************************************************************
 * @fn          get_lwns_addr
 *
 * @brief       get lwns_addr,and save to t.
 *
 * input parameters
 *
 * @param       t - pointer to lwns_addr
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 *
 */
extern void get_lwns_addr(lwns_addr_t *t);

//@@***********************************************************************lwns_addr definitions end

//@@***********************************************************************lwns_HTIMER definitions begin

//htimer heart timer settings. htimer is used to calculate of the time that when need to send messages.
//user who used this lib need to supply a soft timer for htimer,the timer function must not in interrupt. the timer period is (1s/HTIMER_SECOND_NUM).

#define HTIMER_SECOND_NUM    50

typedef unsigned long lwns_clock_time_t;

/*******************************************************************************
 * @fn          lwns_htimer_update
 *
 * @brief       put lwns_htimer_update in the soft timer handler function by yourself.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_htimer_update(void);

/*******************************************************************************
 * @fn          lwns_htimer_flush_all
 *
 * @brief       remove all htimers in htimer list.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_htimer_flush_all(void);

//@@***********************************************************************lwns_HTIMER definitions end

//@@***********************************************************************lwns_neighbor definitions begin

//can not be modified
#define LWNS_NEIGHBOR_LIST_U8_SIZE    13
typedef struct _lwns_neighbor_list_struct
{
    uint8_t data[LWNS_NEIGHBOR_LIST_U8_SIZE];
} lwns_neighbor_list_t; //for user manual allocate

struct lwns_neighbor_info
{
    struct lwns_neighbor_info *next;
    lwns_addr_t                sender;
    uint8_t                    seqno;
    uint8_t                    time;
};

typedef enum
{
    LWNS_NEIGHBOR_AUTO_ADD_STATE_REC_TBONLY = 0,
    LWNS_NEIGHBOR_AUTO_ADD_STATE_RECALL_ADDALL,
    LWNS_NEIGHBOR_AUTO_ADD_STATE_RECALL_NOTADD,
} LWNS_NEIGHBOR_AUTO_ADD_STATE_t;

/*******************************************************************************
 * @fn          lwns_neighbor_mode_set
 *
 * @brief       if set to LWNS_NEIGHBOR_AUTO_ADD_STATE_REC_TBONLY,this node will only receive messages from
 *              the neighbors in neighbor table memory,
 *              if a new neighbor send a packet to it, it will drop it.
 *              if set to LWNS_NEIGHBOR_AUTO_ADD_STATE_RECALL_ADDALL,this node will receive messages from all the neighbors,
 *              if a new neighbor send a packet to it, it will receive it,add it to neighbor table.
 *              then call new_neighbor_callback in lwns_fuc_interface_t.
 *              if set to LWNS_NEIGHBOR_AUTO_ADD_STATE_RECALL_NOTADD,this node will receive messages from all the neighbors,
 *              but not add it to neighbor table.so it will not pass duplicate packets.
 *
 * input parameters
 *
 * @param       status,uint8_t type,value is defined in LWNS_NEIGHBOR_AUTO_ADD_STATE_t.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_neighbor_mode_set(uint8_t mode);

/*******************************************************************************
 * @fn          lwns_neighbor_lookup
 *
 * @brief       look up a neighbor info from neighbor table.
 *
 * input parameters
 *
 * @param       sender - the sender addr
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      NULL if not find, other is the pointer to the neighbor info.
 */
extern struct lwns_neighbor_info *lwns_neighbor_lookup(const lwns_addr_t *sender);

/*******************************************************************************
 * @fn          lwns_neighbor_add
 *
 * @brief       add a neighbor info to neighbor table.
 *
 * input parameters
 *
 * @param       sender - the sender addr.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      0 if success,other failed.
 */
extern int lwns_neighbor_add(const lwns_addr_t *sender);

/*******************************************************************************
 * @fn          lwns_neighbor_get
 *
 * @brief       get a neighbor info from neighbor table.
 *
 * input parameters
 *
 * @param       num - which one from first in neighbor table.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      NULL if not find, other is the pointer to the neighbor info.
 */
extern struct lwns_neighbor_info *lwns_neighbor_get(uint16_t num);

/*******************************************************************************
 * @fn          lwns_neighbor_flush_all
 *
 * @brief       remove all neighbors in neighbor table.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_neighbor_flush_all(void);

/*******************************************************************************
 * @fn          lwns_neighbor_num
 *
 * @brief       get neighbors number in memory.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      neighbors number in memory.
 */
extern int lwns_neighbor_num(void);

/*******************************************************************************
 * @fn          lwns_neighbor_remove
 *
 * @brief       remove a neighbor in neighbor table.
 *
 * input parameters
 *
 * @param       e - lwns_neighbor_info pointer
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_neighbor_remove(struct lwns_neighbor_info *e);

//@@***********************************************************************lwns_neighbor definitions end

//@@***********************************************************************lwns_buffer definitions begin

//can not be modified
#define LWNS_DATA_SIZE              200
#define LWNS_HEADER_SIZE            36

#define LWNS_PHY_OUTPUT_MIN_SIZE    9

//can not be modified
#define LWNS_QBUF_LIST_U8_SIZE      273
typedef struct _lwns_qbuf_ptr_struct
{
    uint8_t data[LWNS_QBUF_LIST_U8_SIZE];
} lwns_qbuf_list_t; //for user manual allocate

/*******************************************************************************
 * @fn          lwns_buffer_datalen
 *
 * @brief       get data length in lwns_buffer.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      data length in buffer
 */
extern uint16_t lwns_buffer_datalen(void);

/*******************************************************************************
 * @fn          lwns_buffer_dataptr
 *
 * @brief       get lwns_buffer data pointer in lwns_buffer.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      data pointer in buffer
 */
extern void *lwns_buffer_dataptr(void);

/*******************************************************************************
 * @fn          lwns_buffer_load_data
 *
 * @brief       load data to lwns_buffer.
 *
 * input parameters
 *
 * @param       from - the data pointer need to be load to lwns_buffer.
 * @param       len - the length need to be load to lwns_buffer.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      the length of data be load to lwns_buffer actually.
 */
extern int lwns_buffer_load_data(const void *from, uint16_t len);

/*******************************************************************************
 * @fn          lwns_buffer_save_data
 *
 * @brief       save data from lwns_buffer to another buffer.
 *
 * input parameters
 *
 * @param       to - the data pointer need to be saved to from lwns_buffer.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      the length of data be saved actually.
 */
extern int lwns_buffer_save_data(void *to);

/*******************************************************************************
 * @fn          lwns_buffer_clear
 *
 * @brief       clear lwns_buffer data in lwns_buffer.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_buffer_clear(void);

/*******************************************************************************
 * @fn          lwns_buffer_set_datalen
 *
 * @brief       set datalen of lwns_buffer data in lwns_buffer.can not over LWNS_DATA_SIZE.
 *
 * input parameters
 *
 * @param       len - the length of buffer data.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_buffer_set_datalen(uint8_t len);

//@@***********************************************************************lwns_buffer definitions end

//@@***********************************************************************lwns basic definitions begin

//the lwns function handler,used in all functions to control lwns.
typedef void *lwns_controller_ptr;

//must fill it with user function.
typedef struct _lwns_fuc_interface
{
    BOOL (*lwns_phy_output)
    (uint8_t *dataptr, uint8_t len);
    uint32_t (*lwns_rand)(void);
    void (*lwns_memcpy)(void *dst, const void *src, uint32_t len);
    void (*lwns_memset)(void *pDst, uint8_t Value, uint32_t len);
    BOOL (*lwns_memcmp)
    (const void *src1, const void *src2, uint32_t len); //return TRUE if same,FALSE not.
    void (*new_neighbor_callback)(lwns_addr_t *n);
} lwns_fuc_interface_t;

//config params during init process.
typedef struct _lwns_config
{
    void       *lwns_lib_name;
    void       *qbuf_ptr; //qbuf_buffer pointer.use lwns_qbuf_ptr_t to define memory block.
    void       *neighbor_list_ptr;
    void       *routetable_ptr; //route table of entry pointer,if you want use mesh,must initialize it.use lwns_route_entry_data_t to defined memory block.
    lwns_addr_t addr;
    uint16_t    routetable_num; //max route table of entry number.if you want use mesh,it cannot be 0.
    uint8_t     qbuf_num;       //max qbuf_buffer numbers.can not be 0.At least 1.
    uint8_t     neighbor_num;
    uint8_t     neighbor_mod;
} lwns_config_t;

//errors when init lwns_lib.
typedef enum
{
    LWNS_LIB_INIT_OK = 0,
    LWNS_LIB_INIT_ERR_LIB_NAME,
    LWNS_LIB_INIT_ERR_MEMCPY,
    LWNS_LIB_INIT_ERR_MEMSET,
    LWNS_LIB_INIT_ERR_MEMCMP,
    LWNS_LIB_INIT_ERR_PHY_OUTPUT,
    LWNS_LIB_INIT_ERR_RAND,
    LWNS_LIB_INIT_ERR_ADDR_NULL,
    LWNS_LIB_INIT_ERR_QBUF,
    LWNS_LIB_INIT_ERR_NEIGHBOR,
    LWNS_LIB_INIT_ERR_ROUTETABLE,
    LWNS_LIB_INIT_ERR_MAX,
} ERR_LWNS_LIB_INIT;

/*******************************************************************************
 * @fn          lwns_lib_init
 *
 * @brief       Init lwns lib.
 *
 * input parameters
 *
 * @param       fuc - fuction interface of lwns lib. should define by lwns_fuc_interface_t.
 * @param       cfg - init config params of lwns lib. should define by lwns_config_t.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      0-success. error defined @ ERR_LWNS_LIB_INIT.
 */
extern int lwns_lib_init(void *fuc, void *cfg);

/*******************************************************************************
 * @fn          lwns_lib_deInit
 *
 * @brief       deInit lwns lib,then you cannot use lwns_lib,if you use,maybe it will enter hardfault mode.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_lib_deInit(void);

/*******************************************************************************
 * @fn          lwns_input
 *
 * @brief       input data to lwns_lib stack buffer.
 *
 * input parameters
 *
 * @param       rxBuf - data pointer of buffer
 * @param       len   - data length
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_input(uint8_t *rxBuf, uint8_t len);

/*******************************************************************************
 * @fn          lwns_dataHandler
 *
 * @brief       use lwns_lib to analysis stack buffer data.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_dataHandler(void);

/*******************************************************************************
 * @fn          get_lwns_object_port
 *
 * @brief       get port num of a lwns_controller_ptr.
 *
 * input parameters
 *
 * @param       controller  - defined with lwns_controller_ptr.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      uint8_t,return port number of the lwns_controller_ptr.
 */
extern uint8_t get_lwns_object_port(lwns_controller_ptr controller);

/*******************************************************************************
 * @fn          lwns_controller_lookup
 *
 * @brief       use port_num to find an opened lwns_controller_ptr .
 *
 * input parameters
 *
 * @param       port_num  - uint8_t.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      lwns_controller_ptr,return the lwns_controller_ptr of this port number.
 */
extern lwns_controller_ptr lwns_controller_lookup(uint8_t port_num);

/*******************************************************************************
 * @fn          lwns_controller_pop
 *
 * @brief       pop an opened lwns_controller_ptr from lwns_lib,
 *              the lwns_controller_ptr will remove from lwns_lib,
 *              so it will not handle data for this returned lwns_controller_ptr.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      NULL if no opened in lib.other is an opened lwns_controller_ptr.
 */
extern lwns_controller_ptr lwns_controller_pop(void);

//@@***********************************************************************lwns basic definitions end

//@@***********************************************************************lwns_broadcast definitions begin

//can not be modified
#define LWNS_BROADCAST_CONTROLLER_U32_SIZE    4
typedef struct _lwns_broadcast_controller_struct
{
    uint32_t data[LWNS_BROADCAST_CONTROLLER_U32_SIZE];
} lwns_broadcast_controller;

struct lwns_broadcast_callbacks
{
    void (*recv)(lwns_controller_ptr ptr, const lwns_addr_t *sender);
    void (*sent)(lwns_controller_ptr ptr);
};

/*******************************************************************************
 * @fn          lwns_broadcast_init
 *
 * @brief       open a port for broadcast.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_broadcast_controller
 * @param       port_num    -value:(1-255)   - port to recognize data
 * @param       callbacks defined with lwns_broadcast_callbacks
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_broadcast_init(lwns_controller_ptr h, uint8_t port_num,
                               const struct lwns_broadcast_callbacks *u);

/*******************************************************************************
 * @fn          lwns_broadcast_close
 *
 * @brief       close a port for broadcast.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_broadcast_controller,
 *                  which must be initialized by lwns_broadcast_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_broadcast_close(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_broadcast_send
 *
 * @brief       send a broadcst message through lwns_controller_ptr h.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_broadcast_controller,
 *                  which must be initialized by lwns_broadcast_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_broadcast_send(lwns_controller_ptr h);

//@@***********************************************************************lwns_broadcast definitions end

//@@***********************************************************************lwns_multicast definitions begin

//can not be modified
#define LWNS_MULTICAST_CONTROLLER_U32_SIZE    6
typedef struct _lwns_multicast_controller_struct
{
    uint32_t data[LWNS_MULTICAST_CONTROLLER_U32_SIZE];
} lwns_multicast_controller;

struct lwns_multicast_callbacks
{
    void (*recv)(lwns_controller_ptr ptr, uint16_t subaddr, const lwns_addr_t *sender);
    void (*sent)(lwns_controller_ptr ptr);
};

/*******************************************************************************
 * @fn          lwns_multicast_init
 *
 * @brief       open a port for multinetflood.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_multicast_controller.
 * @param       port_num    -value:(1-255)   - port to recognize data
 * @param       subaddr      - the array pointer of subscribe addresses.
 * @param       sub_num      - the number of subscribe addresses.
 * @param       callbacks defined with lwns_multicast_callbacks
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_multicast_init(lwns_controller_ptr h, uint8_t port_num,
                               uint16_t *subaddr, uint8_t sub_num, const struct lwns_multicast_callbacks *u);

/*******************************************************************************
 * @fn          lwns_multicast_close
 *
 * @brief       close a port for multicast.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_multicast_controller,
 *                  which must be initialized by lwns_multicast_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_multicast_close(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_multicast_send
 *
 * @brief       send a multicast message to a subaddr. used lwns_controller_ptr h.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_multicast_controller,
 *                  which must be initialized by lwns_multicast_init.
 * @param       subaddr - the dst subaddr.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_multicast_send(lwns_controller_ptr h, uint16_t subaddr);

//@@***********************************************************************lwns_multicast definitions end

//@@***********************************************************************lwns_unicast definitions begin

//can not be modified
#define LWNS_UNICAST_CONTROLLER_U32_SIZE    5
typedef struct _lwns_unicast_controller_struct
{
    uint32_t data[LWNS_UNICAST_CONTROLLER_U32_SIZE];
} lwns_unicast_controller;

struct lwns_unicast_callbacks
{
    void (*recv)(lwns_controller_ptr ptr, const lwns_addr_t *sender);
    void (*sent)(lwns_controller_ptr ptr);
};

/*******************************************************************************
 * @fn          lwns_unicast_init
 *
 * @brief       open a port for unicast.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_unicast_controller.
 * @param       port_num    -value:(1-255)   - port to recognize data
 * @param       callbacks defined with lwns_unicast_callbacks
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_unicast_init(lwns_controller_ptr h, uint8_t port_num,
                             const struct lwns_unicast_callbacks *u);

/*******************************************************************************
 * @fn          lwns_unicast_close
 *
 * @brief       close a port for unicast.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_unicast_controller,
 *                  which must be initialized by lwns_unicast_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_unicast_close(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_unicast_send
 *
 * @brief       send a unicast message through lwns_controller_ptr h.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_unicast_controller,
 *                  which must be initialized by lwns_unicast_init.
 * @param       receiver - the addr of the dest node.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_unicast_send(lwns_controller_ptr h,
                             const lwns_addr_t  *receiver);

//@@***********************************************************************lwns_unicast definitions end

//@@***********************************************************************lwns_ruc definitions begin

//can not be modified
#define LWNS_RUC_CONTROLLER_U32_SIZE    25
typedef struct _lwns_ruc_controller_struct
{
    uint32_t data[LWNS_RUC_CONTROLLER_U32_SIZE];
} lwns_ruc_controller;

//reliable unicast
struct lwns_ruc_callbacks
{
    void (*recv)(lwns_controller_ptr ptr, const lwns_addr_t *sender);
    void (*sent)(lwns_controller_ptr ptr, const lwns_addr_t *to,
                 uint8_t retransmissions);
    void (*timedout)(lwns_controller_ptr ptr, const lwns_addr_t *to);
};

/*******************************************************************************
 * @fn          lwns_ruc_init
 *
 * @brief       open a port for reliable unicast.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_ruc_controller.
 * @param       port_num    -value:(1-255)   - port to recognize data
 * @param       retransmit_time  if dest node not give ack,we will retry to send message after this retransmit_time time.
 * @param       callbacks defined with lwns_ruc_callbacks
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_ruc_init(lwns_controller_ptr h, uint8_t port_num,
                         lwns_clock_time_t                retransmit_time,
                         const struct lwns_ruc_callbacks *u);

/*******************************************************************************
 * @fn          lwns_ruc_close
 *
 * @brief       close a port for reliable unicast.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_ruc_controller,
 *                  which must be initialized by lwns_ruc_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_ruc_close(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_ruc_send
 *
 * @brief       send a reliable unicast message used lwns_controller_ptr h.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_ruc_controller,
 *                  which must be initialized by lwns_ruc_init.
 * @param       receiver - the addr of the dest node.
 * @param       max_retransmissions - if the dest not give ack,we will retry for this set value times.the legal value is 1-255;
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_ruc_send(lwns_controller_ptr h,
                         const lwns_addr_t *receiver, uint8_t max_retransmissions);

/*******************************************************************************
 * @fn          lwns_ruc_is_busy
 *
 * @brief       get status of this ruc lwns_controller_ptr.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_ruc_controller,
 *                  which must be initialized by lwns_ruc_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if busy,0 if free.
 */
extern uint8_t lwns_ruc_is_busy(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_ruc_clean_info
 *
 * @brief       clean senders and seqno in lwns_ruc_controller.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_ruc_controller or lwns_rucft_controller,
 *                  which must be initialized by lwns_ruc_init/lwns_rucft_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if busy,0 if free.
 */
extern void lwns_ruc_clean_info(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_ruc_remove_an_info
 *
 * @brief       remove a sender info in lwns_ruc_controller.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_ruc_controller or lwns_rucft_controller,
 *                  which must be initialized by lwns_ruc_init/lwns_rucft_init.
 * @param       sender - the addr of the sender node.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_ruc_remove_an_info(lwns_controller_ptr h, lwns_addr_t *sender);

//@@***********************************************************************lwns_ruc definitions end

//@@***********************************************************************lwns_rucft definitions begin
//reliable unicast file transfer

//can not be modified
#define LWNS_RUCFT_CONTROLLER_U32_SIZE    35
typedef struct _lwns_rucft_controller_struct
{
    uint32_t data[LWNS_RUCFT_CONTROLLER_U32_SIZE];
} lwns_rucft_controller;

struct lwns_rucft_callbacks
{
    void (*write_file)(lwns_controller_ptr ptr, const lwns_addr_t *sender,
                       int offset, int flag, char *data, int len);
    int (*read_file)(lwns_controller_ptr ptr, int offset, char *to);
    void (*timedout)(lwns_controller_ptr ptr);
};

#define LWNS_RUCFT_DATASIZE    192  //rucft each packet size

enum
{
    LWNS_RUCFT_FLAG_NONE = 0,
    LWNS_RUCFT_FLAG_NEWFILE,
    LWNS_RUCFT_FLAG_END,
};

/*******************************************************************************
 * @fn          lwns_rucft_init
 *
 * @brief       open a port for reliable unicast file transfer.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_rucft_controller.
 * @param       port_num    -value:(1-255)   - port to recognize data
 * @param       retransmit_time  if dest node not give ack,we will retry to send message after this retransmit_time time.
 * @param       max_retransmissions - if the dest not give ack,we will retry for this set value times.
 * @param       callbacks defined with lwns_rucft_callbacks.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_rucft_init(lwns_controller_ptr h, uint8_t port_num,
                           lwns_clock_time_t retransmit_time, uint8_t max_retransmissions,
                           const struct lwns_rucft_callbacks *u);

/*******************************************************************************
 * @fn          lwns_rucft_close
 *
 * @brief       close a port for reliable unicast file transfer.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_rucft_controller,
 *                  which must be initialized by lwns_rucft_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_rucft_close(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_rucft_send
 *
 * @brief       start a reliable unicast file transfer to dest addr:receiver,used lwns_controller_ptr h.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_rucft_controller,
 *                  which must be initialized by lwns_rucft_init.
 * @param       receiver - the addr of the dest node.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_rucft_send(lwns_controller_ptr h,
                           const lwns_addr_t  *receiver);

//@@***********************************************************************lwns_rucft definitions end

//@@***********************************************************************lwns_netflood definitions begin

//can not be modified
#define LWNS_NETFLOOD_CONTROLLER_U32_SIZE    31
typedef struct _lwns_netflood_controller_struct
{
    uint32_t data[LWNS_NETFLOOD_CONTROLLER_U32_SIZE];
} lwns_netflood_controller;

struct lwns_netflood_callbacks
{
    int (*recv)(lwns_controller_ptr ptr, const lwns_addr_t *from,
                const lwns_addr_t *originator, uint8_t hops);
    void (*sent)(lwns_controller_ptr ptr);
    void (*dropped)(lwns_controller_ptr ptr);
};

/*******************************************************************************
 * @fn          lwns_netflood_init
 *
 * @brief       open a port for netflood.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_netflood_controller.
 * @param       port_num    -value:(1-255)   - port to recognize data
 * @param       queue_time - wait queue_time/2  with a random time to receive message.
 * @param       dups - cannot be 0,if this value is set to over 1,this controller will wait to receive the same message over this value,and then drop the message.
 * @param       hops - the message max transmit life time.when another node send this message,this value will -1 until it is 0.
 * @param       drop_old_packet - FALSE/TRUE,TURE will drop the old packet if we prepare to send a new packet.FALSE will send old message immediately.
 * @param       old_phase_value - this value is for self-healing when network error appeared,
 *                                if seqno is too many than old_phase_value smaller than in memory,
 *                                it will still handle this message.
 * @param       callbacks defined with lwns_netflood_callbacks
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_netflood_init(lwns_controller_ptr h, uint8_t port_num,
                              lwns_clock_time_t queue_time, uint8_t dups, uint8_t hops, uint8_t drop_old_packet,
                              uint8_t old_phase_value, const struct lwns_netflood_callbacks *u);

/*******************************************************************************
 * @fn          lwns_netflood_close
 *
 * @brief       close a port for netflood.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_netflood_controller,
 *                  which must be initialized by lwns_netflood_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_netflood_close(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_netflood_send
 *
 * @brief       start a netflood,used lwns_controller_ptr h.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_netflood_controller,
 *                  which must be initialized by lwns_netflood_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_netflood_send(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_netflood_clean_info
 *
 * @brief       clean senders and seqno in lwns_netflood_controller.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_netflood_controller/
 *                  lwns_uninetflood_controller/lwns_multinetflood_controller/lwns_mesh_controller,
 *                  which must be initialized by these controller init function.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if busy,0 if free.
 */
extern void lwns_netflood_clean_info(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_netflood_remove_an_info
 *
 * @brief       remove a sender info in lwns_netflood_controller.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_netflood_controller/
 *                  lwns_uninetflood_controller/lwns_multinetflood_controller/lwns_mesh_controller,
 *                  which must be initialized by these controller init function.
 * @param       sender - the addr of the sender node.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_netflood_remove_an_info(lwns_controller_ptr h, lwns_addr_t *sender);

/*******************************************************************************
 * @fn          lwns_netflood_seqno_set
 *
 * @brief       set seqno of the lwns_netflood_controller.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_netflood_controller/
 *                  lwns_uninetflood_controller/lwns_multinetflood_controller/lwns_mesh_controller,
 *                  which must be initialized by these controller init function.
 * @param       seqno    -value:(0-255)   - seqno need to set.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if busy,0 if free.
 */
extern void lwns_netflood_seqno_set(lwns_controller_ptr h, uint8_t seqno);

//@@***********************************************************************lwns_netflood definitions end

//@@***********************************************************************lwns_uninetflood definitions begin
//based on netflood

//can not be modified
#define LWNS_UNINETFLOOD_CONTROLLER_U32_SIZE    32
typedef struct _lwns_uninetflood_controller_struct
{
    uint32_t data[LWNS_UNINETFLOOD_CONTROLLER_U32_SIZE];
} lwns_uninetflood_controller;

struct lwns_uninetflood_callbacks
{
    void (*recv)(lwns_controller_ptr ptr, const lwns_addr_t *sender, uint8_t hops);
    void (*sent)(lwns_controller_ptr ptr);
};

/*******************************************************************************
 * @fn          lwns_uninetflood_init
 *
 * @brief       open a port for uninetflood.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_uninetflood_controller.
 * @param       port_num    -value:(1-255)   - port to recognize data
 * @param       queue_time - wait queue_time/2  with a random time to receive message.
 * @param       dups - cannot be 0,if this value is set to over 1,
 *                     this controller will wait to receive the same message over this value,
 *                     and then drop the message.
 * @param       hops - the message max transmit life time.when another node send this message,this value will -1 until it is 0.
 * @param       drop_old_packet - FALSE/TRUE,TURE will drop the old packet if we prepare to send a new packet.FALSE will send old message immediately.
 * @param       old_phase_value - this value is for self-healing when network error appeared,
 *                                if seqno is too many than old_phase_value smaller than in memory,
 *                                it will still handle this message.
 * @param       flood_choice - decide whether it will forword a received packet which is not for itself.
 * @param       callbacks defined with lwns_uninetflood_callbacks
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_uninetflood_init(lwns_controller_ptr h, uint8_t port_num,
                                 lwns_clock_time_t queue_time, uint8_t dups, uint8_t hops, uint8_t drop_old_packet, uint8_t old_phase_value,
                                 uint8_t flood_choice, const struct lwns_uninetflood_callbacks *u);

/*******************************************************************************
 * @fn          lwns_uninetflood_close
 *
 * @brief       close a port for uninetflood.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_uninetflood_controller,
 *                  which must be initialized by lwns_uninetflood_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_uninetflood_close(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_uninetflood_send
 *
 * @brief       start a uninetflood,send message to dst. used lwns_controller_ptr h.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_uninetflood_controller,
 *                  which must be initialized by lwns_uninetflood_init.
 * @param       dst - the addr of the dest node.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_uninetflood_send(lwns_controller_ptr h, lwns_addr_t *dst);

//@@***********************************************************************lwns_uninetflood definitions end

//@@***********************************************************************lwns_multinetflood definitions begin

//can not be modified
#define LWNS_MULTINETFLOOD_CONTROLLER_U32_SIZE    33
typedef struct _lwns_multinetflood_controller_struct
{
    uint32_t data[LWNS_MULTINETFLOOD_CONTROLLER_U32_SIZE];
} lwns_multinetflood_controller;

struct lwns_multinetflood_callbacks
{
    void (*recv)(lwns_controller_ptr ptr, uint16_t subaddr, const lwns_addr_t *sender, uint8_t hops);
    void (*sent)(lwns_controller_ptr ptr);
};

/*******************************************************************************
 * @fn          lwns_multinetflood_init
 *
 * @brief       open a port for multinetflood.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_uninetflood_controller.
 * @param       port_num    -value:(1-255)   - port to recognize data
 * @param       queue_time - wait queue_time/2  with a random time to receive message.
 * @param       dups - cannot be 0,if this value is set to over 1,
 *                     this controller will wait to receive the same message over this value,
 *                     and then drop the message.
 * @param       hops - the message max transmit life time.when another node send this message,this value will -1 until it is 0.
 * @param       drop_old_packet - FALSE/TRUE,TURE will drop the old packet if we prepare to send a new packet.FALSE will send old message immediately.
 * @param       old_phase_value - this value is for self-healing when network error appeared,
 *                                if seqno is too many than old_phase_value smaller than in memory,
 *                                it will still handle this message.
 * @param       flood_choice - decide whether it will forword a received packet which is not for itself.
 * @param       subaddr      - the array pointer of subscribe addresses.
 * @param       sub_num      - the number of subscribe addresses.
 * @param       callbacks defined with lwns_multinetflood_callbacks
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_multinetflood_init(lwns_controller_ptr h,
                                   uint8_t port_num, lwns_clock_time_t queue_time,
                                   uint8_t dups, uint8_t hops, uint8_t drop_old_packet, uint8_t old_phase_value, uint8_t flood_choice,
                                   uint16_t *subaddr, uint8_t sub_num,
                                   const struct lwns_multinetflood_callbacks *u);

/*******************************************************************************
 * @fn          lwns_multinetflood_close
 *
 * @brief       close a port for multinetflood.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_multinetflood_controller,
 *                  which must be initialized by lwns_multinetflood_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_multinetflood_close(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_multinetflood_send
 *
 * @brief       start a multinetflood,send message to a subaddr. used lwns_controller_ptr h.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_uninetflood_controller,
 *                  which must be initialized by lwns_uninetflood_init.
 * @param       subaddr - the dst subaddr.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_multinetflood_send(lwns_controller_ptr h, uint16_t subaddr);

//@@***********************************************************************lwns_multinetflood definitions end

//@@***********************************************************************lwns_route definitions begin

/* ROUTE_CONF_ENTRIES */
//the structure of route table
struct lwns_route_entry
{
    struct lwns_route_entry *next;
    lwns_addr_t              dest;
    lwns_addr_t              nexthop;
    uint8_t                  cost;
    uint8_t                  time;
};

#define LWNS_ROUTE_ENTRY_LIST_U8_SIZE    21
typedef struct _lwns_route_entry_data_struct
{
    uint8_t data[LWNS_ROUTE_ENTRY_LIST_U8_SIZE];
} lwns_route_entry_data_t; //for user manual allocate

/*******************************************************************************
 * @fn          lwns_route_init
 *
 * @brief       init lwns_route_management.
 *
 * input parameters
 *
 * @param       route_entry_duplicate - decide route table can save one or more route entry to a dest.
 * @param       max_life_time - 0-255,max_life_time is check period times,if max_life_time = 0,
 *                              it will not enable route auto clean.route will be always alive.
 *                              you can manage route table by yourself.
 * @param       periodic_time - a route table entry real life time is (max_life_time*periodic_time)
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_route_init(uint8_t route_entry_duplicate, uint8_t max_life_time,
                            lwns_clock_time_t periodic_time);

/*******************************************************************************
 * @fn          lwns_route_add
 *
 * @brief       add a route entry to route table.
 *
 * input parameters
 *
 * @param       dest - the dest addr
 * @param       nexthop - nexthop addr to the dest addr
 * @param       cost  - the hops from src to dest.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      0 is success.
 */
extern int lwns_route_add(const lwns_addr_t *dest,
                          const lwns_addr_t *nexthop, uint8_t cost);

/*******************************************************************************
 * @fn          lwns_route_lookup
 *
 * @brief       look up a route entry from route table.
 *
 * input parameters
 *
 * @param       dest - the dest addr
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      NULL if not find, other is the pointer to the entry.
 */
extern struct lwns_route_entry *lwns_route_lookup(
    const lwns_addr_t *dest);

/*******************************************************************************
 * @fn          lwns_route_refresh
 *
 * @brief       refresh the lifetime of a route entry in route table.
 *
 * input parameters
 *
 * @param       e - lwns_route_entry pointer
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_route_refresh(struct lwns_route_entry *e);

/*******************************************************************************
 * @fn          lwns_route_remove
 *
 * @brief       remove a route entry in route table.
 *
 * input parameters
 *
 * @param       e - lwns_route_entry pointer
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_route_remove(struct lwns_route_entry *e);

/*******************************************************************************
 * @fn          lwns_route_flush_all
 *
 * @brief       remove all route entry in route table.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_route_flush_all(void);

/*******************************************************************************
 * @fn          lwns_route_num
 *
 * @brief       get number of a route entry in route table.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      the number of a route entry in route table.
 */
extern int lwns_route_num(void);

//@@***********************************************************************lwns_route definitions end

//@@***********************************************************************lwns_mesh definitions begin

//can not be modified
#define LWNS_MESH_CONTROLLER_U32_SIZE    55
typedef struct _lwns_mesh_controller_struct
{
    uint32_t data[LWNS_MESH_CONTROLLER_U32_SIZE];
} lwns_mesh_controller;

//mesh functions must be used after you initialize route by lwns_route_init.
struct lwns_mesh_callbacks
{
    void (*recv)(lwns_controller_ptr ptr, const lwns_addr_t *sender,
                 uint8_t hops);
    void (*sent)(lwns_controller_ptr ptr);
    void (*timedout)(lwns_controller_ptr ptr);
};

/*******************************************************************************
 * @fn          lwns_mesh_init
 *
 * @brief       open three ports for mesh.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_mesh_controller.
 * @param       port_nums    -value:(1-255)  - port to recognize data,this controller will open 3 ports.so if you type in 128,then 128,129,130 port will be all opened.
 * @param       route_discovery_hoptime - wait route_discovery_hoptime/2  with a random time to receive message.
 * @param       dups - cannot be 0,if this value is set to over 1,this controller will wait to receive the same message over this value,and then drop the message.
 * @param       hops - the message max transmit life time.when another node send this message,this value will -1 until it is 0.
 * @param       drop_old_packet - FALSE/TRUE,TURE will drop the old packet if we prepare to send a new packet.FALSE will send old message immediately.
 * @param       old_phase_value - this value is for self-healing when network error appeared,
 *                                if seqno is too many than old_phase_value smaller than in memory,
 *                                it will still handle this message.
 * @param       route_enable - decide whether this node can be a route or not of this lwns_controller_ptr.
 * @param       route_loop_add_enable - decide whether add route_entry with a sender of a message received or not.
 * @param       mesh_over_time - this is to set find route timeout time.
 * @param       callbacks defined with lwns_mesh_callbacks
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_mesh_init(lwns_controller_ptr h, uint8_t port_nums,
                          lwns_clock_time_t route_discovery_hoptime, uint8_t dups, uint8_t hops, uint8_t rt_drop_old_packet,
                          uint8_t old_phase_value, uint8_t self_route_enable, uint8_t route_loop_add_enable,
                          lwns_clock_time_t                 mesh_over_time,
                          const struct lwns_mesh_callbacks *callbacks);

/*******************************************************************************
 * @fn          lwns_mesh_close
 *
 * @brief       close ports for mesh.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_mesh_controller,
 *                  which must be initialized by lwns_mesh_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void lwns_mesh_close(lwns_controller_ptr h);

/*******************************************************************************
 * @fn          lwns_mesh_send
 *
 * @brief       send a mesh message used lwns_controller_ptr h.
 *
 * input parameters
 *
 * @param       h - lwns_controller_ptr,the pointer of a lwns_mesh_controller,
 *                  which must be initialized by lwns_mesh_init.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      return 1 if success,0 if failed.
 */
extern int lwns_mesh_send(lwns_controller_ptr h,
                          const lwns_addr_t  *dest);

//@@***********************************************************************lwns_mesh definitions end

#ifdef __cplusplus
}
#endif

#endif /* _WCH_LWNS_LIB_H_ */
