cmake_minimum_required(VERSION 3.20.0)

set(BOARD nrf5340dk_nrf5340_cpuapp_ns)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(Macro_logging)

target_sources(app PRIVATE src/log_macro.c ../../../src/logger.c)