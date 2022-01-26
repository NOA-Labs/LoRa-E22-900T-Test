#pragma once

#include <Arduino.h>

#define FW_BUILD_VERSION                "build - "__DATE__","__TIME__

// Uncomment to enable printing out nice debug messages.
#define LoRa_E22_DBG

// Define where debug output will be printed.
#define DBG_PRINTER Serial

// Setup debug printing macros.
#ifdef LoRa_E22_DBG
#define DBG_PRINT(...) { DBG_PRINTER.print(__VA_ARGS__); }
#define DBG_PRINTLN(...) { DBG_PRINTER.println(__VA_ARGS__); }
#define DBG_PRINTF(format, ...) {DBG_PRINTER.printf(format, __VA_ARGS__);}
#define LORA_E22_TEST_MODE                          1
#else
#define DBG_PRINT(...) {}
#define DBG_PRINTLN(...) {}
#define DBG_PRINTF(format, ...) 
#define LORA_E22_TEST_MODE                          0
#endif
/**
 * select lora role :
 *  SYS_LORA_IS_HOST_MODE  1 : lora as host.
 */
#define SYS_LORA_IS_HOST_MODE               1
#define SYS_LORA_IS_SLAVE_MODE              ((SYS_LORA_IS_HOST_MODE == 0) ? 1 : 0)