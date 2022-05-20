/*
 * SPDX-Licence-Identifier: MIT
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <drivers/gpio.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(home_mesh, CONFIG_LOG_DEFAULT_LEVEL);


#include "sys_handler.h"
#include "bt_le.h"
#include "display_test.h"


void main(void) {
  LOG_DBG("Home Mesh");


  le_init();


  while (1) {
    sys_evt_handler();
    k_yield();
  }
}
