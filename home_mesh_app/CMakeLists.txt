# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(home_mesh)

FILE(GLOB app_sources 
  src/main.c
  src/bt_le.c
  src/setting.c
  src/sys_handler.c)

target_sources(app PRIVATE ${app_sources})

