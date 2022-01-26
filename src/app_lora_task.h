#pragma once
#include "lora_setting.h"

#define LORA_MAX_BUFF_SIZE                              32

#define LORA_RECEIVE_WAIT_TIMEOUT                       400

enum LORA_STA{
LORA_STA_IDLE = 0,
LORA_STA_SEND,
LORA_STA_RECIEVE
};

int app_lora_task_create();
int app_lora_set_send_data(const void *buff, int size);
