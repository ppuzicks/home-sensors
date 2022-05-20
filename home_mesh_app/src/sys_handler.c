/*
 * SPDX-Licence-Identifier: MIT
 */

#include "sys_handler.h"

#include <logging/log.h>
LOG_MODULE_DECLARE(home_mesh, CONFIG_LOG_DEFAULT_LEVEL);


K_MSGQ_DEFINE(sys_evt_queue, sizeof(EventMsg), 10, 4);


void sys_evt_handler(void) {
  LOG_DBG("");

  EventMsg evt_msg;

  if (k_msgq_get(&sys_evt_queue, &evt_msg, K_FOREVER) == 0) {

  }
}

void put_sys_evt_msg(const EventID id, uint32_t* msg) {
  EventMsg evt_msg = {
    .id = id,
    .ptr_msg = msg,
  };

  while (k_msgq_put(&sys_evt_queue, &evt_msg, K_NO_WAIT) != 0) {
    /* message queue is full: purge old data & try again */
    k_msgq_purge(&sys_evt_queue);
  }
}
