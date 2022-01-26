#include "app_cli_task.h"

#define CLI_TASK_STACK_SIZE                    4096
#define CLI_TASK_PRIORITY                      6

TaskHandle_t cli_task_handle_t = NULL;

enum commandLineType{
CLI_TYPE_SET_LORA_DB = 0,
CLI_TYPE_SET_CHANNEL,
CLI_TYPE_SEND_DATA_VIA_LORA,
CLI_TYPE_MAX
};

static BaseType_t set_lora_send_power( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t set_lora_channel( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t send_data_use_lora( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

const xCommandLineInput  commandLineInputArray[CLI_TYPE_MAX] = {
{"set-lora-send-db",                 "set-lora-send-db x: x= set send power.\r\n", set_lora_send_power, 1},
{"set-lora-channel",                 "set-lora-channel x: x= lora channel number( 0 ~ 83).\r\n", send_data_use_lora, 1},
{"send-data-use-lora",               "send-data-use-lora x: x= send data.\r\n", send_data_use_lora, 1},
};

static void cli_task_entry(void *param)
{
    int length = 0;
    int totalLength = 0;
    char rx_buff[256] = {0};
    char write_buffer[256] = {0};

    for(int i = 0; i < CLI_TYPE_MAX; i++){
        FreeRTOS_CLIRegisterCommand(&commandLineInputArray[i]);
    }

    while(1){
        delay(50);
        length = Serial.available();
        if(length == 0){
            continue;
        }else{
            if(length > totalLength){
                totalLength = length;
                continue;
            }
        }
        totalLength = 0;
        Serial.read(rx_buff, length);

        FreeRTOS_CLIProcessCommand(rx_buff, write_buffer, sizeof(write_buffer));
        Serial.println(write_buffer);
    }
}

int cli_task_create()
{
    xTaskCreateUniversal(cli_task_entry,
    "cli task",
    CLI_TASK_STACK_SIZE,
    NULL,
    CLI_TASK_PRIORITY,
    &cli_task_handle_t,
    CONFIG_ARDUINO_RUNNING_CORE);

    if(cli_task_handle_t == NULL)return -1;

    DEBUG_PRINTLN("<app_cli_task.cpp>cli task create.");
    return 0;
}


static BaseType_t set_lora_send_power( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{

    return 0;
}

static BaseType_t set_lora_channel( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{

    return 0;
}

static BaseType_t send_data_use_lora( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    
    return 0;
}