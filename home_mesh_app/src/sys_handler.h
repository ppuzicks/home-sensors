/*
 * SPDX-Licence-Identifier: MIT
 */
#ifndef __SYS_HANDLER_H__
#define __SYS_HANDLER_H__

#include <zephyr.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {

  EVT_BT_CONNECTED = 0x10,
  EVT_BT_DISCONNECTED = 0x11,


} EventID;

typedef struct {
  EventID   id;
  uint32_t* ptr_msg;
} __attribute__((aligned(4))) EventMsg;


void put_sys_evt_msg(const EventID id, uint32_t* msg);
void sys_evt_handler(void);

#ifdef __cplusplus
}
#endif


#endif // __SYS_HANDLER_H__
