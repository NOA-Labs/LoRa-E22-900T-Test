#include "system_cfg.h"

#if (SYS_LORA_IS_SLAVE_MODE)

#include "app_lora_task.h"
#include "esp32-hal-timer.h"
#include "string.h"

#define LORA_TASK_STACK_SIZE                2048
#define LORA_TASK_PRIORITY                  10

TaskHandle_t    lora_task_handle_t = NULL;
SemaphoreHandle_t xSemaphoreLoraReceive = NULL;

class appLora *appLoRa;

uint8_t lora_sta = LORA_STA_RECIEVE;

void IRAM_ATTR aux_pin_interrupt_cb_func(void)
{
    static BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    switch(lora_sta){
        case LORA_STA_SEND:
            lora_sta = LORA_STA_RECIEVE;
        break;
        case LORA_STA_RECIEVE:
            xSemaphoreGiveFromISR(xSemaphoreLoraReceive, &xHigherPriorityTaskWoken);
            if( xHigherPriorityTaskWoken != pdFALSE )
            {
                portYIELD_FROM_ISR();
            }
        break;
        default: break;
    }
}

/**
 *****************************************************************************************************************
 * 
 ***************************************************************************************************************** 
 */

static void app_lora_task_entry(void *param)
{
    int length = 0;
    ResponseStructContainer receiveDataRssi;
    char recieve_buff[32] = {0};
    int rssi = 0;

    DBG_PRINTLN("app lora task entry");

    xSemaphoreLoraReceive = xSemaphoreCreateBinary();
    if(xSemaphoreLoraReceive == NULL){
        while(1);
    }

    appLoRa = new appLora();
    appLoRa->setMode(0);
    appLoRa->setLoraParam();
    delay(500);

    while(appLoRa->getLoraAuxPinSta() == LOW){
        delay(5);
    }    
    
    // do{
    //     length = appLoRa->available();
    //     appLoRa->readBytes(recieve_buff, length);
    // }while(length > 0);

    appLoRa->setLoraAuxPinAsInterruptMode(aux_pin_interrupt_cb_func);
    lora_sta = LORA_STA_RECIEVE;
    appLoRa->cleanUARTBuffer();

    while(1){
        // xSemaphoreTake(xSemaphoreLoraReceive, portMAX_DELAY);
        if( xSemaphoreTake( xSemaphoreLoraReceive, ( TickType_t ) LORA_RECEIVE_WAIT_TIMEOUT ) != pdTRUE ){
            appLoRa->cleanUARTBuffer();
            continue;
        }
        length = appLoRa->available();
        DBG_PRINTLN(length);
        receiveDataRssi = appLoRa->receiveDataRssi(length);
        
        rssi = receiveDataRssi.rssi;
        strncpy(recieve_buff, (const char *)receiveDataRssi.data, length);
        recieve_buff[length] = rssi;
        receiveDataRssi.close();
        
        DBG_PRINTLN(recieve_buff);
        DBG_PRINTF("recieve rssi is: %d\r\n", -(256 - rssi));
        app_lora_set_send_data(recieve_buff, length + 1);
        appLoRa->setLoraAuxPinAsInputMode();
        appLoRa->sendDataOnce();
        delay(5);
        appLoRa->setLoraAuxPinAsInterruptMode(aux_pin_interrupt_cb_func);
        lora_sta = LORA_STA_SEND;
    }
}

int app_lora_task_create()
{
    xTaskCreateUniversal(app_lora_task_entry,
    "lora task",
    LORA_TASK_STACK_SIZE,
    NULL,
    LORA_TASK_PRIORITY,
    &lora_task_handle_t,
    CONFIG_ARDUINO_RUNNING_CORE);

    if(lora_task_handle_t == NULL)return -1;

    DBG_PRINTLN("<app_lora.cpp>app lora task create.");
    return 0;
}

int app_lora_set_send_data(const void *buff, int size)
{
    appLoRa->fillDataToSendBuffer((const byte *)buff, size);
    return 0;
}

#endif