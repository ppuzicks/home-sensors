/*
 * SPDX-Licence-Identifier: MIT
 */

#include "bt_le.h"

#include <zephyr.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(home_mesh, CONFIG_LOG_DEFAULT_LEVEL);

static ssize_t write_char2(struct bt_conn* conn, const struct bt_gatt_attr* attr, const void* buf, uint16_t len, uint16_t offset, uint8_t flags);
static void char1_ccc_cfg_changed(const struct bt_gatt_attr* attr, uint16_t value); 

#define BT_UUID_HOME_MESH_SERVICE BT_UUID_128_ENCODE(0xf34ed76c, 0xcff0, 0x4c8a, 0xa845, 0xc1d42efd0456)

static struct bt_uuid_128 home_mesh_service_uuid = BT_UUID_INIT_128(BT_UUID_HOME_MESH_SERVICE);
static struct bt_uuid_128 home_mesh_char1_uuid = BT_UUID_INIT_128(
  BT_UUID_128_ENCODE(0xd9c3001, 0x9c01, 0x5f73, 0x7061, 0x63656e6f726d));
static struct bt_uuid_128 home_mesh_char2_uuid = BT_UUID_INIT_128(
  BT_UUID_128_ENCODE(0xd9c3002, 0x9c01, 0x5f73, 0x7061, 0x63656e6f726d));

static uint8_t notify_data[128];
static uint8_t write_data[128];
static uint8_t notify;


BT_GATT_SERVICE_DEFINE(home_svc,
  BT_GATT_PRIMARY_SERVICE(&home_mesh_service_uuid),
  BT_GATT_CHARACTERISTIC(&home_mesh_char1_uuid.uuid,
    BT_GATT_CHRC_NOTIFY,
    BT_GATT_PERM_NONE,
    NULL, NULL, notify_data),
  BT_GATT_CCC(char1_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
  BT_GATT_CHARACTERISTIC(&home_mesh_char2_uuid.uuid,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE_ENCRYPT,
    NULL, write_char2, &write_data)
);

static const struct bt_data home_mesh_service_adv[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
  BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_HOME_MESH_SERVICE),
};

static ssize_t write_char2(struct bt_conn* conn, const struct bt_gatt_attr* attr, const void* buf, uint16_t len, uint16_t offset, uint8_t flags) {
  LOG_DBG("");

  uint8_t *value = attr->user_data;

  if (offset + len > sizeof(write_data)) {
    return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
  }

  memcpy(value + offset, buf, len);
  value[offset + len] = 0;

  return len;
}

static void char1_ccc_cfg_changed(const struct bt_gatt_attr* attr, uint16_t value) {
  LOG_DBG("");
  notify = (value == BT_GATT_CCC_INDICATE) ? 1 : 0;
}

static void le_connected(struct bt_conn* conn, uint8_t err) {
  char addr[BT_ADDR_LE_STR_LEN];

  bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

  if (err){
    LOG_DBG("Failed to connect to %s (%u)", addr, err);
  }
  else {
    LOG_DBG("Connected %s", addr);
  }
}

static void le_disconnected(struct bt_conn* conn, uint8_t reason) {
  char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

  LOG_DBG("Disconnected from %s (reason 0x%02x)", addr, reason);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
  .connected = le_connected,
  .disconnected = le_disconnected,
};

int le_init(void) {
  LOG_DBG("Bluetooth initialized");

  int err = bt_enable(NULL);
  if (err) {
    return err;
  }

  err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, home_mesh_service_adv, ARRAY_SIZE(home_mesh_service_adv), NULL, 0);
  if (err) {
    LOG_ERR("Advertising failed to start (err %d)", err);
    return err;
  }

  return 0;
}