cmake_minimum_required(VERSION 3.20.0)

set(BOARD nrf5340dk_nrf5340_cpuapp_ns)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(Event_Driven_Logging)

target_sources(app PRIVATE src/evt-driven.c ../../../src/logger.c ../../../src/cpu_info.c)