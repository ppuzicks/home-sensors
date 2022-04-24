/*
 * SPDX-Licence-Identifier: MIT
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <drivers/gpio.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(home_mesh, CONFIG_LOG_DEFAULT_LEVEL);


void main(void) {
  LOG_DBG("Home Mesh");

}